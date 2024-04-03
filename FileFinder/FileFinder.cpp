// FileFinder.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "FileFinder.h"

namespace fs = std::filesystem;


bool isArg(const std::string& arg, const std::string& value) {
	return arg == value;
}

bool searchFileInDirectory(const std::string& directory, const std::string& filename, unsigned int depth);
bool searchFileInDirectoryAll(const std::string& directory, const std::string& filename, unsigned int depth);
void multiSearchFileInDirectory(const std::string& directory, const std::string& filename, unsigned int depth);

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "Użycie: " << argv[0] << " <Path> <File to Find> [<deep>]" << std::endl;
		std::cout << " - Parametr <Path>: Scieżka do folderu, w ktorym ma zostac wykonane wyszukiwanie." << std::endl;
		std::cout << " - Parametr <File to Find>: Nazwa pliku do wyszukania." << std::endl;
		std::cout << " - Parametr [<deep>]: (Opcjonalny) Glebokosc rekursji podczas przeszukiwania folderow." << std::endl;
		std::cout << "   Wartość 0 oznacza przeszukiwanie tylko w danym folderze, wartość wieksza od 0 oznacza przeszukiwanie w podfolderach." << std::endl;
		std::cout << "[-a | -m] : (Optional)Additional options for the search :" << std::endl;
		std::cout << "	-a : Search for all occurrences of the file within the directory tree." << std::endl;
		std::cout << "	- m : Perform a multi - threaded search for improved performance." << std::endl;
		return 0;
	}

	else if (argc < 3) {
		std::cerr << "Błąd: Brak wymaganego argumentu <File to Find>." << std::endl;
		std::cout << "Użycie: " << argv[0] << " <Path> <File to Find> [<deep>]" << std::endl;
		return 1;
	}
	std::string directory = argv[1];;
	std::string filename = argv[2];
	unsigned int depth = (argc > 3) ? std::stoi(argv[3]) : 0; // Można zmieniać tę wartość
	
	if (argc < 4)
	{
		if (isArg(argv[4], "-a")) {
			std::cout << "Wyszukuje wszystkie pliki z nazwą: " << filename << std::endl;
			auto start = std::chrono::high_resolution_clock::now();
			searchFileInDirectoryAll(directory, filename, depth);
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
			std::cout << "Czas dzialania funkcji: " << duration.count() << " milisekund" << std::endl;
			return 0;
		}
		else if (isArg(argv[4], "-m")) {
			auto start = std::chrono::high_resolution_clock::now();
			multiSearchFileInDirectory(directory, filename, depth);
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
			std::cout << "Czas dzialania funkcji: " << duration.count() << " milisekund" << std::endl;
			return 0;
		}
		else{
			std::cout << "Opcja" << argv[4] <<"nie istnieje"<< std::endl;
		}
	}
	std::cout << "Wyszukuje plik " << filename << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	if (!searchFileInDirectory(directory, filename, depth)) {
		std::cout << "Nie znaleziono pliku " << filename << " w folderze " << fs::absolute(directory) << std::endl;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Czas dzialania funkcji: " << duration.count() << " milisekund" << std::endl;

	return 0;
}
bool searchFileInDirectory(const std::string& directory, const std::string& filename, unsigned int depth) {
	// Wyszukiwanie w folderze
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_regular_file() && entry.path().filename() == filename) {
			std::cout << "Znaleziono plik: " << filename << " \n w folderze: " << fs::absolute(directory) << std::endl;
			return true;
		}
	}
	 if (depth != 0) {
		 for (const auto& entry : fs::directory_iterator(directory)) {
			 if (entry.is_directory()) {
				 std::string subdir = entry.path().string();
				 if (searchFileInDirectory(subdir, filename, depth - 1)) {
					 return true;
				 }
			 }
		 }
	}
	return false;
}

bool searchFileInDirectoryAll(const std::string& directory, const std::string& filename, unsigned int depth) {
	bool tmp = false;
	// Wyszukiwanie w folderze
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_regular_file() && entry.path().filename() == filename) {
			mtx.lock();
			std::cout << "Znaleziono plik: " << filename << " \n w folderze: " << fs::absolute(directory) << std::endl;
			mtx.unlock();
			tmp= true;
		}
	}
	if (depth != 0) {
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (entry.is_directory()) {
				std::string subdir = entry.path().string();
				if (searchFileInDirectoryAll(subdir, filename, depth - 1)) {
					tmp=true;
				}
			}
		}
	}
	return tmp;
}

void multiSearchFileInDirectory(const std::string& directory, const std::string& filename, unsigned int depth) {
	int folderCount = 0;
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_directory()) {
			folderCount++;
		}
	}
	if (folderCount > 4 && depth > 1) {
		std::vector<std::thread> threads;
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (entry.is_directory()) {
				std::string subdir = entry.path().string();
				threads.emplace_back(searchFileInDirectoryAll, subdir, filename, depth);
			}
		}
		for (auto& th : threads) {
			th.join();
		}
	}
	else {
		searchFileInDirectoryAll(directory, filename, depth);
	
	}
}