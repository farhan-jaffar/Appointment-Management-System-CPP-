#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <string>
#include <sys/stat.h>
#include <direct.h>
#include <fstream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <map>

std::string getLineInput(const std::string& prompt);
void ensureDataDirExists();
void backupFile(const std::string& filename);

namespace Utils {
    static inline const std::string DATA_DIR = "data";
    
    static bool ensureDirectoryExists(const std::string& path) {
        struct stat info;
        if (stat(path.c_str(), &info) != 0) {
            return _mkdir(path.c_str()) == 0;
        }
        return (info.st_mode & S_IFDIR) != 0;
    }

    static std::string getDataPath(const std::string& filename) {
        ensureDirectoryExists(DATA_DIR);
        return DATA_DIR + "/" + filename;
    }

    static bool isFileValid(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    static void backupFile(const std::string& filename) {
        if (!isFileValid(filename)) return;
        std::string backupName = filename + ".bak";
        std::ifstream src(filename, std::ios::binary);
        std::ofstream dst(backupName, std::ios::binary);
        dst << src.rdbuf();
    }

    // Data validation methods
    bool isValidID(const std::string& id);
    bool isValidName(const std::string& name);
    bool isValidSpecialization(const std::string& spec);
    bool isValidDate(const std::string& date);
    bool isValidTime(const std::string& time);
    bool isValidUrgencyLevel(int level);  // 1 is highest priority, 4 is lowest priority
    bool isValidSector(const std::string& sector);

    // Helper function to throw formatted validation errors
    void validateOrThrow(bool condition, const std::string& message);

    int getSafeInt(const std::string& prompt);
    std::string getLineInput(const std::string& prompt);
}

#endif

