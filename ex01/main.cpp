#include "RPN.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./RPN \"<RPN expression>\"" << std::endl;
        return 1;
    }

    try {
        RPN calculator;
        calculator.setExpression(argv[1]);
        std::cout << calculator.calculate() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    return 0;
}
