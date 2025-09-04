#ifndef RPN_HPP
#define RPN_HPP

// #define RPN_DEBUG

#include <stack>

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>

class RPN {
private:
    std::string _expression;
    std::stack<double> _stack;

    bool isNumber(const std::string& token) const;

public:

    RPN();                              
    RPN(const RPN& other);              
    RPN& operator=(const RPN& other);   
    ~RPN();                            

    void setExpression(const std::string& expr);
    std::string getExpression() const;

    double calculate();
};

#endif
