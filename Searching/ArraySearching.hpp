#ifndef ARRAY_SEARCHING_HPP
#define ARRAY_SEARCHING_HPP

#include <chrono>
#include <stdexcept>
#include <string>

#include "../Array/PatientArray.hpp"

using namespace std;

namespace ArraySearching
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
        const PatientArray& patients,
        int searchType)
    {
        for (int i = 1; i < patients.getSize(); i++)
        {
            Patient previous = patients.get(i - 1);
            Patient current = patients.get(i);

            if (searchType == 1 && previous.age > current.age)
            {
                return false;
            }

            if (searchType == 2 &&
                previous.careType > current.careType)
            {
                return false;
            }

            if (searchType == 3 &&
                previous.lengthOfStay > current.lengthOfStay)
            {
                return false;
            }
        }

        return true;
    }

    // Matches the current PatientArray implementation:
    // initial capacity 10, doubled whenever full.
    inline size_t estimateStorage(int patientCount)
    {
        size_t capacity = 10;

        while (capacity < static_cast<size_t>(patientCount))
        {
            capacity = capacity * 2;
        }

        return sizeof(PatientArray) + capacity * sizeof(Patient);
    }

    // Works on either sorted or unsorted data.
    // results must be a separate, empty PatientArray.
    inline void linearSearch(
        const PatientArray& patients,
        const SearchQuery& query,
        PatientArray& results,
        SearchStats& stats)
    {
        stats = SearchStats();

        validateQuery(query);

        if (&patients == &results || results.getSize() != 0)
        {
            throw invalid_argument(
                "Use a separate, empty array for results."
            );
        }

        auto start = chrono::steady_clock::now();

        for (int i = 0; i < patients.getSize(); i++)
        {
            Patient patient = patients.get(i);
            stats.recordsExamined++;

            if (comparePatient(patient, query, stats) == 0)
            {
                results.add(patient);
            }
        }

        auto finish = chrono::steady_clock::now();

        stats.microseconds =
            chrono::duration<double, micro>(finish - start).count();

        stats.matches = results.getSize();
        stats.inputStorageBytes = estimateStorage(patients.getSize());
        stats.resultStorageBytes = estimateStorage(results.getSize());
    }

    // Requires ascending order by the requested search field.
    // Finds the first possible match, then collects all matches.
    inline void binarySearch(
        const PatientArray& patients,
        const SearchQuery& query,
        PatientArray& results,
        SearchStats& stats)
    {
        stats = SearchStats();

        validateQuery(query);

        if (&patients == &results || results.getSize() != 0)
        {
            throw invalid_argument(
                "Use a separate, empty array for results."
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

        int left = 0;
        int right = patients.getSize();

        // Locate the first patient that is not below the query.
        while (left < right)
        {
            int middle = left + (right - left) / 2;

            Patient patient = patients.get(middle);
            stats.recordsExamined++;

            int comparison = comparePatient(patient, query, stats);

            if (comparison < 0)
            {
                left = middle + 1;
            }
            else
            {
                right = middle;
            }
        }

        // Matching patients are consecutive in sorted data.
        for (int i = left; i < patients.getSize(); i++)
        {
            Patient patient = patients.get(i);
            stats.recordsExamined++;

            if (comparePatient(patient, query, stats) != 0)
            {
                break;
            }

            results.add(patient);
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