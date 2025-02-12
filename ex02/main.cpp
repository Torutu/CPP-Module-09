#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <deque>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error: Invalid number of arguments." << std::endl;
        return 1;
    }

    std::vector<int> vec;
    std::deque<int> deq;

    // Parse the input arguments
    for (int i = 1; i < argc; ++i) {
        try {
            int num = std::stoi(argv[i]);
            if (num <= 0) {
                throw std::invalid_argument("Error: All numbers must be positive.");
            }
            vec.push_back(num);
            deq.push_back(num);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    // Display the original sequence
    std::cout << "Before: ";
    for (int n : vec) std::cout << n << " ";
    std::cout << std::endl;

    // Sort and measure time for vector
    auto start = std::chrono::high_resolution_clock::now();
    fordJohnsonSort(vec);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> vec_duration = end - start;

    // Display sorted vector
    std::cout << "After: ";
    for (int n : vec) std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector: "
              << vec_duration.count() * 1e6 << " us" << std::endl;

    // Sort and measure time for deque
    start = std::chrono::high_resolution_clock::now();
    fordJohnsonSort(deq);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> deq_duration = end - start;

    std::cout << "Time to process a range of " << deq.size() << " elements with std::deque: "
              << deq_duration.count() * 1e6 << " us" << std::endl;

    return 0;
}
