#ifndef SORTINGCOMMON_HPP
#define SORTINGCOMMON_HPP

#include "../main/Patient.hpp"

namespace PatientSorting {
struct SortStats {
    long long comparisons = 0;
    long long swaps = 0;
    long long writes = 0;
    double microseconds = 0;
};

inline double key(const Patient& p, int field) {
    if (field == 1) return p.age;
    if (field == 2) return p.lengthOfStay;
    return p.totalCost;
}

inline bool outOfOrder(const Patient& a, const Patient& b,
                       int field, bool ascending) {
    return ascending ? key(a, field) > key(b, field)
                     : key(a, field) < key(b, field);
}

} // namespace PatientSorting
#endif