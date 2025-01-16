//
// Created by sphdx on 16.01.2025.
//

#include "include/path.h"

std::string getAbsoluteFilePath(const std::string &relativePath) {
    const std::filesystem::path currentPath = std::filesystem::current_path();
    const std::filesystem::path absoluteFilePath = currentPath / "../" / relativePath;

    return absoluteFilePath.string();
}