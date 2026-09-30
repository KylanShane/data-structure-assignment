
#ifndef SORTINGMENU_HPP
#define SORTINGMENU_HPP
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include "ArraySorting.hpp"
#include "SinglyListSorting.hpp"
namespace PatientSorting {
// Clear the console before displaying the next screen.
inline void clearSortingScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

inline void printMenuTitle(const char* title) {
    std::cout << "\n========================================\n"
              << "  " << title << "\n"
              << "========================================\n\n";
}

inline void printHeader() {
    std::cout << std::left << std::setw(16) << "Patient ID"
              << std::setw(8) << "Age" << std::setw(22) << "Care type"
              << std::setw(16) << "Duration (hr)" << "Total cost (MYR)\n";
    std::cout << std::string(80, '-') << '\n';
}
inline void printPatient(const Patient& p) {
    std::cout << std::left << std::setw(16) << p.patientID
              << std::setw(8) << p.age << std::setw(22) << p.careType
              << std::setw(16) << p.lengthOfStay
              << std::fixed << std::setprecision(2) << p.totalCost << '\n';
}
inline int readChoice(int maximum) {
    int choice;
    while (true) {
        std::cout << "Enter choice: ";
        if (std::cin >> choice && choice >= 1 && choice <= maximum) return choice;
        if (std::cin.eof()) return maximum;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Enter 1-" << maximum << ".\n";
    }
}

inline void pauseResults() {
    std::cout << "\n========================================\n"
              << "Press ENTER to return to the Facility Menu...\n"
              << "========================================" << std::flush;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
inline int choose(const char* title, const char* options, int maximum) {
    clearSortingScreen();
    printMenuTitle(title);
    std::cout << options;
    return readChoice(maximum);
}
inline const char* algorithmName(int algorithm) {
    return algorithm == 1 ? "Bubble sort" : "Insertion sort";
}
inline const char* fieldName(int field) {
    return field == 1 ? "Age" : field == 2 ? "Visit duration" : "Total cost";
}
inline SortStats sortArray(PatientArray& a, int algorithm, int field, bool asc) {
    return algorithm == 1 ? bubbleSortArray(a, field, asc) : insertionSortArray(a, field, asc);
}
inline SortStats sortList(PatientList& l, int algorithm, int field, bool asc) {
    return algorithm == 1 ? bubbleSortList(l, field, asc) : insertionSortList(l, field, asc);
}
inline void copyArray(const PatientArray& source, PatientArray& target) {
    for (int i = 0; i < source.getSize(); ++i) target.add(source.get(i));
}
inline void copyList(const PatientList& source, PatientList& target) {
    for (PatientNode* n = source.getHead(); n; n = n->next) target.insertAtEnd(n->data);
}
inline void performanceHeader() {
    std::cout << std::left << std::setw(14) << "Structure" << std::setw(10) << "Records"
              << std::setw(15) << "Comparisons" << std::setw(15) << "Record writes"
              << std::setw(16) << "Time (us)" << "Est. bytes\n"
              << std::string(86, '-') << '\n';
}
inline void performanceRow(const char* name, int n, const SortStats& stats, std::size_t bytes) {
    std::cout << std::left << std::setw(14) << name << std::setw(10) << n
              << std::setw(15) << stats.comparisons << std::setw(15) << stats.writes
              << std::setw(16) << std::fixed << std::setprecision(3) << stats.microseconds
              << bytes << '\n';
}
inline void complexityAndNotes() {
    std::cout << "\n" << std::left << std::setw(14) << "Structure" << std::setw(14) << "Best time"
              << std::setw(14) << "Average time" << std::setw(14) << "Worst time" << "Aux. space\n"
              << std::string(70, '-') << '\n';
    const char* names[2] = {"Array", "Singly list"};
    for (int i = 0; i < 2; ++i)
        std::cout << std::setw(14) << names[i] << std::setw(14) << "O(n)"
                  << std::setw(14) << "O(n^2)" << std::setw(14) << "O(n^2)" << "O(1)\n";
    // Memory values estimate occupied records only. They exclude unused array
    // capacity, string heap allocations and allocator overhead. Sorting timing
    // includes counters, excludes copying/output and varies between runs.
    // Auxiliary space excludes input and experiment copies; records are assumed
    // bounded in size. Writes exclude assignments to local temporary records.

}
inline void sortingFlow(const PatientArray* const arrays[], const PatientList* const lists[]) {
    while (true) {
        int structure = choose("VIEW SORTING", "1. Array\n2. Singly Linked List\n3. Back\n", 3);
        if (structure == 3) return;
        while (true) {
            int facility = choose("SELECT FACILITY", "1. Facility A\n2. Facility B\n3. Facility C\n4. Back\n", 4);
            if (facility == 4) break;
            int algorithm = choose("SELECT ALGORITHM", "1. Bubble Sort\n2. Insertion Sort\n3. Back\n", 3);
            if (algorithm == 3) continue;
            int field = choose("SORT BY", "1. Age\n2. Visit duration\n3. Total medical cost\n4. Back\n", 4);
            if (field == 4) continue;
            int order = choose("SORT ORDER", "1. Ascending\n2. Descending\n3. Back\n", 3);
            if (order == 3) continue;
            clearSortingScreen();
            printMenuTitle("SORTED PATIENT RECORDS");
            std::cout << "Facility " << char('A' + facility - 1) << " | " << algorithmName(algorithm)
                      << " | " << fieldName(field) << " | " << (order == 1 ? "Ascending" : "Descending") << "\n\n";
            if (structure == 1) {
                PatientArray copy; copyArray(*arrays[facility - 1], copy);
                if (!copy.getSize()) std::cout << "No data found.\n";
                else {
                    sortArray(copy, algorithm, field, order == 1);
                    printHeader();
                    for (int i = 0; i < copy.getSize(); ++i) printPatient(copy.get(i));
                }
            } else {
                PatientList copy; copyList(*lists[facility - 1], copy);
                if (!copy.getSize()) std::cout << "No data found.\n";
                else {
                    sortList(copy, algorithm, field, order == 1);
                    printHeader();
                    for (PatientNode* n = copy.getHead(); n; n = n->next) printPatient(n->data);
                }
            }
            pauseResults();
        }
    }
}
inline void comparisonFlow(const PatientArray* const arrays[], const PatientList* const lists[]) {
    while (true) {
        int facility = choose("COMPARISON - SELECT FACILITY", "1. Facility A\n2. Facility B\n3. Facility C\n4. Back\n", 4);
        if (facility == 4) return;
        int algorithm = choose("COMPARE ALGORITHM", "1. Bubble Sort\n2. Insertion Sort\n3. Back\n", 3);
        if (algorithm == 3) continue;
        clearSortingScreen(); printMenuTitle("SORTING PERFORMANCE COMPARISON");
        std::cout << "Facility " << char('A' + facility - 1) << " | " << algorithmName(algorithm) << '\n';
        if (!arrays[facility - 1]->getSize() || !lists[facility - 1]->getSize()) {
            std::cout << "No data in one or both structures. Check CSV loading.\n";
            pauseResults(); continue;
        }
        for (int field = 1; field <= 3; ++field) {
            for (int order = 1; order <= 2; ++order) {
                PatientArray a; PatientList l;
                copyArray(*arrays[facility - 1], a); copyList(*lists[facility - 1], l);
                SortStats sa = sortArray(a, algorithm, field, order == 1);
                SortStats sl = sortList(l, algorithm, field, order == 1);
                std::cout << "\n" << fieldName(field) << " - " << (order == 1 ? "Ascending" : "Descending") << '\n';
                performanceHeader();
                performanceRow("Array", a.getSize(), sa, static_cast<std::size_t>(a.getSize()) * sizeof(Patient));
                performanceRow("Singly list", l.getSize(), sl, static_cast<std::size_t>(l.getSize()) * sizeof(PatientNode));
            }
        }
        complexityAndNotes(); pauseResults();
    }
}
inline void menu(const PatientArray& A, const PatientArray& B, const PatientArray& C,
                 const PatientList& LA, const PatientList& LB, const PatientList& LC) {
    const PatientArray* arrays[3] = {&A, &B, &C};
    const PatientList* lists[3] = {&LA, &LB, &LC};
    while (true) {
        int action = choose("SORTING EXPERIMENTS", "1. View sorting\n2. View comparison\n3. Back\n", 3);
        if (action == 3) return;
        if (action == 1) sortingFlow(arrays, lists);
        else comparisonFlow(arrays, lists);
    }
}
}
#endif

