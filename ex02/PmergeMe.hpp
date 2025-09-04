#ifndef PMERGEME_HPP
#define PMERGEME_HPP

// #define PMERGE_DEBUG

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

template <typename T> bool PmergeMe::_comp(T left, T right) {
	PmergeMe::nbr_of_comps++;
    #ifdef PMERGE_DEBUG
        std::cout << "nbr of comps: " << nbr_of_comps;
        std::cout << " | left " << *left << " < right " << *right << "\n";
    #endif
	return *left < *right;
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
    main.insert(main.end(), std::next(container.begin(), pair_level - 1)); //b1
    main.insert(main.end(), std::next(container.begin(), pair_level * 2 - 1)); //b2

    // Insert the rest of a's into the main chain.
    // Insert the rest of b's into the pend.
    for (int i = 4; i <= pair_units_nbr; i += 2) {
        pend.insert(pend.end(), std::next(container.begin(), pair_level * (i - 1) - 1));// ai
        main.insert(main.end(), std::next(container.begin(), pair_level * i - 1));//bi
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

    for (int j = 2;; j++) {
        int curr_jacobsthal = _jacobsthal_number(j);
        int jacobsthal_diff = curr_jacobsthal - prev_jacobsthal;
        int offset = 0;
        #ifdef PMERGE_DEBUG
            std::cout << "jacob diff = " << jacobsthal_diff << "\n";
        #endif
        if (jacobsthal_diff > static_cast<int>(pend.size())){
        #ifdef PMERGE_DEBUG
            std::cout << "[Jacob] not enough pending elements -> break\n";
        #endif
        break;
        }

        int nbr_of_times = jacobsthal_diff;
        typename std::vector<Iterator>::iterator pend_it =
            std::next(pend.begin(), jacobsthal_diff - 1); // start inserting from the back of the current batch.if pend = [10 , 6] we star with 6
        typename std::vector<Iterator>::iterator bound_it =
            std::next(main.begin(), curr_jacobsthal + inserted_numbers); // where in main is it allowed to be inserted

            #ifdef PMERGE_DEBUG
                    std::cout << "[Jacob] j=" << j
                  << " curr=" << curr_jacobsthal
                  << " prev=" << prev_jacobsthal
                  << " diff=" << jacobsthal_diff
                  << " | pend size=" << pend.size() << "\n";
            #endif

        while (nbr_of_times) {
            typename std::vector<Iterator>::iterator idx =
                std::upper_bound(main.begin(), bound_it, *pend_it, _comp<Iterator>); // Find insertion index.
            typename std::vector<Iterator>::iterator inserted =
                main.insert(idx, *pend_it); // Insert the element from pend into main.

            #ifdef PMERGE_DEBUG
                    std::cout << "     inserting pend=" << **pend_it
                  << " at position " << (inserted - main.begin()) << "\n";
            #endif

            nbr_of_times--;
            pend_it = pend.erase(pend_it);
            std::advance(pend_it, -1);

            offset += (inserted - main.begin()) == curr_jacobsthal + inserted_numbers;
            bound_it = std::next(main.begin(), curr_jacobsthal + inserted_numbers - offset);

            #ifdef PMERGE_DEBUG
                                std::cout << "     main now: ";
            for (auto it = main.begin(); it != main.end(); ++it)
                std::cout << **it << " ";
            std::cout << "\n";
            #endif
        }

        prev_jacobsthal = curr_jacobsthal;
        inserted_numbers += jacobsthal_diff;
        offset = 0;
    }
}

template <typename T>
void PmergeMe::_merge_insertion_sort(T& container, int pair_level)
{
    typedef typename T::iterator Iterator;

    int pair_units_nbr = container.size() / pair_level;
    if (pair_units_nbr < 2) return;
    bool is_odd = pair_units_nbr % 2 == 1;

    Iterator start = container.begin();                           // start of first full pair
    Iterator last  = std::next(container.begin(), pair_level * pair_units_nbr); // past-end of last full pair(s)
    Iterator end   = std::next(last, -(is_odd * pair_level));    // past-end of last EVEN pair block

#ifdef PMERGE_DEBUG
    auto idx = [&](Iterator it){ return std::distance(container.begin(), it); };
    std::cout << "\n[L" << pair_level << "] pair_units=" << pair_units_nbr
              << " is_odd=" << is_odd
              << " | range: start=" << idx(start)
              << " last="  << idx(last)
              << " end="   << idx(end)  << "\n";

    std::cout << "[L" << pair_level << "] before swap: ";
    for (auto v : container) std::cout << v << " ";
    std::cout << "\n";
#endif

    _swap_pairs_in_range(start, end, pair_level);

#ifdef PMERGE_DEBUG
    std::cout << "[L" << pair_level << "] after  swap:  ";
    { size_t shown=0; for (auto v: container){
        if (shown++==32){
            std::cout<<"..."; break;} 
            std::cout<<v<<" ";
        }
    }
    std::cout << "\n";
#endif

    // Recurse to build bigger sorted blocks
    _merge_insertion_sort(container, pair_level * 2);

    std::vector<Iterator> main, pend;
    _build_main_and_pend(container, pair_units_nbr, pair_level, is_odd, end, main, pend);

#ifdef PMERGE_DEBUG
    std::cout << "[L" << pair_level << "] container after build: ";
    for (auto v : container) std::cout << v << " ";
    std::cout << "\n";
    std::cout << "[L" << pair_level << "] main vals: ";
    for (auto it : main) std::cout << *it << " ";
    std::cout << "\n";
    std::cout << "[L" << pair_level << "] pend vals: ";
    for (auto it : pend) std::cout << *it << " ";
    std::cout << "\n";
#endif

    _insert_pend_jacobsthal(main, pend);

#ifdef PMERGE_DEBUG
    std::cout << "[L" << pair_level << "] after jacobsthal: main size=" << main.size()
              << " pend size=" << pend.size() << "\n";
#endif

    // Insert remaining pend in reverse
    for (std::ptrdiff_t i = static_cast<std::ptrdiff_t>(pend.size()) - 1; i >= 0; --i)
    {
        typename std::vector<Iterator>::iterator curr_pend  = std::next(pend.begin(), i);
        typename std::vector<Iterator>::iterator curr_bound =
            std::next(main.begin(), main.size() - pend.size() + i + is_odd);
        typename std::vector<Iterator>::iterator idx_it =
            std::upper_bound(main.begin(), curr_bound, *curr_pend, _comp<Iterator>);
        main.insert(idx_it, *curr_pend);
    }

#ifdef PMERGE_DEBUG
    std::cout << "[L" << pair_level << "] main vals (final): ";
    for (auto it : main) std::cout << *it << " ";
    std::cout << "\n";
#endif

    std::vector<int> copy;
    copy.reserve(container.size());
    for (typename std::vector<Iterator>::iterator it = main.begin(); it != main.end(); ++it)
    {
        for (int i = 0; i < pair_level; ++i)
        {
            Iterator pair_start = *it;
            std::advance(pair_start, -pair_level + i + 1);
            copy.push_back(*pair_start);
        }
    }

    std::copy(copy.begin(), copy.end(), container.begin());

#ifdef PMERGE_DEBUG
std::cout << "[L" << pair_level << "] wrote back: ";

size_t shown = 0;
for (auto v : container) {
    if (shown++ == 32) {
        std::cout << "...";
        break;
    }
    std::cout << v << " ";
}

std::cout << "\n";
#endif

}

#endif