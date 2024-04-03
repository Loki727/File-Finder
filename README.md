# File-Finder
File Finder is a command-line utility written in C++ that allows users to search for a specific file within a directory and its subdirectories.

## Features
* Search for a file by name within a specified directory.
* Option to specify the depth of recursion for subdirectory search.
* Option to search for all occurrences of the file within the directory tree.
* Multi-threaded search for improved performance in scenarios with multiple subdirectories.

## Usage
To use File Finder, follow these steps:

Compile: Compile the program using a C++ compiler. For example:

1. **Compile**: Compile the program using a C++ compiler. For example: 
```bash
  g++ FileFinder.cpp -o FileFinder -std=c++17 -lpthread
```
2. **Execute**: Run the compiled executable with the following command-line arguments
```bash
  ./FileFinder <Path> <File to Find> [<depth>] [-a | -m]
```
  ``` <Path>```: Path to the directory where the search will be performed.
  ```<File to Find>```: Name of the file to search for.
  ```[<depth>]```: (Optional) Depth of recursion for subdirectory search. Use 0 for searching only in the specified directory.
  ```[-a | -m]```: (Optional) Additional options for the search:
    -a: Search for all occurrences of the file within the directory tree.
    -m: Perform a multi-threaded search for improved performance.
## Requirements
* C++17 compatible compiler
* POSIX Threads (pthread) library

## License
This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License.



