#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <deque>

class PmergeMe {
private:

    template <typename T>
    void insertionSort(T& container, int left, int right);

    template <typename T>
    void merge(T& container, int left, int mid, int right);

    template <typename T>
    void mergeSort(T& container, int left, int right);

    template <typename T>
    void fordJohnsonSort(T& container);
public:

    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    static void parseInput(int argc, char** argv, std::vector<int>& vec, std::deque<int>& deq);
    static void printSequence(const std::string& msg, const std::vector<int>& vec);

    template <typename Container>
    static double sortAndMeasure(Container& container);
};

#include "PmergeMe.tpp"
#endif
