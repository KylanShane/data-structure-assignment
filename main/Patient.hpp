#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <string>
using namespace std;

struct Patient
{
    string patientID;
    int age;
    string careType;
    int lengthOfStay;
    double baseCostPerHour;
    int visitsPerYear;
    double totalCost;
};

Patient rowToPatient(const string* row) // converts one Table row into a Patient
{
    Patient p;

    p.patientID = row[0];
    p.age = stoi(row[1]);
    p.careType = row[2];
    p.lengthOfStay = stoi(row[3]);
    p.baseCostPerHour = stod(row[4]);
    p.visitsPerYear = stoi(row[5]);

    p.totalCost = p.lengthOfStay * p.baseCostPerHour * p.visitsPerYear;

    return p;
}

#endif