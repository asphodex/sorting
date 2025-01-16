//
// Created by sphdx on 17.01.2025.
//

#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <fstream>
#include <random>
#include <string>

// Generator have two options:
// 1. generate a line like A123AA String String String
// 2. fill the file with path specified n times using option 1
class Generator {
    static std::random_device rd;
    static std::mt19937 gen;

    // generateRandomUppercaseLetter generates random letter in upper case between A and Z
    static char generateRandomUppercaseLetter() {
        std::uniform_int_distribution<> dis('A', 'Z');
        return static_cast<char>(dis(gen));
    }

    // generateRandomNumber generates random number between 100 and 999
    static int generateRandomNumber() {
        std::uniform_int_distribution<> dis(100, 999);
        return dis(gen);
    }

    // generateRandomWord generates random word with first letter in upper case
    static std::string generateRandomWord() {
        std::uniform_int_distribution<> length_dis(3, 15);
        std::uniform_int_distribution<> letter_dis('a', 'z');

        const int length = length_dis(gen);
        std::string word;
        word += generateRandomUppercaseLetter();
        for (int i = 1; i < length; ++i) {
            word += static_cast<char>(letter_dis(gen));
        }
        return word;
    }

public:
    // generateRandomString generates string like 'A123AA String String String'
    static std::string generateRandomString() {
        std::string result;

        result += generateRandomUppercaseLetter();
        const int number = generateRandomNumber();
        result += std::to_string(number);
        result += generateRandomUppercaseLetter();
        result += generateRandomUppercaseLetter();

        result += " " + generateRandomWord();
        result += " " + generateRandomWord();
        result += " " + generateRandomWord();

        return result;
    }

    // generateAndWriteToFile fill the file with path specified n times using generateRandomString()
    static void generateAndWriteToFile(const std::string &filePath, const int numberOfLines) {
        std::ofstream outFile(filePath);
        if (!outFile) {
            std::cerr << "[ERROR] Can't open the file. Path: " << filePath << std::endl;
            return;
        }

        for (int i = 0; i < numberOfLines; ++i) {
            outFile << generateRandomString() << std::endl;
        }

        outFile.close();
    }
};

std::random_device Generator::rd;
std::mt19937 Generator::gen(rd());

#endif //GENERATOR_H
