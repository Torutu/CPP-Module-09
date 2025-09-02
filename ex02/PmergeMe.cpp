#include "PmergeMe.hpp"
#include "PmergeMe.tpp"

// ---------------- constructor and destructor ----------------
PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) {
    _vec = other._vec;
    _deq = other._deq;
}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}
PmergeMe::~PmergeMe() {}

// ---------------- Static functions ----------------
static bool isSingleNumber(const std::string& s) {
    if (s.empty()) return false;
    size_t start = 0;
    if (s[0] == '+' || s[0] == '-') start = 1;
    for (size_t i = start; i < s.size(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}

// ---------------- Member functions ----------------
void PmergeMe::parseInput(int argc, char** argv, std::vector<int>& vec, std::deque<int>& deq) {
    if (argc < 2)
        throw std::invalid_argument("Error: Invalid number of arguments.");

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (!isSingleNumber(arg))
            throw std::invalid_argument("Error: Invalid argument (not a single number).");

        int num = std::stoi(arg);
        if (num <= 0)
            throw std::invalid_argument("Error: All numbers must be positive.");

        vec.push_back(num);
        deq.push_back(num);
    }
}

void PmergeMe::printSequence(const std::string& msg, const std::vector<int>& vec) {
    std::cout << msg;
    for (int n : vec) std::cout << n << " ";
    std::cout << std::endl;
}
