#include "RPN.hpp"

// ---------------- constructor and destructor ----------------
RPN::RPN() : _expression("") {}

RPN::RPN(const RPN& other) {
    _expression = other._expression;
    _stack = other._stack;
}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        _expression = other._expression;
        _stack = other._stack;
    }
    return *this;
}

RPN::~RPN() {}

// ---------------- Member functions ----------------
void RPN::setExpression(const std::string& expr) {
    _expression = expr;
}

std::string RPN::getExpression() const {
    return _expression;
}

bool RPN::isNumber(const std::string& token) const {
    if (token.empty()) return false;
    size_t start = 0;
    if (token[0] == '-' || token[0] == '+') {
        if (token.size() == 1) return false;
        start = 1;
    }

    for (size_t i = start; i < token.size(); ++i) {
        if (!isdigit(token[i])) return false;
    }

    int value = std::stoi(token);
    return value >= -9 && value <= 9;
}

int RPN::calculate() {
    _stack = std::stack<int>();
    std::stringstream ss(_expression);
    std::string token;

    while (ss >> token) {
        if (isNumber(token)) {
            _stack.push(std::stoi(token));
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (_stack.size() < 2)
                throw std::invalid_argument("Error: Not enough operands.");

            int b = _stack.top(); _stack.pop();
            int a = _stack.top(); _stack.pop();
            int result;

            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else {
                if (b == 0) throw std::invalid_argument("Error: Division by zero.");
                result = a / b;
            }
            _stack.push(result);
        } else {
            throw std::invalid_argument("Error: Invalid token.");
        }
    }

    if (_stack.size() != 1)
        throw std::invalid_argument("Error: Invalid expression.");

    return _stack.top();
}
