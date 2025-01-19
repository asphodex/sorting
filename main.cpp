//
// Created by sphdx on 17.01.2025.
//

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "src/include/licensePlate.h"
#include "src/include/path.h"
#include "src/include/personName.h"
#include "src/include/generator.h"
#include "src/include/sorting.h"
#include "src/include/sortingType.h"

constexpr int CODE_ERROR_WRONG_INPUT = -1;

std::string error(const std::string &err) {
    return "[ERROR] " + err;
}

std::string warn(const std::string &err) {
    return "[WARN] " + err;
}

void readDataFromFile(const std::string &filePath, const int linesToRead, sortingType *arr) {
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

        arr[i].licensePlate = licensePlate::parseLicensePlate(plateStr);

        // парсим personName
        const size_t firstSpace = nameStr.find(' ');
        const size_t secondSpace = nameStr.find(' ', firstSpace + 1);
        if (firstSpace == std::string::npos || secondSpace == std::string::npos) {
            std::cerr << warn("Invalid input format. Here is the line: ") << nameStr << std::endl;
            continue;
        }

        arr[i].personName.lastName = nameStr.substr(0, firstSpace);
        arr[i].personName.firstName = nameStr.substr(firstSpace + 1, secondSpace - (firstSpace + 1));
        arr[i].personName.middleName = nameStr.substr(secondSpace + 1);

        // заносим номер строки
        arr[i].line = i + 1;
    }

    inputFile.close();
}

void writeToFile(const std::string &outputFilePath, const int linesToWrite, const sortingType *arr,
                 bool eraseExistsDataInFile = false) {
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
            << std::setw(20) << "Last Name"
            << std::setw(20) << "First Name"
            << std::setw(20) << "Middle Name"
            << std::setw(20) << "ID"
            << std::endl;

    for (int i = 0; i < linesToWrite; ++i) {
        outputFile << std::left
                << std::setw(20) << arr[i].licensePlate.toString()
                << std::setw(20) << arr[i].personName.lastName
                << std::setw(20) << arr[i].personName.firstName
                << std::setw(20) << arr[i].personName.middleName
                << std::setw(20) << arr[i].line
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

    if (linesToRead < 1 || linesToRead > 1'000'000) {
        std::cerr << error("Wrong file size. Exiting.") << std::endl;
        return CODE_ERROR_WRONG_INPUT;
    }
    return linesToRead;
}

bool getIsNeedGenerateInputFromUser() {
    std::string isNeedGenerateInput{};
    std::cout << "Need to generate input data? (Yes/y/No): ";
    std::cin >> isNeedGenerateInput;
    std::cout << std::endl;

    if (isNeedGenerateInput == "y" || isNeedGenerateInput == "yes" || isNeedGenerateInput == "Y" || isNeedGenerateInput
        == "Yes") {
        return true;
    }
    return false;
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

// void testIsSortStable() {
//     sorting::testing::isSortStableCout(sorting::insertionSort, sorting::Order::ASC, true);
//     sorting::testing::isSortStableCout(sorting::shellSort, sorting::Order::ASC, true);
// }

int main() {
    const std::string pathToInputFile = getAbsoluteFilePath("resources/input.txt");

    const int linesToRead = getLinesToReadFromUser();

    if (linesToRead == CODE_ERROR_WRONG_INPUT) {
        return -1;
    }
    if (getIsNeedGenerateInputFromUser()) Generator::generateAndWriteToFile(pathToInputFile, linesToRead);

    {
        const std::string pathToFile = getAbsoluteFilePath("output/insertion.txt");
        truncateFile(pathToFile);

        const auto arr = new sortingType[linesToRead]{};
        readDataFromFile(pathToInputFile, linesToRead, arr);

        const auto dur = sortAndBench(sorting::insertionSort, arr, linesToRead);
        const std::string isStable = sorting::testing::isSortStable(sorting::insertionSort) ? "Yes" : "No";

        addLineToFile(pathToFile, "Total time: " + std::to_string(dur.count()) + " ms." + '\n');
        addLineToFile(pathToFile, "Is sort stable? " + isStable + '\n');
        writeToFile(pathToFile, linesToRead, arr);

        delete [] arr;
    }

    {
        const std::string pathToFile = getAbsoluteFilePath("output/shell.txt");
        truncateFile(pathToFile);

        const auto arr = new sortingType[linesToRead]{};
        readDataFromFile(pathToInputFile, linesToRead, arr);

        const auto dur = sortAndBench(sorting::shellSort, arr, linesToRead);
        const std::string isStable = sorting::testing::isSortStable(sorting::shellSort) ? "Yes" : "No";

        addLineToFile(pathToFile, "Total time: " + std::to_string(dur.count()) + " ms." + '\n');
        addLineToFile(pathToFile, "Is sort stable? " + isStable + '\n');
        writeToFile(pathToFile, linesToRead, arr);

        delete [] arr;
    }
    return 0;
}
