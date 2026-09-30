
#ifndef ARRAYSORTING_HPP
#define ARRAYSORTING_HPP
#include <chrono>
#include "SortingCommon.hpp"
#include "../Array/PatientArray.hpp"
namespace PatientSorting {
inline SortStats insertionSortArray(PatientArray& patients, int field, bool ascending) {
    SortStats stats;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 1; i < patients.getSize(); ++i) {
        Patient current = patients.get(i);
        int j = i - 1;
        while (j >= 0) {
            ++stats.comparisons;
            Patient previous = patients.get(j);
            if (!outOfOrder(previous, current, field, ascending)) break;
            patients.set(j + 1, previous);
            ++stats.writes;
            --j;
        }
        patients.set(j + 1, current);
        ++stats.writes;
    }
    stats.microseconds = std::chrono::duration<double, std::micro>(
        std::chrono::steady_clock::now() - start).count();
    return stats;
}
// Bubble sort compares adjacent records. Strict comparison preserves ties.
inline SortStats bubbleSortArray(PatientArray& patients, int field, bool ascending) {
    SortStats stats;
    const auto start = std::chrono::steady_clock::now();
    for (int end = patients.getSize() - 1; end > 0; --end) {
        bool changed = false;
        for (int i = 0; i < end; ++i) {
            Patient a = patients.get(i);
            Patient b = patients.get(i + 1);
            ++stats.comparisons;
            if (outOfOrder(a, b, field, ascending)) {
                patients.set(i, b);
                patients.set(i + 1, a);
                ++stats.swaps;
                stats.writes += 2;
                changed = true;
            }
        }
        if (!changed) break;
    }
    stats.microseconds = std::chrono::duration<double, std::micro>(
        std::chrono::steady_clock::now() - start).count();
    return stats;
}

}
#endif

