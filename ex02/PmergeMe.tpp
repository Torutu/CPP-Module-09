#pragma once

// Insertion Sort
template <typename T>
void PmergeMe::insertionSort(T& container, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        auto key = container[i];
        int j = i - 1;
        while (j >= left && container[j] > key) {
            container[j + 1] = container[j];
            --j;
        }
        container[j + 1] = key;
    }
}

// Merge
template <typename T>
void PmergeMe::merge(T& container, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    T leftPart(container.begin() + left, container.begin() + mid + 1);
    T rightPart(container.begin() + mid + 1, container.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftPart[i] <= rightPart[j]) {
            container[k++] = leftPart[i++];
        } else {
            container[k++] = rightPart[j++];
        }
    }
    while (i < n1) container[k++] = leftPart[i++];
    while (j < n2) container[k++] = rightPart[j++];
}

// Merge Sort
template <typename T>
void PmergeMe::mergeSort(T& container, int left, int right) {
    if (right - left <= 10) {  // threshold for insertion sort
        insertionSort(container, left, right);
    } else {
        int mid = left + (right - left) / 2;
        mergeSort(container, left, mid);
        mergeSort(container, mid + 1, right);
        merge(container, left, mid, right);
    }
}

// Ford-Johnson Sort
template <typename T>
void PmergeMe::fordJohnsonSort(T& container) {
    if (!container.empty())
        mergeSort(container, 0, container.size() - 1);
}

template <typename Container>
double PmergeMe::sortAndMeasure(Container& container) {
    auto start = std::chrono::high_resolution_clock::now();
    fordJohnsonSort(container);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1e6; // microseconds
}