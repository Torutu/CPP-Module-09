#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <vector>
#include <set>

#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>


class PmergeMe
{
  public:
    PmergeMe();
    PmergeMe(const PmergeMe& pm);
    PmergeMe& operator=(const PmergeMe& pm);
    ~PmergeMe();

    void sort_vec(std::vector<int>& vec);
    void sort_deque(std::deque<int>& deque);

	static int nbr_of_comps;

  private:
    template <typename T> void _merge_insertion_sort(T& container, int pair_level);

    template <typename T> void _swap_pair(T it, int pair_level);

    template <typename T> static bool _comp(T lv, T rv);

    template <typename Iterator> void _swap_pairs_in_range(Iterator start, Iterator end, int pair_level);

    template <typename T> void _build_main_and_pend(
        T& container,
        int pair_units_nbr,
        int pair_level,
        bool is_odd,
        typename T::iterator end,
        std::vector<typename T::iterator>& main,
        std::vector<typename T::iterator>& pend);
    
    template <typename Iterator> void _insert_pend_jacobsthal(
        std::vector<Iterator>& main,
        std::vector<Iterator>& pend);
};

//----------------------------------------------------------------

long _jacobsthal_number(long n);

//----------------------------------------------------------------

template <typename T> bool PmergeMe::_comp(T lv, T rv) {
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

template <typename Iterator>
void PmergeMe::_swap_pairs_in_range(Iterator start, Iterator end, int pair_level) {
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
void PmergeMe::_build_main_and_pend(
    T& container,
    int pair_units_nbr,
    int pair_level,
    bool is_odd,
    typename T::iterator end,
    std::vector<typename T::iterator>& main,
    std::vector<typename T::iterator>& pend)
{
    // Initialize the main chain with the {b1, a1}.
    main.insert(main.end(), std::next(container.begin(), pair_level - 1));
    main.insert(main.end(), std::next(container.begin(), pair_level * 2 - 1));

    // Insert the rest of a's into the main chain.
    // Insert the rest of b's into the pend.
    for (int i = 4; i <= pair_units_nbr; i += 2) {
        pend.insert(pend.end(), std::next(container.begin(), pair_level * (i - 1) - 1));
        main.insert(main.end(), std::next(container.begin(), pair_level * i - 1));
    }

    // Insert an odd element to the pend, if there are any.
    if (is_odd) {
        pend.insert(pend.end(), std::next(end, pair_level - 1));
    }
}

template <typename Iterator>
void PmergeMe::_insert_pend_jacobsthal(
    std::vector<Iterator>& main,
    std::vector<Iterator>& pend)
{
    int prev_jacobsthal = _jacobsthal_number(1);
    int inserted_numbers = 0;

    for (int k = 2;; k++) {
        int curr_jacobsthal = _jacobsthal_number(k);
        int jacobsthal_diff = curr_jacobsthal - prev_jacobsthal;
        int offset = 0;

        if (jacobsthal_diff > static_cast<int>(pend.size()))
            break;

        int nbr_of_times = jacobsthal_diff;
        typename std::vector<Iterator>::iterator pend_it =
            std::next(pend.begin(), jacobsthal_diff - 1);
        typename std::vector<Iterator>::iterator bound_it =
            std::next(main.begin(), curr_jacobsthal + inserted_numbers);

        while (nbr_of_times) {
            typename std::vector<Iterator>::iterator idx =
                std::upper_bound(main.begin(), bound_it, *pend_it, _comp<Iterator>);
            typename std::vector<Iterator>::iterator inserted =
                main.insert(idx, *pend_it);

            nbr_of_times--;
            pend_it = pend.erase(pend_it);
            std::advance(pend_it, -1);

            // Handle eclipsed bound
            offset += (inserted - main.begin()) == curr_jacobsthal + inserted_numbers;
            bound_it = std::next(main.begin(), curr_jacobsthal + inserted_numbers - offset);
        }

        prev_jacobsthal = curr_jacobsthal;
        inserted_numbers += jacobsthal_diff;
        offset = 0;
    }
}

template <typename T> void PmergeMe::_merge_insertion_sort(T& container, int pair_level)
{
    typedef typename T::iterator Iterator;

    int pair_units_nbr = container.size() / pair_level;
    if (pair_units_nbr < 2)
        return;
    bool is_odd = pair_units_nbr % 2 == 1;

    Iterator start = container.begin();
    Iterator last = next(container.begin(), pair_level * (pair_units_nbr));
    Iterator end = next(last, -(is_odd * pair_level));

    _swap_pairs_in_range(start, end, pair_level);
    _merge_insertion_sort(container, pair_level * 2);

    std::vector<Iterator> main;
    std::vector<Iterator> pend;

    /* Initialize the main chain with the {b1, a1}. */
    _build_main_and_pend(container, pair_units_nbr, pair_level, is_odd, end, main, pend);
    _insert_pend_jacobsthal(main, pend);

    /* Insert the remaining pend elements (if any) in reverse order. */
    for (ssize_t i = pend.size() - 1; i >= 0; i--)
    {
        typename std::vector<Iterator>::iterator curr_pend = next(pend.begin(), i);
        typename std::vector<Iterator>::iterator curr_bound =
            next(main.begin(), main.size() - pend.size() + i + is_odd);
        typename std::vector<Iterator>::iterator idx =
            std::upper_bound(main.begin(), curr_bound, *curr_pend, _comp<Iterator>);
        main.insert(idx, *curr_pend);
    }

    /* Create a copy of the sorted values. */
    std::vector<int> copy;
    copy.reserve(container.size());
    for (typename std::vector<Iterator>::iterator it = main.begin(); it != main.end(); it++)
    {
        for (int i = 0; i < pair_level; i++)
        {
            Iterator pair_start = *it;
            std::advance(pair_start, -pair_level + i + 1);
            copy.insert(copy.end(), *pair_start);
        }
    }

    /* Replace values in the original container. */
    Iterator container_it = container.begin();
    std::vector<int>::iterator copy_it = copy.begin();
    while (copy_it != copy.end())
    {
        *container_it = *copy_it;
        container_it++;
        copy_it++;
    }
}
#endif