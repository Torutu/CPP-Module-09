#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>
#include <deque>
#include <vector>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <set>
#include <cmath>
#include <deque>

class PmergeMe
{
   private:
    template <typename T>
    void _merge_insertion_sort(T& container, int pair_level);
    
    template <typename T>
    void _swap_pair(T it, int pair_level);

    template <typename T>
    void swap_pairs_by_max(T& container, int pair_level);

   public:
    PmergeMe();
    PmergeMe(const PmergeMe& pm);
    PmergeMe& operator=(const PmergeMe& pm);
    ~PmergeMe();

    void sort_vec(std::vector<int>& vec);
    void sort_deque(std::deque<int>& deque);

	static int nbr_of_comps;

};

long _jacobsthal_number(long n);

template <typename T> bool _comp(T lv, T rv) {
	PmergeMe::nbr_of_comps++;
	return *lv < *rv;
}

template <typename T> T next(T it, int steps)
{
    std::advance(it, steps);
    return it;
}

template <typename T> void PmergeMe::_swap_pair(T it, int pair_level)
{
    T start = next(it, -pair_level + 1);
    T end = next(start, pair_level);
    while (start != end)
    {
        std::iter_swap(start, next(start, pair_level));
        start++;
    }
}

template <typename T>
void PmergeMe::swap_pairs_by_max(T& container, int pair_level) {
    typedef typename T::iterator Iterator;
    int pair_units_nbr = container.size() / pair_level;
    // Skip last if odd pair
    bool is_odd = pair_units_nbr % 2 == 1; 
    // Define iteration range (ignore leftover elements that can't form a pair)
    Iterator start = container.begin();
    Iterator last = std::next(container.begin(), pair_level * pair_units_nbr);
    Iterator end  = std::next(last, -(is_odd * pair_level));
    // Compare adjacent pairs by their largest element and swap if needed
    int jump = 2 * pair_level;
    for (Iterator it = start; it != end; std::advance(it, jump)) {
        Iterator this_pair = std::next(it, pair_level - 1);
        Iterator next_pair = std::next(it, pair_level * 2 - 1);
        if (_comp(next_pair, this_pair)) {
            _swap_pair(this_pair, pair_level);
        }
    }
}

template <typename T>
void build_main_and_pend(T& container, int pair_level,
                         std::vector<typename T::iterator>& main,
                         std::vector<typename T::iterator>& pend,
                         bool is_odd, int pair_units_nbr) 
{
    // Take the first two pairs and put them in the main chain
    main.push_back(std::next(container.begin(), pair_level - 1));
    main.push_back(std::next(container.begin(), pair_level * 2 - 1));

    // For the rest of the pairs: 
    //   - put the "a" element into pend
    //   - put the "b" element into main
    for (int i = 4; i <= pair_units_nbr; i += 2) {
        pend.push_back(std::next(container.begin(), pair_level * (i - 1) - 1));
        main.push_back(std::next(container.begin(), pair_level * i - 1));
    }

    // If the number of pairs is odd, 
    // put the leftover element into pend
    if (is_odd) {
        pend.push_back(std::next(container.begin(), pair_level * (pair_units_nbr - 1)));
    }
}

template <typename Iterator>
void insert_pend_jacobsthal(std::vector<Iterator>& main, std::vector<Iterator>& pend) {
    int prev_jacob = _jacobsthal_number(1);  // start with first Jacobsthal number
    int inserted = 0;                        // track how many elements have been inserted

    for (int k = 2;; k++) {
        int curr_jacob = _jacobsthal_number(k);       // next Jacobsthal number
        int diff = curr_jacob - prev_jacob;          // how many pend elements to insert
        if (diff > (int)pend.size())                 // stop if there aren’t enough elements
            break;

        auto pend_it = std::next(pend.begin(), diff - 1);         // point to last element in this group
        auto bound_it = std::next(main.begin(), curr_jacob + inserted); // insertion limit in main

        // Insert pend elements into main in order
        for (int i = 0; i < diff; i++) {
            auto idx = std::upper_bound(main.begin(), bound_it, *pend_it, _comp<Iterator>);
            main.insert(idx, *pend_it);      // insert at correct position
            pend_it = pend.erase(pend_it);    // remove from pend
            std::advance(pend_it, -1);       // adjust iterator after erase
        }
        prev_jacob = curr_jacob;              // update previous Jacobsthal
        inserted += diff;                      // update number of inserted elements
    }
}


template <typename Iterator>
void insert_remaining_pend(std::vector<Iterator>& main, std::vector<Iterator>& pend, bool is_odd) {
    // Insert remaining elements from pend into main, starting from the last element
    for (ssize_t i = pend.size() - 1; i >= 0; i--) {
        auto curr_pend = std::next(pend.begin(), i);                        // current element to insert
        auto curr_bound = std::next(main.begin(), main.size() - pend.size() + i + is_odd); // upper bound for insertion
        auto idx = std::upper_bound(main.begin(), curr_bound, *curr_pend, _comp<Iterator>); // find correct position
        main.insert(idx, *curr_pend);                                      // insert element into main
    }
}


template <typename T>
void flatten_into_container(T& container, std::vector<typename T::iterator>& main, int pair_level) {
    std::vector<int> copy;
    copy.reserve(container.size());  // prepare a temporary vector to store sorted elements

    // Go through each iterator in main and add its corresponding pair elements to copy
    for (auto it : main) {
        for (int i = 0; i < pair_level; i++) {
            auto pair_start = it;
            std::advance(pair_start, -pair_level + i + 1);  // move to the start of the pair
            copy.push_back(*pair_start);                    // store the element
        }
    }

    std::copy(copy.begin(), copy.end(), container.begin()); // overwrite original container with sorted elements
}

template <typename T>
void PmergeMe::_merge_insertion_sort(T& container, int pair_level) {
    int pair_units_nbr = container.size() / pair_level;
    if (pair_units_nbr < 2)
        return;
    bool is_odd = pair_units_nbr % 2 == 1;
    swap_pairs_by_max(container, pair_level);
    _merge_insertion_sort(container, pair_level * 2);

    std::vector<typename T::iterator> main, pend;
    build_main_and_pend(container, pair_level, main, pend, is_odd, pair_units_nbr);
    insert_pend_jacobsthal(main, pend);

    insert_remaining_pend(main, pend, is_odd);
    flatten_into_container(container, main, pair_level);
}
#endif