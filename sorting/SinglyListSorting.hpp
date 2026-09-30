
#ifndef LINKEDLISTSORTING_HPP
#define LINKEDLISTSORTING_HPP

#include <chrono>
#include "SortingCommon.hpp"
#include "../SinglyLinkedList/PatientList.hpp"

namespace PatientSorting {
// Traverse actual nodes; swap whole patient records, leaving links intact.
// This keeps the existing list's private head and tail valid.
inline SortStats bubbleSortList(PatientList& patients, int field, bool ascending) {
    SortStats stats;
    const auto start = std::chrono::steady_clock::now();
    PatientNode* end = nullptr;
    if (patients.getHead() != nullptr) {
        bool changed;
        do {
            changed = false;
            PatientNode* current = patients.getHead();
            while (current->next != end) {
                ++stats.comparisons;
                if (outOfOrder(current->data, current->next->data, field, ascending)) {
                    Patient temporary = current->data;
                    current->data = current->next->data;
                    current->next->data = temporary;
                    ++stats.swaps;
                    stats.writes += 2;
                    changed = true;
                }
                current = current->next;
            }
            end = current;
        } while (changed);
    }
    stats.microseconds = std::chrono::duration<double, std::micro>(
        std::chrono::steady_clock::now() - start).count();
    return stats;
}

// Insertion sort over nodes, shifting whole records without changing links.
// The last sorted record gives a fast path for already ordered input.
inline SortStats insertionSortList(PatientList& patients, int field, bool ascending) {
    SortStats stats;
    const auto start = std::chrono::steady_clock::now();
    PatientNode* previous = patients.getHead();
    if (previous) {
        for (PatientNode* current = previous->next; current; current = current->next) {
            Patient value = current->data;
            ++stats.comparisons;
            if (outOfOrder(previous->data, value, field, ascending)) {
                PatientNode* position = patients.getHead();
                while (position != current) {
                    ++stats.comparisons;
                    if (outOfOrder(position->data, value, field, ascending)) break;
                    position = position->next;
                }
                // Carry displaced records forward up to the insertion slot.
                Patient carry = value;
                while (position != current) {
                    Patient displaced = position->data;
                    position->data = carry;
                    ++stats.writes;
                    carry = displaced;
                    position = position->next;
                }
                current->data = carry;
                ++stats.writes;
            }
            previous = current;
        }
    }
    stats.microseconds = std::chrono::duration<double, std::micro>(
        std::chrono::steady_clock::now() - start).count();
    return stats;
}

} // namespace PatientSorting
#endif