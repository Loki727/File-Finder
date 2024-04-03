// FileFinder.h: plik dołączany dla standardowych systemowych plików dołączanych,
// lub pliki dołączane specyficzne dla projektu.

#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
// W tym miejscu przywołaj dodatkowe nagłówki wymagane przez program.


//const int MAXTHREAD = 4;
std::mutex mtx; // Mutex dla bezpiecznego dostępu do zapisywania wyników

// Stałe 
