#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <deque>
#include <algorithm>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int> _deq;

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void parseInput(int argc, char** argv, std::vector<int>& vec, std::deque<int>& deq);
    void printSequence(const std::string& msg, const std::vector<int>& vec);

    template <typename Container>
    double sortAndMeasure(Container& container);
};

#include "PmergeMe.tpp"

#endif
