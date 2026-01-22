#include "Utils.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <ctime>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

namespace Utils {
    std::string getLineInput(const std::string& prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    bool isValidID(const std::string& id) {
        try {
            int numID = std::stoi(id);
            return numID > 0;
        } catch (...) {
            return false;
        }
    }

    bool isValidName(const std::string& name) {
        if (name.empty() || name.length() > 30) return false;
        return std::regex_match(name, std::regex("^[a-zA-Z0-9\\s]+$"));
    }

    bool isValidSpecialization(const std::string& spec) {
        if (spec.empty() || spec.length() > 20) return false;
        return std::regex_match(spec, std::regex("^[a-zA-Z0-9\\s]+$"));
    }

    bool isValidDate(const std::string& date) {
        std::regex datePattern("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-([0-9]{4})$");
        return std::regex_match(date, datePattern);
    }

    bool isValidTime(const std::string& time) {
        std::regex timePattern("^([01][0-9]|2[0-3]):([0-5][0-9])$");
        return std::regex_match(time, timePattern);
    }

    bool isValidUrgencyLevel(int level) {
        return level >= 1 && level <= 4;  // 1 is highest priority, 4 is lowest priority
    }

    bool isValidSector(const std::string& sector) {
        static const std::vector<std::string> validSectors = {"G-9", "G-10", "F-8", "F-9", "F-10"};
        return std::find(validSectors.begin(), validSectors.end(), sector) != validSectors.end();
    }

    void validateOrThrow(bool condition, const std::string& message) {
        if (!condition) {
            throw std::invalid_argument(message);
        }
    }

    int getSafeInt(const std::string& prompt) {
        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);
            try {
                return std::stoi(input);
            } catch (...) {
                std::cout << "Please enter a valid number.\n";
            }
        }
    }
}

string getLineInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

void ensureDataDirExists() {
    filesystem::path dataDir = "data";
    if (!filesystem::exists(dataDir)) {
        filesystem::create_directory(dataDir);
    }
}

void backupFile(const string& filename) {
    if (!filesystem::exists(filename)) return;
    
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    
    stringstream backupName;
    backupName << filename << "." 
               << (1900 + ltm->tm_year) 
               << (1 + ltm->tm_mon) 
               << ltm->tm_mday 
               << "_" 
               << ltm->tm_hour 
               << ltm->tm_min 
               << ltm->tm_sec 
               << ".bak";
               
    filesystem::copy_file(filename, backupName.str(), 
                         filesystem::copy_options::overwrite_existing);
} 