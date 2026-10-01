#ifndef LINKED_LIST_SEARCHING_HPP
#define LINKED_LIST_SEARCHING_HPP

#include <chrono>
#include <stdexcept>
#include <string>

#include "../SinglyLinkedList/PatientList.hpp"

using namespace std;

namespace LinkedListSearching
{
    struct SearchQuery
    {
        // 1 = age range, 2 = care type, 3 = duration threshold
        int searchType = 1;

        int minimumAge = 0;
        int maximumAge = 100;

        string careType = "";

        // Find durations strictly greater than this value.
        int durationThreshold = 0;
    };

    struct SearchStats
    {
        int matches = 0;
        long long comparisons = 0;
        long long recordsExamined = 0;

        // Counts movements through next pointers.
        long long pointerSteps = 0;

        double microseconds = 0;

        // Fixed-object storage estimates; exclude string allocations.
        size_t inputStorageBytes = 0;
        size_t resultStorageBytes = 0;
    };

    inline void validateQuery(const SearchQuery& query)
    {
        if (query.searchType < 1 || query.searchType > 3)
        {
            throw invalid_argument("Search type must be 1, 2 or 3.");
        }

        if (query.searchType == 1)
        {
            if (query.minimumAge < 0 ||
                query.maximumAge > 100 ||
                query.minimumAge > query.maximumAge)
            {
                throw invalid_argument("Invalid age range.");
            }
        }
        else if (query.searchType == 2)
        {
            if (query.careType.empty())
            {
                throw invalid_argument("Care type cannot be empty.");
            }
        }
        else if (query.durationThreshold < 0)
        {
            throw invalid_argument("Duration cannot be negative.");
        }
    }

    // Returns:
    // -1 = patient is below the requested range/value
    //  0 = patient matches
    //  1 = patient is above the requested range/value
    inline int comparePatient(
        const Patient& patient,
        const SearchQuery& query,
        SearchStats& stats)
    {
        if (query.searchType == 1)
        {
            stats.comparisons++;

            if (patient.age < query.minimumAge)
            {
                return -1;
            }

            stats.comparisons++;

            if (patient.age > query.maximumAge)
            {
                return 1;
            }

            return 0;
        }

        if (query.searchType == 2)
        {
            // Count one string comparison, not individual characters.
            stats.comparisons++;

            int comparison = patient.careType.compare(query.careType);

            if (comparison < 0)
            {
                return -1;
            }

            if (comparison > 0)
            {
                return 1;
            }

            return 0;
        }

        stats.comparisons++;

        if (patient.lengthOfStay <= query.durationThreshold)
        {
            return -1;
        }

        return 0;
    }

    // Check ascending order using the requested search field.
    inline bool isSorted(
        const PatientList& patients,
        int searchType)
    {
        const PatientNode* current = patients.getHead();

        while (current != nullptr && current->next != nullptr)
        {
            const Patient& first = current->data;
            const Patient& second = current->next->data;

            if (searchType == 1 && first.age > second.age)
            {
                return false;
            }

            if (searchType == 2 && first.careType > second.careType)
            {
                return false;
            }

            if (searchType == 3 &&
                first.lengthOfStay > second.lengthOfStay)
            {
                return false;
            }

            current = current->next;
        }

        return true;
    }

    inline size_t estimateStorage(int patientCount)
    {
        return sizeof(PatientList) +
               static_cast<size_t>(patientCount) * sizeof(PatientNode);
    }

    // Works on either sorted or unsorted data.
    // results must be a separate, empty PatientList.
    inline void linearSearch(
        const PatientList& patients,
        const SearchQuery& query,
        PatientList& results,
        SearchStats& stats)
    {
        stats = SearchStats();

        validateQuery(query);

        if (&patients == &results || results.getSize() != 0)
        {
            throw invalid_argument(
                "Use a separate, empty linked list for results."
            );
        }

        auto start = chrono::steady_clock::now();

        const PatientNode* current = patients.getHead();

        while (current != nullptr)
        {
            stats.recordsExamined++;

            if (comparePatient(current->data, query, stats) == 0)
            {
                results.insertAtEnd(current->data);
            }

            current = current->next;
            stats.pointerSteps++;
        }

        auto finish = chrono::steady_clock::now();

        stats.microseconds =
            chrono::duration<double, micro>(finish - start).count();

        stats.matches = results.getSize();
        stats.inputStorageBytes = estimateStorage(patients.getSize());
        stats.resultStorageBytes = estimateStorage(results.getSize());
    }

    // Requires ascending order by the requested search field.
    // The search stays on linked-list nodes; no array is created.
    inline void binarySearch(
        const PatientList& patients,
        const SearchQuery& query,
        PatientList& results,
        SearchStats& stats)
    {
        stats = SearchStats();

        validateQuery(query);

        if (&patients == &results || results.getSize() != 0)
        {
            throw invalid_argument(
                "Use a separate, empty linked list for results."
            );
        }

        if (!isSorted(patients, query.searchType))
        {
            throw invalid_argument(
                "Binary search requires ascending data "
                "sorted by the requested search field."
            );
        }

        // Validation above is excluded from the search measurements.
        auto start = chrono::steady_clock::now();

        const PatientNode* first = patients.getHead();
        int remaining = patients.getSize();

        // Locate the first patient that is not below the query.
        while (remaining > 0)
        {
            int half = remaining / 2;
            const PatientNode* middle = first;

            // A linked list must walk to its middle node.
            for (int i = 0; i < half; i++)
            {
                middle = middle->next;
                stats.pointerSteps++;
            }

            stats.recordsExamined++;

            int comparison =
                comparePatient(middle->data, query, stats);

            if (comparison < 0)
            {
                first = middle->next;
                stats.pointerSteps++;

                remaining = remaining - half - 1;
            }
            else
            {
                remaining = half;
            }
        }

        // Matching patients are consecutive in sorted data.
        const PatientNode* current = first;

        while (current != nullptr)
        {
            stats.recordsExamined++;

            if (comparePatient(current->data, query, stats) != 0)
            {
                break;
            }

            results.insertAtEnd(current->data);

            current = current->next;
            stats.pointerSteps++;
        }

        auto finish = chrono::steady_clock::now();

        stats.microseconds =
            chrono::duration<double, micro>(finish - start).count();

        stats.matches = results.getSize();
        stats.inputStorageBytes = estimateStorage(patients.getSize());
        stats.resultStorageBytes = estimateStorage(results.getSize());
    }
}

#endif