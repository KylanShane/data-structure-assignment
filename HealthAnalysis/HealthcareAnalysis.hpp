#ifndef HEALTHCARE_ANALYSIS_HPP
#define HEALTHCARE_ANALYSIS_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

#include "../main/Patient.hpp"
#include "../Array/PatientArray.hpp"
#include "../SinglyLinkedList/PatientList.hpp"

using namespace std;


// Stores the analysis results for one age group
struct AgeGroupStats
{
    int patientCount;
    double totalCost;
    double totalDuration;

    int carePatientCount[6];
    double careTotalCost[6];
};


// Stores the overall analysis results
struct OverallStats
{
    int patientCount;
    double totalCost;
    double totalDuration;

    int carePatientCount[6];
    double careTotalCost[6];
};


// Age group names
const string AGE_GROUP_NAMES[5] =
{
    "0-17: Pediatrics & Adolescents",
    "18-25: Young Adults / University Students",
    "26-45: Working Adults (Early Career)",
    "46-60: Working Adults (Late Career)",
    "61-100: Senior Citizens / Geriatric Care"
};


// Care type names
const string CARE_TYPE_NAMES[6] =
{
    "Emergency",
    "Inpatient",
    "Outpatient",
    "Vaccination",
    "Rehabilitation",
    "Routine Checkup"
};


// Formats a double as a cost string with $ and comma separators so it is easier to read
string formatCost(double value)
{
    // Separate the whole number and cents
    long long whole = (long long)value;
    int cents = (int)((value - whole) * 100 + 0.5);

    // Convert the whole number to a string
    string number = to_string(whole);

    // Add commas every 3 digits
    int commaPosition = (int)number.length() - 3;

    while (commaPosition > 0)
    {
        number.insert(commaPosition, ",");
        commaPosition -= 3;
    }

    // Make sure cents always have 2 digits
    string centString = to_string(cents);

    if (centString.length() < 2)
    {
        centString = "0" + centString;
    }

    return "$" + number + "." + centString;
}


// Returns the age group index
int getAgeGroupIndex(int age)
{
    if (age >= 0 && age <= 17)
    {
        return 0;
    }
    else if (age >= 18 && age <= 25)
    {
        return 1;
    }
    else if (age >= 26 && age <= 45)
    {
        return 2;
    }
    else if (age >= 46 && age <= 60)
    {
        return 3;
    }
    else if (age >= 61 && age <= 100)
    {
        return 4;
    }

    return -1;
}


// Returns the care type index
int getCareTypeIndex(const string& careType)
{
    for (int i = 0; i < 6; i++)
    {
        if (careType == CARE_TYPE_NAMES[i])
        {
            return i;
        }
    }

    return -1;
}


// Resets age group statistics
void resetAgeGroupStats(AgeGroupStats stats[])
{
    for (int i = 0; i < 5; i++)
    {
        stats[i].patientCount = 0;
        stats[i].totalCost = 0.0;
        stats[i].totalDuration = 0.0;

        for (int j = 0; j < 6; j++)
        {
            stats[i].carePatientCount[j] = 0;
            stats[i].careTotalCost[j] = 0.0;
        }
    }
}


// Resets overall statistics
void resetOverallStats(OverallStats& stats)
{
    stats.patientCount = 0;
    stats.totalCost = 0.0;
    stats.totalDuration = 0.0;

    for (int i = 0; i < 6; i++)
    {
        stats.carePatientCount[i] = 0;
        stats.careTotalCost[i] = 0.0;
    }
}


// Adds one patient into the statistics
void addPatientToStats(
    const Patient& p,
    AgeGroupStats ageStats[],
    OverallStats& overallStats)
{
    int ageIndex = getAgeGroupIndex(p.age);
    int careIndex = getCareTypeIndex(p.careType);

    // Update overall statistics
    overallStats.patientCount++;
    overallStats.totalCost += p.totalCost;
    overallStats.totalDuration += p.lengthOfStay;

    if (careIndex != -1)
    {
        overallStats.carePatientCount[careIndex]++;
        overallStats.careTotalCost[careIndex] += p.totalCost;
    }

    // Update age group statistics
    if (ageIndex != -1)
    {
        ageStats[ageIndex].patientCount++;
        ageStats[ageIndex].totalCost += p.totalCost;
        ageStats[ageIndex].totalDuration += p.lengthOfStay;

        if (careIndex != -1)
        {
            ageStats[ageIndex].carePatientCount[careIndex]++;
            ageStats[ageIndex].careTotalCost[careIndex] += p.totalCost;
        }
    }
}


// Analyse a PatientArray
void analysePatients(
    const PatientArray& patients,
    AgeGroupStats ageStats[],
    OverallStats& overallStats)
{
    resetAgeGroupStats(ageStats);
    resetOverallStats(overallStats);

    for (int i = 0; i < patients.getSize(); i++)
    {
        Patient p = patients.get(i);

        addPatientToStats(
            p,
            ageStats,
            overallStats
        );
    }
}


// Analyse a PatientList
void analysePatients(
    const PatientList& patients,
    AgeGroupStats ageStats[],
    OverallStats& overallStats)
{
    resetAgeGroupStats(ageStats);
    resetOverallStats(overallStats);

    PatientNode* current = patients.getHead();

    while (current != nullptr)
    {
        addPatientToStats(
            current->data,
            ageStats,
            overallStats
        );

        current = current->next;
    }
}


// Find the most preferred care type for an age group
string getTopCareType(const AgeGroupStats& stats)
{
    int highestCount = 0;
    int topIndex = -1;

    for (int i = 0; i < 6; i++)
    {
        if (stats.carePatientCount[i] > highestCount)
        {
            highestCount = stats.carePatientCount[i];
            topIndex = i;
        }
    }

    if (topIndex == -1)
    {
        return "None";
    }

    return CARE_TYPE_NAMES[topIndex];
}


// Find the most preferred overall care type
string getTopCareType(const OverallStats& stats)
{
    int highestCount = 0;
    int topIndex = -1;

    for (int i = 0; i < 6; i++)
    {
        if (stats.carePatientCount[i] > highestCount)
        {
            highestCount = stats.carePatientCount[i];
            topIndex = i;
        }
    }

    if (topIndex == -1)
    {
        return "None";
    }

    return CARE_TYPE_NAMES[topIndex];
}


// Display the facility report
void displayFacilityReport(
    const string& facilityName,
    const AgeGroupStats ageStats[],
    const OverallStats& overallStats)
{
    clearScreen();

    cout << "==============================================================\n";
    cout << "FACILITY REPORT: " << facilityName << "\n";
    cout << "==============================================================\n\n";

    double averageCost = 0.0;
    double averageDuration = 0.0;

    if (overallStats.patientCount > 0)
    {
        averageCost =
            overallStats.totalCost /
            overallStats.patientCount;

        averageDuration =
            overallStats.totalDuration /
            overallStats.patientCount;
    }

    cout << fixed << setprecision(2);

    cout << "Total Patients             : "
         << overallStats.patientCount << "\n";

    cout << "Total Medical Cost         : "
         << formatCost(overallStats.totalCost) << "\n";

    cout << "Average Cost per Patient   : "
         << formatCost(averageCost) << "\n";

    cout << "Total Visit Duration       : "
         << overallStats.totalDuration << " hours\n";

    cout << "Average Visit Duration     : "
         << averageDuration << " hours\n";

    cout << "==============================================================\n\n";


    // Age group table
    cout << left
         << setw(44) << "Age Group"
         << right
         << setw(10) << "Patients"
         << setw(18) << "Visit Duration"
         << setw(18) << "Total Cost"
         << setw(14) << "Avg Cost"
         << setw(20) << "Top Care Type"
         << "\n";

    cout << string(124, '-') << "\n";

    for (int i = 0; i < 5; i++)
    {
        if (ageStats[i].patientCount > 0)
        {
            double averageAgeCost =
                ageStats[i].totalCost /
                ageStats[i].patientCount;

            cout << left
                 << setw(44) << AGE_GROUP_NAMES[i]
                 << right
                 << setw(10) << ageStats[i].patientCount
                 << setw(18) << ageStats[i].totalDuration
                 << setw(18) << formatCost(ageStats[i].totalCost)
                 << setw(14) << formatCost(averageAgeCost)
                 << setw(20) << getTopCareType(ageStats[i])
                 << "\n";
        }
    }

    cout << string(124, '-') << "\n\n";


    // Care type table
    cout << left
         << setw(22) << "Care Type"
         << right
         << setw(12) << "Patients"
         << setw(22) << "Total Cost"
         << setw(24) << "Avg Cost/Patient"
         << "\n";

    cout << string(80, '-') << "\n";

    for (int i = 0; i < 6; i++)
    {
        if (overallStats.carePatientCount[i] > 0)
        {
            int patientCount =
                overallStats.carePatientCount[i];

            double totalCost =
                overallStats.careTotalCost[i];

            double averageCareCost =
                totalCost / patientCount;

            cout << left
                 << setw(22) << CARE_TYPE_NAMES[i]
                 << right
                 << setw(12) << patientCount
                 << setw(22) << formatCost(totalCost)
                 << setw(24) << formatCost(averageCareCost)
                 << "\n";
        }
    }

    cout << string(80, '-') << "\n";

    cout << left
         << setw(22) << "TOTAL"
         << right
         << setw(12) << overallStats.patientCount
         << setw(22) << formatCost(overallStats.totalCost)
         << setw(24) << formatCost(averageCost)
         << "\n";

    pressEnterToContinue();
}


// Display one age group's detailed analysis
void displayAgeGroupAnalysis(
    int ageGroupIndex,
    const AgeGroupStats& stats)
{
    double averageCost = 0.0;
    double averageDuration = 0.0;

    if (stats.patientCount > 0)
    {
        averageCost =
            stats.totalCost /
            stats.patientCount;

        averageDuration =
            stats.totalDuration /
            stats.patientCount;
    }

    cout << "\n";
    cout << "====================================================================\n";
    cout << "Age Group: " << AGE_GROUP_NAMES[ageGroupIndex] << "\n";
    cout << "====================================================================\n\n";

    cout << fixed << setprecision(2);

    cout << "Total Patients             : "
         << stats.patientCount << "\n";

    cout << "Total Medical Cost         : "
         << formatCost(stats.totalCost) << "\n";

    cout << "Average Cost per Patient   : "
         << formatCost(averageCost) << "\n";

    cout << "Total Visit Duration       : "
         << stats.totalDuration << " hours\n";

    cout << "Average Visit Duration     : "
         << averageDuration << " hours\n";

    cout << "Most Preferred Care Type   : "
         << getTopCareType(stats) << "\n";

    cout << "\n";

    // Care type table
    cout << string(84, '-') << "\n";

    cout << left
         << setw(30) << "Care Type"
         << right
         << setw(12) << "Patients"
         << setw(20) << "Total Cost"
         << setw(22) << "Avg Cost / Patient"
         << "\n";

    cout << string(84, '-') << "\n";

    bool hasCareType = false;

    for (int i = 0; i < 6; i++)
    {
        if (stats.carePatientCount[i] > 0)
        {
            hasCareType = true;

            double averageCareCost =
                stats.careTotalCost[i] /
                stats.carePatientCount[i];

            cout << left
                 << setw(30) << CARE_TYPE_NAMES[i]
                 << right
                 << setw(12) << stats.carePatientCount[i]
                 << setw(20) << formatCost(stats.careTotalCost[i])
                 << setw(22) << formatCost(averageCareCost)
                 << "\n";
        }
    }

    if (!hasCareType)
    {
        cout << "No patients in this age group.\n";
    }

    cout << string(84, '-') << "\n";
}


// Display all age groups for combined analysis
void displayCombinedPerAgeGroup(
    const AgeGroupStats ageStats[])
{
    clearScreen();

    cout << "====================================================================\n";
    cout << "              COMBINED ANALYSIS BY AGE GROUP\n";
    cout << "====================================================================\n";

    cout << fixed << setprecision(2);

    for (int i = 0; i < 5; i++)
    {
        displayAgeGroupAnalysis(
            i,
            ageStats[i]
        );
    }

    pressEnterToContinue();
}


// Display the overall combined analysis
void displayCombinedOverview(
    const OverallStats& stats)
{
    clearScreen();

    double averageCost = 0.0;
    double averageDuration = 0.0;

    if (stats.patientCount > 0)
    {
        averageCost =
            stats.totalCost /
            stats.patientCount;

        averageDuration =
            stats.totalDuration /
            stats.patientCount;
    }

    cout << "====================================================================\n";
    cout << "                 COMBINED ANALYSIS - OVERVIEW\n";
    cout << "====================================================================\n\n";

    cout << fixed << setprecision(2);

    cout << "Total Patients             : "
         << stats.patientCount << "\n";

    cout << "Total Medical Cost         : "
         << formatCost(stats.totalCost) << "\n";

    cout << "Average Cost per Patient   : "
         << formatCost(averageCost) << "\n";

    cout << "Total Visit Duration       : "
         << stats.totalDuration << " hours\n";

    cout << "Average Visit Duration     : "
         << averageDuration << " hours\n";

    cout << "Most Preferred Care Type   : "
         << getTopCareType(stats) << "\n";

    cout << "\n";

    // Care type table
    cout << string(84, '-') << "\n";

    cout << left
         << setw(30) << "Care Type"
         << right
         << setw(12) << "Patients"
         << setw(20) << "Total Cost"
         << setw(22) << "Avg Cost / Patient"
         << "\n";

    cout << string(84, '-') << "\n";

    for (int i = 0; i < 6; i++)
    {
        if (stats.carePatientCount[i] > 0)
        {
            double averageCareCost =
                stats.careTotalCost[i] /
                stats.carePatientCount[i];

            cout << left
                 << setw(30) << CARE_TYPE_NAMES[i]
                 << right
                 << setw(12) << stats.carePatientCount[i]
                 << setw(20) << formatCost(stats.careTotalCost[i])
                 << setw(22) << formatCost(averageCareCost)
                 << "\n";
        }
    }

    cout << string(84, '-') << "\n";

    pressEnterToContinue();
}


// Menu for combined analysis
void combinedAnalysisMenu(
    const AgeGroupStats ageStats[],
    const OverallStats& overallStats)
{
    int choice;

    while (true)
    {
        clearScreen();

        cout << "======================================================\n";
        cout << "          COMBINED ANALYSIS - ALL FACILITIES\n";
        cout << "======================================================\n\n";

        cout << "1. By Age Group\n";
        cout << "2. Overview\n";
        cout << "3. Back\n";

        cout << "\n======================================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            displayCombinedPerAgeGroup(ageStats);
        }
        else if (choice == 2)
        {
            displayCombinedOverview(overallStats);
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}


// Analysis menu for one data structure
void healthcareStructureMenu(
    const string& structureName,
    const string& facilityAName,
    const string& facilityBName,
    const string& facilityCName,
    const AgeGroupStats ageStatsA[],
    const OverallStats& overallStatsA,
    const AgeGroupStats ageStatsB[],
    const OverallStats& overallStatsB,
    const AgeGroupStats ageStatsC[],
    const OverallStats& overallStatsC,
    const AgeGroupStats combinedAgeStats[],
    const OverallStats& combinedOverallStats)
{
    int choice;

    while (true)
    {
        clearScreen();

        cout << "======================================================\n";
        cout << "  HEALTHCARE ANALYSIS - " << structureName << "\n";
        cout << "======================================================\n\n";

        cout << "1. Facility A\n";
        cout << "2. Facility B\n";
        cout << "3. Facility C\n";
        cout << "4. Combined Analysis (All Facilities)\n";
        cout << "5. Back\n";

        cout << "\n======================================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            displayFacilityReport(
                facilityAName,
                ageStatsA,
                overallStatsA
            );
        }
        else if (choice == 2)
        {
            displayFacilityReport(
                facilityBName,
                ageStatsB,
                overallStatsB
            );
        }
        else if (choice == 3)
        {
            displayFacilityReport(
                facilityCName,
                ageStatsC,
                overallStatsC
            );
        }
        else if (choice == 4)
        {
            combinedAnalysisMenu(
                combinedAgeStats,
                combinedOverallStats
            );
        }
        else if (choice == 5)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}


// Main healthcare analysis menu
void healthcareMenu(
    const PatientArray& arrayA,
    const PatientArray& arrayB,
    const PatientArray& arrayC,
    const PatientList& listA,
    const PatientList& listB,
    const PatientList& listC)
{
    // Analyse Array data
    AgeGroupStats arrayAgeA[5];
    AgeGroupStats arrayAgeB[5];
    AgeGroupStats arrayAgeC[5];
    AgeGroupStats arrayAgeCombined[5];

    OverallStats arrayOverallA;
    OverallStats arrayOverallB;
    OverallStats arrayOverallC;
    OverallStats arrayOverallCombined;

    analysePatients(
        arrayA,
        arrayAgeA,
        arrayOverallA
    );

    analysePatients(
        arrayB,
        arrayAgeB,
        arrayOverallB
    );

    analysePatients(
        arrayC,
        arrayAgeC,
        arrayOverallC
    );

    // Analyse Linked List data
    AgeGroupStats listAgeA[5];
    AgeGroupStats listAgeB[5];
    AgeGroupStats listAgeC[5];
    AgeGroupStats listAgeCombined[5];

    OverallStats listOverallA;
    OverallStats listOverallB;
    OverallStats listOverallC;
    OverallStats listOverallCombined;

    analysePatients(
        listA,
        listAgeA,
        listOverallA
    );

    analysePatients(
        listB,
        listAgeB,
        listOverallB
    );

    analysePatients(
        listC,
        listAgeC,
        listOverallC
    );

    // Combine Array results
    resetAgeGroupStats(arrayAgeCombined);
    resetOverallStats(arrayOverallCombined);

    for (int age = 0; age < 5; age++)
    {
        arrayAgeCombined[age].patientCount =
            arrayAgeA[age].patientCount +
            arrayAgeB[age].patientCount +
            arrayAgeC[age].patientCount;

        arrayAgeCombined[age].totalCost =
            arrayAgeA[age].totalCost +
            arrayAgeB[age].totalCost +
            arrayAgeC[age].totalCost;

        arrayAgeCombined[age].totalDuration =
            arrayAgeA[age].totalDuration +
            arrayAgeB[age].totalDuration +
            arrayAgeC[age].totalDuration;

        for (int care = 0; care < 6; care++)
        {
            arrayAgeCombined[age].carePatientCount[care] =
                arrayAgeA[age].carePatientCount[care] +
                arrayAgeB[age].carePatientCount[care] +
                arrayAgeC[age].carePatientCount[care];

            arrayAgeCombined[age].careTotalCost[care] =
                arrayAgeA[age].careTotalCost[care] +
                arrayAgeB[age].careTotalCost[care] +
                arrayAgeC[age].careTotalCost[care];
        }
    }

    arrayOverallCombined.patientCount =
        arrayOverallA.patientCount +
        arrayOverallB.patientCount +
        arrayOverallC.patientCount;

    arrayOverallCombined.totalCost =
        arrayOverallA.totalCost +
        arrayOverallB.totalCost +
        arrayOverallC.totalCost;

    arrayOverallCombined.totalDuration =
        arrayOverallA.totalDuration +
        arrayOverallB.totalDuration +
        arrayOverallC.totalDuration;

    for (int care = 0; care < 6; care++)
    {
        arrayOverallCombined.carePatientCount[care] =
            arrayOverallA.carePatientCount[care] +
            arrayOverallB.carePatientCount[care] +
            arrayOverallC.carePatientCount[care];

        arrayOverallCombined.careTotalCost[care] =
            arrayOverallA.careTotalCost[care] +
            arrayOverallB.careTotalCost[care] +
            arrayOverallC.careTotalCost[care];
    }


    // Combine Linked List results
    resetAgeGroupStats(listAgeCombined);
    resetOverallStats(listOverallCombined);

    for (int age = 0; age < 5; age++)
    {
        listAgeCombined[age].patientCount =
            listAgeA[age].patientCount +
            listAgeB[age].patientCount +
            listAgeC[age].patientCount;

        listAgeCombined[age].totalCost =
            listAgeA[age].totalCost +
            listAgeB[age].totalCost +
            listAgeC[age].totalCost;

        listAgeCombined[age].totalDuration =
            listAgeA[age].totalDuration +
            listAgeB[age].totalDuration +
            listAgeC[age].totalDuration;

        for (int care = 0; care < 6; care++)
        {
            listAgeCombined[age].carePatientCount[care] =
                listAgeA[age].carePatientCount[care] +
                listAgeB[age].carePatientCount[care] +
                listAgeC[age].carePatientCount[care];

            listAgeCombined[age].careTotalCost[care] =
                listAgeA[age].careTotalCost[care] +
                listAgeB[age].careTotalCost[care] +
                listAgeC[age].careTotalCost[care];
        }
    }

    listOverallCombined.patientCount =
        listOverallA.patientCount +
        listOverallB.patientCount +
        listOverallC.patientCount;

    listOverallCombined.totalCost =
        listOverallA.totalCost +
        listOverallB.totalCost +
        listOverallC.totalCost;

    listOverallCombined.totalDuration =
        listOverallA.totalDuration +
        listOverallB.totalDuration +
        listOverallC.totalDuration;

    for (int care = 0; care < 6; care++)
    {
        listOverallCombined.carePatientCount[care] =
            listOverallA.carePatientCount[care] +
            listOverallB.carePatientCount[care] +
            listOverallC.carePatientCount[care];

        listOverallCombined.careTotalCost[care] =
            listOverallA.careTotalCost[care] +
            listOverallB.careTotalCost[care] +
            listOverallC.careTotalCost[care];
    }


    int choice;

    while (true)
    {
        clearScreen();

        cout << "======================================================\n";
        cout << "  HEALTHCARE EXPENDITURE & SERVICE ANALYSIS\n";
        cout << "======================================================\n\n";

        cout << "1. Array\n";
        cout << "2. Singly Linked List\n";
        cout << "3. Back\n";

        cout << "\n======================================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            healthcareStructureMenu(
                "ARRAY",
                "General Hospital (Facility A)",
                "University Medical Center (Facility B)",
                "Community Health Clinic (Facility C)",
                arrayAgeA,
                arrayOverallA,
                arrayAgeB,
                arrayOverallB,
                arrayAgeC,
                arrayOverallC,
                arrayAgeCombined,
                arrayOverallCombined
            );
        }
        else if (choice == 2)
        {
            healthcareStructureMenu(
                "SINGLY LINKED LIST",
                "General Hospital (Facility A)",
                "University Medical Center (Facility B)",
                "Community Health Clinic (Facility C)",
                listAgeA,
                listOverallA,
                listAgeB,
                listOverallB,
                listAgeC,
                listOverallC,
                listAgeCombined,
                listOverallCombined
            );
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
}

#endif