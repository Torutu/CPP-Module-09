#pragma once

template <typename T>
void insertIntoSorted(T& sorted, typename T::value_type elem) {
    auto it = std::upper_bound(sorted.begin(), sorted.end(), elem);
    sorted.insert(it, elem);
}

template <typename T>
void splitPairs(const T& elements, T& smaller, T& larger) {
    for (size_t i = 0; i + 1 < elements.size(); i += 2) {
        if (elements[i] < elements[i + 1]) {
            smaller.push_back(elements[i]);
            larger.push_back(elements[i + 1]);
        } else {
            smaller.push_back(elements[i + 1]);
            larger.push_back(elements[i]);
        }
    }

    if (elements.size() % 2 != 0) {
        smaller.push_back(elements.back());
    }
}

// Helper: Insert all smaller elements into sorted larger elements
template <typename T>
void insertSmallerIntoLarger(T& larger, const T& smaller) {
    for (size_t i = 0; i < smaller.size(); ++i) {
        insertIntoSorted(larger, smaller[i]);
    }
}

// Ford-Johnson merge-insertion sort
template <typename T>
void fordJohnsonSort(T& elements) {
    if (elements.size() <= 1) return;

    T smaller;
    T larger;

    splitPairs(elements, smaller, larger);
    //recursively sort the larger elements
    fordJohnsonSort(larger);
    insertSmallerIntoLarger(larger, smaller);
    // Step 4: copy back into original container
    elements = std::move(larger);
}
