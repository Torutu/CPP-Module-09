#include "PmergeMe.hpp"

int main(int argc, char** argv) {
    std::vector<int> vec;
    std::deque<int> deq;

    try {
        PmergeMe::parseInput(argc, argv, vec, deq);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    PmergeMe::printSequence("Before: ", vec);
    double vec_time = PmergeMe::sortAndMeasure(vec);
    PmergeMe::printSequence("After: ", vec);
    std::cout << "Time to process " << vec.size() << " elements with std::vector: "
              << vec_time << " us" << std::endl;

    double deq_time = PmergeMe::sortAndMeasure(deq);
    std::cout << "Time to process " << deq.size() << " elements with std::deque: "
              << deq_time << " us" << std::endl;

    return 0;
}
