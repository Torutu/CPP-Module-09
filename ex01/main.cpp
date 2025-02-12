#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./RPN \"<RPN expression>\"" << std::endl;
        return 1;
    }

    try {
        std::string expression = argv[1];
        std::cout << calculateRPN(expression) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    return 0;
}
