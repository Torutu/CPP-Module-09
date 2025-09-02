#include "PmergeMe.hpp"


static bool validate_arg(const std::string& arg)
{
    if (arg[0] == '-') {
        std::cerr << "Error: Negative numbers\n";
        return false;
    }

    errno = 0; // reset errno before strtol
    long nbr = strtol(arg.c_str(), NULL, 10);

    if ((nbr == 0 && arg != "0") || errno == EINVAL) {
        std::cerr << "Error: Non-number arguments\n";
        return false;
    }

    if (nbr > INT_MAX || errno == ERANGE) {
        std::cerr << "Error: Too big arguments\n";
        return false;
    }

    return true;
}

static bool validate(int argc, char** argv)
{
    if (argc == 1) {
        std::cerr << "Error: No arguments were provided\n";
        return false;
    }

    for (int i = 1; i < argc; i++) {
        if (!validate_arg(argv[i]))
            return false;
    }
    return true;
}

static std::vector<int> argv_to_vector(int argc, char** argv)
{
    std::vector<int> res;
    res.reserve(argc - 1);
    for (int i = 1; i < argc; i++)
    {
        res.push_back(atoi(argv[i]));
    }
    return res;
}

static std::deque<int> argv_to_deque(int argc, char** argv)
{
    std::deque<int> res;
    for (int i = 1; i < argc; i++)
    {
        res.push_back(atoi(argv[i]));
    }
    return res;
}

template <typename T> static bool is_sorted(const T& container)
{
    if (container.size() == 0 || container.size() == 1)
        return true;
    typename T::const_iterator end = container.end();
    std::advance(end, -1);
    for (typename T::const_iterator it = container.begin(); it != end; it++)
    {
        typename T::const_iterator next = it;
        std::advance(next, 1);
        if (*it > *next)
            return false;
    }
    return true;
}

static std::string argv_to_str(int argc, char** argv)
{
    std::string res("");
    res.reserve(argc + 1);
    res += "[";
    res += argv[1];
    for (int i = 2; argv[i]; i++)
    {
        res += " ";
        res += argv[i];
    }
    res += "]";
    return res;
}

static std::string vec_to_str(std::vector<int>& vec)
{
    std::stringstream ss;
    ss << "[" << vec[0];
    for (size_t i = 1; i < vec.size(); i++)
    {
        ss << " " << vec[i];
    }
    ss << "]";
    return ss.str();
}

static bool retained_original_values(std::vector<int> original, std::vector<int>& sorted)
{
    if (original.size() != sorted.size())
        return false;

    std::sort(original.begin(), original.end());
    std::vector<int> copy = sorted;
    std::sort(copy.begin(), copy.end());

    return original == copy;
}


int main(int argc, char** argv)
{
    PmergeMe pm;
	
    if (!validate(argc, argv)){
        return EXIT_FAILURE;
    }
	std::vector<int> original_values = argv_to_vector(argc, argv);

    clock_t start_vec = clock();
    std::vector<int> vec = argv_to_vector(argc, argv);
    pm.sort_vec(vec);
    clock_t end_vec = clock();
    double time_elapsed_vec = static_cast<double>(end_vec - start_vec) / CLOCKS_PER_SEC;

	PmergeMe::nbr_of_comps = 0;
    clock_t start_deque = clock();
    std::deque<int> deque = argv_to_deque(argc, argv);
    pm.sort_deque(deque);
    clock_t end_deque = clock();
    double time_elapsed_deque = static_cast<double>(end_deque - start_deque) / CLOCKS_PER_SEC;

    if (!is_sorted(vec) || (int)deque.size() != (argc - 1) || !retained_original_values(original_values, vec))
	{
        std::cout << "Vector was not sorted properly.\n";
		return 1;
	}
    if (!is_sorted(deque) || (int)deque.size() != (argc - 1))
	{
        std::cout << "Deque was not sorted properly.\n";
		return 1;
	}

    std::cout << "Before: " << argv_to_str(argc, argv) << "\n";
    std::cout << "After: " << vec_to_str(vec) << "\n";
    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::vector: " << std::fixed << std::setprecision(6)
              << time_elapsed_vec << "s\n";
    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::deque:  " << std::fixed << std::setprecision(6)
              << time_elapsed_deque << "s\n";
	std::cout << "Number of comparisons: " << PmergeMe::nbr_of_comps << '\n';
}
