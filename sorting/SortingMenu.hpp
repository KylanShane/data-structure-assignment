#ifndef SORTINGMENU_HPP
#define SORTINGMENU_HPP
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
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

// Reusable console-table borders and cells (no built-in containers).
inline void tableBorder(const int widths[], int count, int style = 1) {
    // 0 = top edge, 1 = row separator, 2 = bottom edge.
    std::cout << (style == 0 ? "\u250c" : style == 2 ? "\u2514" : "\u251c");
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < widths[i] + 2; ++j) std::cout << "\u2500";
        if (i == count - 1)
            std::cout << (style == 0 ? "\u2510" : style == 2 ? "\u2518" : "\u2524");
        else
            std::cout << (style == 0 ? "\u252c" : style == 2 ? "\u2534" : "\u253c");
    }
    std::cout << '\n';
}
template <typename T>
inline void tableCell(const T& value, int width) {
    std::cout << " " << std::left << std::setw(width) << value << " \u2502";
}
inline void patientBorder(int style = 1) {
    const int widths[5] = {12, 5, 24, 13, 17};
    tableBorder(widths, 5, style);
}
inline void printHeader() {
    patientBorder(0);
    std::cout << "\u2502";
    tableCell("Patient ID", 12); tableCell("Age", 5); tableCell("Care type", 24);
    tableCell("Duration (hr)", 13); tableCell("Total cost (MYR)", 17);
    std::cout << '\n'; patientBorder();
}
inline void printPatient(const Patient& p) {
    std::cout << "\u2502" << std::fixed << std::setprecision(2);
    tableCell(p.patientID, 12); tableCell(p.age, 5); tableCell(p.careType, 24);
    tableCell(p.lengthOfStay, 13); tableCell(p.totalCost, 17);
    std::cout << '\n';
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
inline void performanceBorder(int style = 1) {
    const int widths[7] = {12, 14, 7, 11, 13, 12, 10};
    tableBorder(widths, 7, style);
}
inline void performanceHeader() {
    performanceBorder(0); std::cout << "\u2502";
    tableCell("Structure", 12); tableCell("Algorithm", 14); tableCell("Records", 7);
    tableCell("Comparisons", 11); tableCell("Record writes", 13);
    tableCell("Time (us)", 12); tableCell("Est. bytes", 10);
    std::cout << '\n'; performanceBorder();
}
inline void performanceRow(const char* name, int algorithm, int n,
                           const SortStats& stats, std::size_t bytes, bool lastRow = false) {
    std::cout << "\u2502" << std::fixed << std::setprecision(3);
    tableCell(name, 12); tableCell(algorithmName(algorithm), 14); tableCell(n, 7);
    tableCell(stats.comparisons, 11); tableCell(stats.writes, 13);
    tableCell(stats.microseconds, 12); tableCell(bytes, 10);
    std::cout << '\n'; performanceBorder(lastRow ? 2 : 1);
}
inline void complexityAndNotes() {
    const int widths[6] = {12, 14, 9, 12, 10, 10};
    std::cout << "\nTIME COMPLEXITY AND AUXILIARY SPACE\n";
    tableBorder(widths, 6, 0); std::cout << "\u2502";
    tableCell("Structure", 12); tableCell("Algorithm", 14); tableCell("Best time", 9);
    tableCell("Average time", 12); tableCell("Worst time", 10); tableCell("Aux. space", 10);
    std::cout << '\n'; tableBorder(widths, 6);
    for (int algorithm = 1; algorithm <= 2; ++algorithm) {
        for (int structure = 1; structure <= 2; ++structure) {
            std::cout << "\u2502"; tableCell(structure == 1 ? "Array" : "Singly list", 12);
            tableCell(algorithmName(algorithm), 14); tableCell("O(n)", 9);
            tableCell("O(n^2)", 12); tableCell("O(n^2)", 10); tableCell("O(1)", 10);
            std::cout << '\n'; tableBorder(widths, 6, algorithm == 2 && structure == 2 ? 2 : 1);
        }
    }
    // Estimated occupied record storage excludes unused capacity, string heap
    // storage and allocator overhead. Times exclude copying/printing. Complexity
    // assumes bounded-size patient records; auxiliary space excludes the copies.
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
                    patientBorder(2);
                }
            } else {
                PatientList copy; copyList(*lists[facility - 1], copy);
                if (!copy.getSize()) std::cout << "No data found.\n";
                else {
                    sortList(copy, algorithm, field, order == 1);
                    printHeader();
                    for (PatientNode* n = copy.getHead(); n; n = n->next) printPatient(n->data);
                    patientBorder(2);
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
        clearSortingScreen(); printMenuTitle("SORTING PERFORMANCE COMPARISON");
        std::cout << "Facility " << char('A' + facility - 1) << " | Both sorting algorithms\n";
        if (!arrays[facility - 1]->getSize() || !lists[facility - 1]->getSize()) {
            std::cout << "No data in one or both structures. Check CSV loading.\n";
            pauseResults(); continue;
        }
        for (int field = 1; field <= 3; ++field) {
            for (int order = 1; order <= 2; ++order) {
                std::cout << "\n" << fieldName(field) << " - "
                          << (order == 1 ? "Ascending" : "Descending") << '\n';
                performanceHeader();
                for (int algorithm = 1; algorithm <= 2; ++algorithm) {
                    // Start each algorithm from the same original record order.
                    PatientArray a; PatientList l;
                    copyArray(*arrays[facility - 1], a);
                    copyList(*lists[facility - 1], l);
                    SortStats sa = sortArray(a, algorithm, field, order == 1);
                    SortStats sl = sortList(l, algorithm, field, order == 1);
                    performanceRow("Array", algorithm, a.getSize(), sa,
                        static_cast<std::size_t>(a.getSize()) * sizeof(Patient));
                    performanceRow("Singly list", algorithm, l.getSize(), sl,
                        static_cast<std::size_t>(l.getSize()) * sizeof(PatientNode), algorithm == 2);
                }
            }
        }
        complexityAndNotes(); pauseResults();
    }
}
inline void menu(const PatientArray& A, const PatientArray& B, const PatientArray& C,
                 const PatientList& LA, const PatientList& LB, const PatientList& LC) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Display solid Unicode table borders on Windows.
#endif
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

