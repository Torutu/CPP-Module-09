#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib> // for strtod

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    _bitcoinPrices = other._bitcoinPrices;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        _bitcoinPrices = other._bitcoinPrices;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open database file.");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date;
        float price;
        if (std::getline(ss, date, ',') && ss >> price) {
            _bitcoinPrices[date] = price;
        }
    }
}

void BitcoinExchange::processInput(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, valueStr;
        if (std::getline(ss, date, '|') && std::getline(ss, valueStr)) {
            // Trim whitespace
            date.erase(date.find_last_not_of(' ') + 1);
            valueStr.erase(0, valueStr.find_first_not_of(' '));

            // Validate date
            if (!isValidDate(date)) {
                std::cerr << "Error: bad input => " << date << std::endl;
                continue;
            }

            // Validate value
            char* endPtr;
            float value = std::strtof(valueStr.c_str(), &endPtr);
            if (*endPtr != '\0' || !isValidValue(value)) {
                std::cerr << "Error: invalid value => " << valueStr << std::endl;
                continue;
            }

            // Find closest date and calculate Bitcoin value
            std::string closestDate = findClosestDate(date);
            float bitcoinValue = value * _bitcoinPrices[closestDate];
            std::cout << date << " => " << value << " = " << bitcoinValue << std::endl;
        }
    }
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    // Basic date format validation (e.g., 2011-01-03)
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    return true;
}

bool BitcoinExchange::isValidValue(float value) const {
    return value >= 0 && value <= 1000;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const {
    std::map<std::string, float>::const_iterator it = _bitcoinPrices.lower_bound(date);
    if (it == _bitcoinPrices.begin() && it->first != date) {
        return it->first;
    }
    if (it->first != date) {
        --it;
    }
    return it->first;
}