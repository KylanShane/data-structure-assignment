#ifndef SEARCHINGMENU_HPP
#define SEARCHINGMENU_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>
#include <cstdlib>

#include "ArraySearching.hpp"
#include "LinkedListSearching.hpp"

namespace SearchingMenu {

struct Row {
    std::string algorithm;
    std::string state;
    std::string structure;
    int matches = 0;
    long long comparisons = 0;
    double milliseconds = 0;
    bool correct = false;
};

inline void clearSearchingScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

inline void printMenuTitle(
    const std::string& text,
    char symbol = '=') {

    std::cout << '\n'
              << std::string(60, symbol) << '\n'
              << "  " << text << '\n'
              << std::string(60, symbol) << "\n\n";
}

inline int choose(
    const std::string& heading,
    const char* options,
    int maximum,
    char symbol = '-') {

    bool invalid = false;

    while (true) {
        clearSearchingScreen();
        printMenuTitle(heading, symbol);
        std::cout << options << '\n';

        if (invalid)
            std::cout << "Please enter 1-" << maximum << ".\n\n";

        std::cout << "Enter choice: ";

        std::string line;
        if (!std::getline(std::cin >> std::ws, line))
            return maximum;

        std::istringstream input(line);
        int value;
        char extra;

        if ((input >> value) && !(input >> extra) &&
            value >= 1 && value <= maximum) {
            return value;
        }

        invalid = true;
    }
}

inline void pauseResults() {
    std::cout << "\nPress ENTER to go back..." << std::flush;
    std::string line;
    std::getline(std::cin, line);
}

inline std::string structureName(int structure) {
    if (structure == 1) return "ARRAY";
    if (structure == 2) return "SINGLY LINKED LIST";
    return "ARRAY VS LINKED LIST";
}

inline std::string searchName(int criterion) {
    if (criterion == 1) return "Age Group";
    if (criterion == 2) return "Care Type";
    return "Visit Duration";
}

inline ArraySearching::SearchQuery makeQuery(
    int criterion,
    int group,
    std::string& label) {

    ArraySearching::SearchQuery query;
    query.searchType = criterion;

    if (criterion == 1) {
        const int minimum[5] = {0, 18, 26, 46, 61};
        const int maximum[5] = {17, 25, 45, 60, 100};

        query.minimumAge = minimum[group];
        query.maximumAge = maximum[group];

        label = std::to_string(minimum[group]) + "-" +
                std::to_string(maximum[group]);
    }
    else if (criterion == 2) {
        const char* types[6] = {
            "Emergency", "Outpatient", "Inpatient",
            "Vaccination", "Rehabilitation", "Routine Checkup"
        };

        query.careType = types[group];
        label = types[group];
    }
    else {
        const int hours[5] = {6, 12, 24, 48, 72};

        query.durationThreshold = hours[group];
        label = "> " + std::to_string(hours[group]) + " hr";
    }

    return query;
}

inline LinkedListSearching::SearchQuery makeListQuery(
    const ArraySearching::SearchQuery& query) {

    LinkedListSearching::SearchQuery result;
    result.searchType = query.searchType;
    result.minimumAge = query.minimumAge;
    result.maximumAge = query.maximumAge;
    result.careType = query.careType;
    result.durationThreshold = query.durationThreshold;
    return result;
}

inline bool chooseSpecific(
    int criterion,
    ArraySearching::SearchQuery& query,
    std::string& label) {

    if (criterion == 1) {
        int group = choose(
            "AGE GROUPS",
            "1. 0-17   Pediatrics and Adolescents\n"
            "2. 18-25  Young Adults\n"
            "3. 26-45  Adults (Early Career)\n"
            "4. 46-60  Adults (Late Career)\n"
            "5. 61-100 Senior Citizens\n"
            "6. Back\n",
            6
        );

        if (group == 6) return false;
        query = makeQuery(criterion, group - 1, label);
        return true;
    }

    if (criterion == 2) {
        int group = choose(
            "CARE TYPES",
            "1. Emergency\n"
            "2. Outpatient\n"
            "3. Inpatient\n"
            "4. Vaccination\n"
            "5. Rehabilitation\n"
            "6. Routine Checkup\n"
            "7. Back\n",
            7
        );

        if (group == 7) return false;
        query = makeQuery(criterion, group - 1, label);
        return true;
    }

    bool invalid = false;

    while (true) {
        clearSearchingScreen();
        printMenuTitle("VISIT DURATION", '-');

        std::cout << "Find visits longer than the entered hours.\n"
                  << "B. Back\n\n";

        if (invalid)
            std::cout << "Enter a non-negative whole number.\n\n";

        std::cout << "Hours: ";

        std::string line;
        if (!std::getline(std::cin, line)) return false;

        std::istringstream backInput(line);
        std::string word;
        std::string extraWord;

        if ((backInput >> word) && !(backInput >> extraWord) &&
            (word == "B" || word == "b")) {
            return false;
        }

        std::istringstream input(line);
        int hours;
        char extra;

        if ((input >> hours) && !(input >> extra) && hours >= 0) {
            query = ArraySearching::SearchQuery();
            query.searchType = 3;
            query.durationThreshold = hours;
            label = "> " + std::to_string(hours) + " hr";
            return true;
        }

        invalid = true;
    }
}

inline void append(
    const PatientArray& source,
    PatientArray& target) {

    for (int i = 0; i < source.getSize(); ++i)
        target.add(source.get(i));
}

inline void append(
    const PatientList& source,
    PatientList& target) {

    for (const PatientNode* p = source.getHead(); p; p = p->next)
        target.insertAtEnd(p->data);
}

inline bool comesBefore(
    const Patient& first,
    const Patient& second,
    int criterion) {

    if (criterion == 1) return first.age < second.age;
    if (criterion == 2) return first.careType < second.careType;
    return first.lengthOfStay < second.lengthOfStay;
}

inline void prepareSorted(PatientArray& patients, int criterion) {
    for (int i = 1; i < patients.getSize(); ++i) {
        Patient selected = patients.get(i);
        int j = i - 1;

        while (j >= 0 &&
               comesBefore(selected, patients.get(j), criterion)) {
            patients.set(j + 1, patients.get(j));
            --j;
        }

        patients.set(j + 1, selected);
    }
}

inline void prepareSorted(PatientList& patients, int criterion) {
    for (PatientNode* p = patients.getHead(); p; p = p->next) {
        PatientNode* smallest = p;

        for (PatientNode* q = p->next; q; q = q->next) {
            if (comesBefore(q->data, smallest->data, criterion))
                smallest = q;
        }

        if (smallest != p) {
            Patient temporary = p->data;
            p->data = smallest->data;
            smallest->data = temporary;
        }
    }
}

inline bool matchesQuery(
    const Patient& patient,
    const ArraySearching::SearchQuery& query) {

    if (query.searchType == 1) {
        return patient.age >= query.minimumAge &&
               patient.age <= query.maximumAge;
    }

    if (query.searchType == 2)
        return patient.careType == query.careType;

    return patient.lengthOfStay > query.durationThreshold;
}

inline bool samePatient(const Patient& a, const Patient& b) {
    return a.patientID == b.patientID &&
           a.age == b.age &&
           a.careType == b.careType &&
           a.lengthOfStay == b.lengthOfStay &&
           a.baseCostPerHour == b.baseCostPerHour &&
           a.visitsPerYear == b.visitsPerYear &&
           a.totalCost == b.totalCost;
}

inline bool verifyResults(
    const PatientArray& source,
    const PatientArray& results,
    const ArraySearching::SearchQuery& query) {

    int index = 0;

    for (int i = 0; i < source.getSize(); ++i) {
        Patient patient = source.get(i);
        if (!matchesQuery(patient, query)) continue;

        if (index >= results.getSize() ||
            !samePatient(patient, results.get(index))) {
            return false;
        }

        ++index;
    }

    return index == results.getSize();
}

inline bool verifyResults(
    const PatientList& source,
    const PatientList& results,
    const ArraySearching::SearchQuery& query) {

    const PatientNode* result = results.getHead();

    for (const PatientNode* p = source.getHead(); p; p = p->next) {
        if (!matchesQuery(p->data, query)) continue;

        if (!result || !samePatient(p->data, result->data))
            return false;

        result = result->next;
    }

    return result == nullptr;
}

inline bool sameInput(
    const PatientArray& array,
    const PatientList& list) {

    if (array.getSize() != list.getSize()) return false;

    const PatientNode* p = list.getHead();

    for (int i = 0; i < array.getSize(); ++i) {
        if (!p || !samePatient(array.get(i), p->data))
            return false;

        p = p->next;
    }

    return p == nullptr;
}

inline void execute(
    const PatientArray& source,
    const ArraySearching::SearchQuery& query,
    bool binary,
    PatientArray& results,
    ArraySearching::SearchStats& stats) {

    if (binary)
        ArraySearching::binarySearch(source, query, results, stats);
    else
        ArraySearching::linearSearch(source, query, results, stats);
}

inline void execute(
    const PatientList& source,
    const ArraySearching::SearchQuery& query,
    bool binary,
    PatientList& results,
    LinkedListSearching::SearchStats& stats) {

    LinkedListSearching::SearchQuery q = makeListQuery(query);

    if (binary)
        LinkedListSearching::binarySearch(source, q, results, stats);
    else
        LinkedListSearching::linearSearch(source, q, results, stats);
}

inline Row runSearch(
    const PatientArray& source,
    const ArraySearching::SearchQuery& query,
    bool binary,
    const std::string& state) {

    PatientArray results;
    ArraySearching::SearchStats stats;
    execute(source, query, binary, results, stats);

    Row row;
    row.algorithm = binary ? "Binary Search" : "Linear Search";
    row.structure = "Array";
    row.state = state;
    row.matches = stats.matches;
    row.comparisons = stats.comparisons;
    row.milliseconds = stats.microseconds / 1000.0;
    row.correct = stats.matches == results.getSize() &&
                  verifyResults(source, results, query);
    return row;
}

inline Row runSearch(
    const PatientList& source,
    const ArraySearching::SearchQuery& query,
    bool binary,
    const std::string& state) {

    PatientList results;
    LinkedListSearching::SearchStats stats;
    execute(source, query, binary, results, stats);

    Row row;
    row.algorithm = binary ? "Binary Search" : "Linear Search";
    row.structure = "Singly List";
    row.state = state;
    row.matches = stats.matches;
    row.comparisons = stats.comparisons;
    row.milliseconds = stats.microseconds / 1000.0;
    row.correct = stats.matches == results.getSize() &&
                  verifyResults(source, results, query);
    return row;
}

inline void cell(
    const std::string& text,
    int width,
    bool number = false) {

    std::cout << "| ";
    if (number) std::cout << std::right;
    else std::cout << std::left;
    std::cout << std::setw(width) << text << ' ';
}

inline void border(bool comparison) {
    const int widths[8] = {15, 13, 10, 11, 7, 11, 11, 10};

    for (int i = 0; i < 8; ++i) {
        if (i == 3 && !comparison) continue;
        std::cout << '|' << std::string(widths[i] + 2, '-');
    }

    std::cout << "|\n";
}

inline void tableHeader(int criterion, bool comparison) {
    border(comparison);
    cell(searchName(criterion), 15);
    cell("Algorithm", 13);
    cell("Data State", 10);
    if (comparison) cell("Structure", 11);
    cell("Matches", 7, true);
    cell("Comparisons", 11, true);
    cell("Time (ms)", 11, true);
    cell("Aux. Space", 10, true);
    std::cout << "|\n";
    border(comparison);
}

inline void printRow(
    const std::string& label,
    const Row& row,
    bool comparison,
    bool hideAlgorithm = false,
    bool hideState = false) {

    std::ostringstream time;
    time << std::fixed << std::setprecision(6)
         << row.milliseconds;

    cell(label, 15);
    cell(hideAlgorithm ? "" : row.algorithm, 13);
    cell(hideState ? "" : row.state, 10);
    if (comparison) cell(row.structure, 11);

    std::string count = std::to_string(row.matches);
    if (!row.correct) count += "*";

    cell(count, 7, true);
    cell(std::to_string(row.comparisons), 11, true);
    cell(time.str(), 11, true);
    cell("O(1)", 10, true);
    std::cout << "|\n" << std::flush;
}

inline void runCriterion(
    int structure,
    int criterion,
    const PatientArray* const arrays[],
    const PatientList* const lists[]) {

    bool useArray = structure != 2;
    bool useList = structure != 1;
    bool comparison = structure == 3;

    PatientArray originalArray, sortedArray;
    PatientList originalList, sortedList;

    clearSearchingScreen();
    std::cout << "\nPreparing search experiment...\n" << std::flush;

    for (int i = 0; i < 3; ++i) {
        if (useArray) append(*arrays[i], originalArray);
        if (useList) append(*lists[i], originalList);
    }

    if ((useArray && originalArray.getSize() == 0) ||
        (useList && originalList.getSize() == 0)) {
        clearSearchingScreen();
        std::cout << "\nNo records loaded. Check CSV loading.\n";
        return;
    }

    if (comparison && !sameInput(originalArray, originalList))
        throw std::runtime_error(
            "Array and Linked List have different records or input order."
        );

    std::string arrayState = "Unsorted";
    std::string listState = "Unsorted";

    if (useArray) {
        if (ArraySearching::isSorted(originalArray, criterion))
            arrayState = "Sorted";

        append(originalArray, sortedArray);
        prepareSorted(sortedArray, criterion);
    }

    if (useList) {
        if (LinkedListSearching::isSorted(originalList, criterion))
            listState = "Sorted";

        append(originalList, sortedList);
        prepareSorted(sortedList, criterion);
    }

    clearSearchingScreen();
    printMenuTitle("SEARCHING PERFORMANCE - " + structureName(structure));

    std::cout << "Dataset   : Facility A + B + C\n"
              << "Search By : " << searchName(criterion) << '\n'
              << "Records   : "
              << (useArray ? originalArray.getSize()
                           : originalList.getSize())
              << '\n'
              << '\n'
              << "Note: Binary Search is performed only on sorted datasets.\n\n";

    tableHeader(criterion, comparison);

    int groups = criterion == 2 ? 6 : 5;
    bool allCorrect = true;

    for (int group = 0; group < groups; ++group) {
        std::string label;
        ArraySearching::SearchQuery query =
            makeQuery(criterion, group, label);

        int expectedArray = -1;
        int expectedList = -1;

        for (int test = 0; test < 3; ++test) {
            bool binary = test == 2;
            Row arrayRow, listRow;

            if (useArray) {
                const PatientArray& source =
                    test == 0 ? originalArray : sortedArray;

                arrayRow = runSearch(
                    source, query, binary,
                    test == 0 ? arrayState : "Sorted"
                );

                if (test == 0) expectedArray = arrayRow.matches;

                arrayRow.correct = arrayRow.correct &&
                                   arrayRow.matches == expectedArray;
            }

            if (useList) {
                const PatientList& source =
                    test == 0 ? originalList : sortedList;

                listRow = runSearch(
                    source, query, binary,
                    test == 0 ? listState : "Sorted"
                );

                if (test == 0) expectedList = listRow.matches;

                listRow.correct = listRow.correct &&
                                  listRow.matches == expectedList;
            }

            if (comparison && arrayRow.matches != listRow.matches) {
                arrayRow.correct = false;
                listRow.correct = false;
            }

            if (useArray) {
                printRow(test == 0 ? label : "", arrayRow, comparison);
                allCorrect = allCorrect && arrayRow.correct;
            }

            if (useList) {
                printRow(
                    !useArray && test == 0 ? label : "",
                    listRow, comparison, comparison,
                    comparison && listRow.state == arrayRow.state
                );

                allCorrect = allCorrect && listRow.correct;
            }
        }

        border(comparison);
        std::cout << std::flush;
    }

    if (!allCorrect)
        std::cout << "\n* Result check failed. Review the marked rows.\n";
}

inline void resultsHeading(
    int structure,
    const std::string& state,
    bool binary,
    int criterion,
    const std::string& label) {

    clearSearchingScreen();
    printMenuTitle("SEARCH RESULTS - " + structureName(structure));

    std::cout << "Dataset   : Facility A + B + C\n"
              << "Data State: " << state << '\n'
              << "Algorithm : "
              << (binary ? "Binary Search" : "Linear Search") << '\n'
              << "Search By : " << searchName(criterion)
              << " - " << label << "\n\n";

    std::cout << std::left
              << std::setw(10) << "Facility"
              << std::setw(16) << "Patient ID"
              << std::setw(7) << "Age"
              << std::setw(24) << "Care Type"
              << "Duration (hr)\n"
              << std::string(74, '-') << '\n';
}

inline void printPatient(const Patient& patient, int facility) {
    std::cout << std::left
              << std::setw(10) << std::string(1, char('A' + facility))
              << std::setw(16) << patient.patientID
              << std::setw(7) << patient.age
              << std::setw(24) << patient.careType
              << patient.lengthOfStay << '\n';
}

inline void resultsFooter(const int counts[]) {
    int total = counts[0] + counts[1] + counts[2];

    if (total == 0)
        std::cout << "No matching patient records.\n";

    std::cout << std::string(74, '-') << '\n'
              << "Facility A: " << counts[0]
              << " | Facility B: " << counts[1]
              << " | Facility C: " << counts[2]
              << "\nTotal Matches: " << total << '\n';
}

inline int findFacility(
    const Patient& patient,
    const PatientArray* const arrays[],
    bool used[]) {

    int position = 0;

    for (int facility = 0; facility < 3; ++facility) {
        for (int i = 0; i < arrays[facility]->getSize(); ++i) {
            if (!used[position] &&
                samePatient(patient, arrays[facility]->get(i))) {
                used[position] = true;
                return facility;
            }

            ++position;
        }
    }

    throw std::runtime_error("Cannot identify a result's facility.");
}

inline int findFacility(
    const Patient& patient,
    const PatientList* const lists[],
    bool used[]) {

    int position = 0;

    for (int facility = 0; facility < 3; ++facility) {
        for (const PatientNode* p = lists[facility]->getHead();
             p; p = p->next) {

            if (!used[position] && samePatient(patient, p->data)) {
                used[position] = true;
                return facility;
            }

            ++position;
        }
    }

    throw std::runtime_error("Cannot identify a result's facility.");
}

inline void searchPatients(
    int structure,
    int state,
    int algorithm,
    const ArraySearching::SearchQuery& query,
    const std::string& label,
    const PatientArray* const arrays[],
    const PatientList* const lists[]) {

    bool binary = algorithm == 2;
    int counts[3] = {0, 0, 0};

    clearSearchingScreen();
    std::cout << "\nSearching...\n" << std::flush;

    if (structure == 1) {
        PatientArray source, results;

        for (int i = 0; i < 3; ++i)
            append(*arrays[i], source);

        if (source.getSize() == 0)
            throw std::runtime_error("No records loaded. Check CSV loading.");

        if (state == 2)
            prepareSorted(source, query.searchType);

        std::string actualState =
            ArraySearching::isSorted(source, query.searchType)
            ? "Sorted" : "Unsorted";

        ArraySearching::SearchStats stats;
        execute(source, query, binary, results, stats);

        if (stats.matches != results.getSize() ||
            !verifyResults(source, results, query)) {
            throw std::runtime_error("Search result validation failed.");
        }

        std::unique_ptr<bool[]> used(new bool[source.getSize()]());

        resultsHeading(
            structure, actualState, binary, query.searchType, label
        );

        for (int i = 0; i < results.getSize(); ++i) {
            Patient patient = results.get(i);
            int facility = findFacility(patient, arrays, used.get());
            ++counts[facility];
            printPatient(patient, facility);
        }
    }
    else {
        PatientList source, results;

        for (int i = 0; i < 3; ++i)
            append(*lists[i], source);

        if (source.getSize() == 0)
            throw std::runtime_error("No records loaded. Check CSV loading.");

        if (state == 2)
            prepareSorted(source, query.searchType);

        std::string actualState =
            LinkedListSearching::isSorted(source, query.searchType)
            ? "Sorted" : "Unsorted";

        LinkedListSearching::SearchStats stats;
        execute(source, query, binary, results, stats);

        if (stats.matches != results.getSize() ||
            !verifyResults(source, results, query)) {
            throw std::runtime_error("Search result validation failed.");
        }

        std::unique_ptr<bool[]> used(new bool[source.getSize()]());

        resultsHeading(
            structure, actualState, binary, query.searchType, label
        );

        for (const PatientNode* p = results.getHead(); p; p = p->next) {
            int facility = findFacility(p->data, lists, used.get());
            ++counts[facility];
            printPatient(p->data, facility);
        }
    }

    resultsFooter(counts);
}

inline void showError(const std::exception& error) {
    clearSearchingScreen();
    printMenuTitle("SEARCH ERROR");
    std::cout << error.what() << '\n';
}

inline void patientMenu(
    const PatientArray* const arrays[],
    const PatientList* const lists[]) {

    while (std::cin) {
        int structure = choose(
            "SEARCH PATIENT RECORDS - SELECT STRUCTURE",
            "1. Array\n2. Singly Linked List\n3. Back\n", 3
        );

        if (structure == 3) return;

        while (std::cin) {
            int state = choose(
                "SELECT DATA STATE",
                "1. Unsorted\n2. Sorted\n3. Back\n", 3
            );

            if (state == 3) break;

            while (std::cin) {
                int algorithm = choose(
                    "SELECT ALGORITHM",
                    state == 1
                        ? "1. Linear Search\n2. Back\n\n"
                          "Unsorted data does not support Binary Search.\n"
                        : "1. Linear Search\n2. Binary Search\n3. Back\n",
                    state == 1 ? 2 : 3
                );

                if (algorithm == (state == 1 ? 2 : 3)) break;

                while (std::cin) {
                    int criterion = choose(
                        "SEARCH BY",
                        "1. Age Group\n2. Care Type\n"
                        "3. Visit Duration\n4. Back\n", 4
                    );

                    if (criterion == 4) break;

                    ArraySearching::SearchQuery query;
                    std::string label;

                    if (!chooseSpecific(criterion, query, label))
                        continue;

                    try {
                        searchPatients(
                            structure, state, algorithm,
                            query, label, arrays, lists
                        );
                    }
                    catch (const std::exception& error) {
                        showError(error);
                    }

                    pauseResults();
                    return;
                }
            }
        }
    }
}

inline void performanceMenu(
    const PatientArray* const arrays[],
    const PatientList* const lists[]) {

    while (std::cin) {
        int structure = choose(
            "PERFORMANCE COMPARISON",
            "1. Array\n"
            "2. Singly Linked List\n"
            "3. Compare Array vs Linked List\n"
            "4. Back\n",
            4, '='
        );

        if (structure == 4) return;

        while (std::cin) {
            int criterion = choose(
                "SEARCH BY - " + structureName(structure),
                "1. Age Group\n"
                "2. Care Type\n"
                "3. Visit Duration\n"
                "4. Back\n", 4
            );

            if (criterion == 4) break;

            try {
                runCriterion(structure, criterion, arrays, lists);
            }
            catch (const std::exception& error) {
                showError(error);
            }

            pauseResults();
        }
    }
}

inline void menu(
    const PatientArray& patientArrA,
    const PatientArray& patientArrB,
    const PatientArray& patientArrC,
    const PatientList& patientListA,
    const PatientList& patientListB,
    const PatientList& patientListC) {

    const PatientArray* arrays[3] = {
        &patientArrA, &patientArrB, &patientArrC
    };

    const PatientList* lists[3] = {
        &patientListA, &patientListB, &patientListC
    };

    while (std::cin) {
        int choice = choose(
            "SEARCHING EXPERIMENTS",
            "1. Search Patient Records\n"
            "2. Performance Comparison\n"
            "3. Back\n",
            3, '='
        );

        if (choice == 3) return;

        if (choice == 1)
            patientMenu(arrays, lists);
        else
            performanceMenu(arrays, lists);
    }
}

}

#endif