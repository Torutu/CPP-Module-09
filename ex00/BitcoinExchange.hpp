#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>

#include <fstream>
#include <sstream>
#include <iostream>
//iostream includes <cstdlib>, sstream includes <string>

class BitcoinExchange {
private:
    std::map<std::string, float> _bitcoinPrices;

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    BitcoinExchange& operator=(const BitcoinExchange& other);
    ~BitcoinExchange();

    void loadDatabase(const std::string& filename);
    void processInput(const std::string& filename);

private:
    bool isValidDate(const std::string& date) const;
    bool isValidValue(float value) const;
    std::string findClosestDate(const std::string& date) const;
};

#endif