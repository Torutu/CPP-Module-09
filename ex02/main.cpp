#include "PmergeMe.hpp"

int main(int argc, char** argv) {
    PmergeMe pm;
    std::vector<int> vec;
    std::deque<int> deq;

    try {
        pm.parseInput(argc, argv, vec, deq);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    pm.printSequence("Before: ", vec);
    double vec_time = pm.sortAndMeasure(vec);
    pm.printSequence("After: ", vec);
    std::cout << "Time to process " << vec.size() << " elements with std::vector: "
              << vec_time << " us" << std::endl;

    double deq_time = pm.sortAndMeasure(deq);
    std::cout << "Time to process " << deq.size() << " elements with std::deque: "
              << deq_time << " us" << std::endl;

    return 0;
}