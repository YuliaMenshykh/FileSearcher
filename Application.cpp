#include "Finder.h"
#include <iostream>
int main()
{
    std::cout << "Writ a path\n";
    std::string filePath;
    std::cin >> filePath;
    if (std::filesystem::exists(filePath))
    {
        std::cout << "Let's search!\n";
    }
    else
    {
        std::cout << "It's a wrong path\n";
    }

    std::string fileName;
    std::cout << "Writ a full name\n";
    std::cin >> fileName;
    ThreadPool threadPool(8);
    searchFileInDirectory(threadPool, filePath, fileName);
}