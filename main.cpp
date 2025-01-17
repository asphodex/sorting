//
// Created by sphdx on 17.01.2025.
//

#include <string>
#include <fstream>
#include <iostream>
#include "src/include/licensePlate.h"
#include "src/include/path.h"
#include "src/include/personName.h"
#include "src/include/generator.h"
#include "src/include/sorting.h"

constexpr int CODE_ERROR_WRONG_INPUT = -1;

std::string error(const std::string &err) {
    return "[ERROR] " + err;
}

std::string warn(const std::string &err) {
    return "[WARN] " + err;
}

void readDataFromFile(const std::string &filePath, const int linesToRead, licensePlate *plates, personName *names) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << error("Can't open the file. Check the file path: ") << filePath << std::endl;
        return;
    }

    std::string line;
    for (int i = 0; i < linesToRead; ++i) {
        if (!std::getline(inputFile, line)) {
            std::cerr << error("Can't read the file. Something went wrong.") << std::endl;
            break;
        }

        // разделяем строку на два ключа
        const size_t spacePos = line.find(' ');
        if (spacePos == std::string::npos) {
            std::cerr << warn("Invalid input format. Here is the line: ") << line << std::endl;
            continue;
        }

        std::string plateStr = line.substr(0, spacePos);
        std::string nameStr = line.substr(spacePos + 1);

        plates[i] = licensePlate::parseLicensePlate(plateStr);

        // парсим personName
        const size_t firstSpace = nameStr.find(' ');
        const size_t secondSpace = nameStr.find(' ', firstSpace + 1);
        if (firstSpace == std::string::npos || secondSpace == std::string::npos) {
            std::cerr << warn("Invalid input format. Here is the line: ") << nameStr << std::endl;
            continue;
        }

        names[i].lastName = nameStr.substr(0, firstSpace);
        names[i].firstName = nameStr.substr(firstSpace + 1, secondSpace - (firstSpace + 1));
        names[i].middleName = nameStr.substr(secondSpace + 1);

        // заносим номер строки
        plates[i].line = i;
        names[i].line = i;
    }

    inputFile.close();
}

void writeToFile(const std::string &outputFilePath, const int linesToWrite, const licensePlate *plates,
                 const personName *names, bool eraseExistsDataInFile = false) {
    std::ofstream outputFile;

    if (eraseExistsDataInFile) {
        outputFile = std::ofstream(outputFilePath);
    } else {
        outputFile = std::ofstream(outputFilePath, std::ios::app);
    }

    if (!outputFile.is_open()) {
        std::cerr << error("Can't open the output file: ") << outputFilePath << std::endl;
        return;
    }

    outputFile << std::left
            << std::setw(20) << "License Plate"
            << std::setw(20) << "ID"
            << std::setw(20) << "Last Name"
            << std::setw(20) << "First Name"
            << std::setw(20) << "Middle Name"
            << std::setw(20) << "ID"
            << std::endl;

    for (int i = 0; i < linesToWrite; ++i) {
        outputFile << std::left
                << std::setw(20) << plates[i].toString()
                << std::setw(20) << plates[i].line
                << std::setw(20) << names[i].lastName
                << std::setw(20) << names[i].firstName
                << std::setw(20) << names[i].middleName
                << std::setw(20) << names[i].line
                << std::endl;
    }

    outputFile.close();
}

void addLineToFile(const std::string &outputFilePath, const std::string &lineToAdd) {
    std::ofstream outputFile(outputFilePath, std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << error("Can't open the output file: ") << outputFilePath << std::endl;
        return;
    }

    outputFile << lineToAdd << std::endl;

    outputFile.close();
}

int getLinesToReadFromUser() {
    int linesToRead = 0;
    std::cout << "Enter count of lines to read from file (N): ";
    std::cin >> linesToRead;
    if (linesToRead < 1 || linesToRead > 100'000'000) {
        std::cerr << error("Wrong file size. Exiting.") << std::endl;
        return CODE_ERROR_WRONG_INPUT;
    }
    return linesToRead;
}

template<typename T>
std::chrono::milliseconds sortAndBench(void sort(T *arr, int arraySize, sorting::Order), T *arr, const int arraySize,
                                       const sorting::Order order = sorting::Order::ASC) {
    const std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    sort(arr, arraySize, order);
    const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
}

// truncateFile erases data for a given file
void truncateFile(const std::string &filePath) {
    std::ofstream file(filePath, std::ofstream::trunc);
    if (!file) {
        std::cerr << error("Can't truncate the given file. Path: ") << filePath << std::endl;
        return;
    }
    file.close();
}

// void checkForDecimals() {
//     const int size = 100000;
//     auto arr = new int[size];
//
//     std::srand(std::time(nullptr));
//
//     for (int i = 0; i < size; ++i) {
//         arr[i] = std::rand() % 1000;
//     }
//
//     std::cout << "insertion: " << sortAndBench(sorting::insertionSort, arr, size, sorting::Order::ASC).count() << std::endl;
//
//     for (int i = 0; i < size; ++i) {
//         arr[i] = std::rand() % 1000;
//     }
//
//     std::cout << "shell: " << sortAndBench(sorting::shellSort, arr, size, sorting::Order::ASC).count() << std::endl;
//
//     delete[] arr;
// }

int main() {
    const std::string pathToInputFile = getAbsoluteFilePath("resources/input.txt");

    const int linesToRead = getLinesToReadFromUser();

    if (linesToRead == CODE_ERROR_WRONG_INPUT) {
        return -1;
    }

    Generator::generateAndWriteToFile(pathToInputFile, linesToRead);

    auto *plates1 = new licensePlate[linesToRead]{}, *plates2 = new licensePlate[linesToRead]{};
    auto *names1 = new personName[linesToRead]{}, *names2 = new personName[linesToRead]{};

    readDataFromFile(pathToInputFile, linesToRead, plates1, names1);
    readDataFromFile(pathToInputFile, linesToRead, plates2, names2);

    const std::string pathToFile1 = getAbsoluteFilePath("output/shell.txt"), pathToFile2 = getAbsoluteFilePath(
        "output/insertion.txt");

    truncateFile(pathToFile1);
    truncateFile(pathToFile2);

    {
        std::chrono::milliseconds sum = {};
        auto dur = sortAndBench(sorting::shellSort, plates1, linesToRead);
        sum += dur;
        addLineToFile(pathToFile1, "Time to sort for plates: " + std::to_string(dur.count()) + " ms.");

        dur = sortAndBench(sorting::shellSort, names1, linesToRead, sorting::Order::DESC);
        sum += dur;
        addLineToFile(pathToFile1, "Time to sort for names: " + std::to_string(dur.count()) + " ms.");

        addLineToFile(pathToFile1, "Total time: " + std::to_string(sum.count()) + " ms." + '\n');

        writeToFile(pathToFile1, linesToRead, plates1, names1);
    }

    {
        std::chrono::milliseconds sum = {};
        auto dur = sortAndBench(sorting::insertionSort, plates2, linesToRead);
        sum += dur;
        addLineToFile(pathToFile2, "Time to sort for plates: " + std::to_string(dur.count()) + " ms.");

        dur = sortAndBench(sorting::insertionSort, names2, linesToRead, sorting::Order::DESC);
        sum += dur;
        addLineToFile(pathToFile2, "Time to sort for names: " + std::to_string(dur.count()) + " ms.");

        addLineToFile(pathToFile2, "Total time: " + std::to_string(sum.count()) + " ms." + '\n');

        writeToFile(pathToFile2, linesToRead, plates2, names2);
    }

    delete [] plates1;
    delete [] plates2;
    delete [] names1;
    delete [] names2;
}