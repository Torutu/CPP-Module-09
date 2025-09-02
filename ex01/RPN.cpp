#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>

static bool isNumber(const std::string& token) {
    if (token.empty()) return false;

    size_t start = 0;
    if (token[0] == '-' || token[0] == '+') {
        if (token.size() == 1) return false; // just "+" or "-" is not a number
        start = 1;
    }

    for (size_t i = start; i < token.size(); ++i) {
        if (!isdigit(token[i])) return false;
    }

    int value = std::stoi(token);
    if (value < -9 || value > 9) return false;

    return true;
}

int calculateRPN(const std::string& expression) {
    std::stack<int> stack;
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token) {
        if (isNumber(token)) {  // If the token is a number
            stack.push(std::stoi(token));
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stack.size() < 2) {
                throw std::invalid_argument("Error: Not enough operands.");
            }

            int b = stack.top();
            stack.pop();
            int a = stack.top();
            stack.pop();
            int result;
            if (token == "+") {
                result = a + b;
            } else if (token == "-") {
                result = a - b;
            } else if (token == "*") {
                result = a * b;
            } else if (token == "/") {
                if (b == 0) {
                    throw std::invalid_argument("Error: Division by zero.");
                }
                result = a / b;
            }
            stack.push(result);
        } else {
            throw std::invalid_argument("Error: Invalid token.");
        }
    }

    if (stack.size() != 1) {
        throw std::invalid_argument("Error: Invalid expression.");
    }

    return stack.top();
}
