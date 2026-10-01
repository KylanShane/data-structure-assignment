

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

void clearScreen();
void pressEnterToContinue();

#include "Patient.hpp"
#include "../Array/PatientArray.hpp"
#include "../SinglyLinkedList/PatientList.hpp"
#include "../HealthAnalysis/HealthcareAnalysis.hpp"
#include "../Searching/SearchingMenu.hpp"
#include "../sorting/SortingMenu.hpp"

using namespace std;


void clearScreen() // To clear the console screen
{
    system("cls");
}


void pressEnterToContinue() // To pauae the console until the user press enter to proceed
{
    cout << "\n========================================\n";
    cout << "Press ENTER to return to the Main Menu...";
    cout << "\n========================================";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


#include "../HealthAnalysis/HealthcareAnalysis.hpp"

struct Table // replaces vector<vector<string>>, a 2D dynamic array of strings
{
    string** cells;   // cells[row][column]
    int* colCount;    // number of columns in each row
    int rows;         // number of rows

    Table()
    {
        cells = nullptr;
        colCount = nullptr;
        rows = 0;
    }

    ~Table()
    {
        clear();
    }

    Table(const Table&) = delete;            // avoid accidental copying
    Table& operator=(const Table&) = delete;

    void allocate(int r) // reserve r empty rows
    {
        clear();

        rows = r;

        if (r > 0)
        {
            cells = new string*[r];
            colCount = new int[r];

            for (int i = 0; i < r; i++)
            {
                cells[i] = nullptr;
                colCount[i] = 0;
            }
        }
    }

    void clear() // free all memory
    {
        if (cells != nullptr)
        {
            for (int i = 0; i < rows; i++)
            {
                delete[] cells[i];
            }

            delete[] cells;
        }

        delete[] colCount;

        cells = nullptr;
        colCount = nullptr;
        rows = 0;
    }
};


void readCSV(const string& filename, Table& table) //dataset readers
{
    table.clear();

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Cannot open file: "
             << filename << endl;

        return;
    }

    string line;
    int lineCount = 0;

    while (getline(file, line)) // first pass: count the rows
    {
        lineCount++;
    }

    file.clear();
    file.seekg(0);

    table.allocate(lineCount);

    int r = 0;

    while (r < lineCount && getline(file, line)) // second pass: fill the rows
    {
        string value;
        int count = 0;

        stringstream counter(line);

        while (getline(counter, value, ','))
        {
            count++;
        }

        table.cells[r] = new string[count];
        table.colCount[r] = count;

        stringstream ss(line);

        for (int c = 0; c < count; c++)
        {
            getline(ss, value, ',');
            table.cells[r][c] = value;
        }

        r++;
    }

    file.close();
}



void displayData( //to display the data from the csv
    const Table& data,
    const string& title)
{
    clearScreen();

    cout << "========================================\n";
    cout << "          " << title << "\n";
    cout << "========================================\n\n";

    if (data.rows == 0)
    {
        cout << "No data found.\n";
    }
    else
    {
        for (int i = 0; i < data.rows; i++)
        {
            for (int j = 0; j < data.colCount[i]; j++)
            {
                cout << data.cells[i][j] << "\t";
            }

            cout << endl;
        }
    }

    pressEnterToContinue();
}



void combineData(  //to combine the data from the three facilities
    const Table& A,
    const Table& B,
    const Table& C,
    Table& combined)
{
    combined.allocate(A.rows + B.rows + C.rows);

    int r = 0;

    const Table* sources[3] = { &A, &B, &C };

    for (int s = 0; s < 3; s++)
    {
        for (int i = 0; i < sources[s]->rows; i++)
        {
            int count = sources[s]->colCount[i];

            combined.cells[r] = new string[count];
            combined.colCount[r] = count;

            for (int j = 0; j < count; j++)
            {
                combined.cells[r][j] = sources[s]->cells[i][j];
            }

            r++;
        }
    }
}



void displayArray( //array section
    const Table& data,
    const string& title)
{
    clearScreen();

    cout << "========================================\n";
    cout << "              " << title << "\n";
    cout << "========================================\n\n";

    if (data.rows == 0)
    {
        cout << "No data found.\n";
    }
    else
    {
        for (int i = 0; i < data.rows; i++)
        {
            for (int j = 0; j < data.colCount[i]; j++)
            {
                cout << data.cells[i][j] << "\t";
            }

            cout << endl;
        }
    }

    pressEnterToContinue();
}


void datasetsMenu( //datasets menu
    const Table& A,
    const Table& B,
    const Table& C)
{
    int choice;

    while (true)
    {
        clearScreen();

        cout << "========================================\n";
        cout << "                DATASETS\n";
        cout << "========================================\n\n";

        cout << "1. Facility A dataset\n";
        cout << "2. Facility B dataset\n";
        cout << "3. Facility C dataset\n";
        cout << "4. Back\n";

        cout << "\n========================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            displayData(A, "FACILITY A DATASET");
        }
        else if (choice == 2)
        {
            displayData(B, "FACILITY B DATASET");
        }
        else if (choice == 3)
        {
            displayData(C, "FACILITY C DATASET");
        }
        else if (choice == 4)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cin.get();
        }
    }
}


void placeholderMenu( //generic menu with empty buttons, the last option is always Back
    const string& title,
    const string options[],
    int optionCount)
{
    int choice;
    int backChoice = optionCount + 1;

    while (true)
    {
        clearScreen();

        cout << "========================================\n";
        cout << "  " << title << "\n";
        cout << "========================================\n\n";

        for (int i = 0; i < optionCount; i++)
        {
            cout << i + 1 << ". " << options[i] << "\n";
        }

        cout << backChoice << ". Back\n";

        cout << "\n========================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice >= 1 && choice <= optionCount)
        {
            cout << "\nFeature under development.";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        else if (choice == backChoice)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cin.get();
        }
    }
}

void displayPatientArray(const PatientArray& arr, const string& title)
{
    clearScreen();

    cout << "========================================\n";
    cout << "          " << title << "\n";
    cout << "========================================\n\n";

    if (arr.getSize() == 0)
    {
        cout << "No data found.\n";
    }
    else
    {
        for (int i = 0; i < arr.getSize(); i++)
        {
            Patient p = arr.get(i);

            cout << p.patientID << "\t"
                 << p.age << "\t"
                 << p.careType << "\t"
                 << p.lengthOfStay << "\t"
                 << p.baseCostPerHour << "\t"
                 << p.visitsPerYear << "\t"
                 << p.totalCost << endl;
        }
    }

    pressEnterToContinue();
}

void arrayMenu( //array menu
    const PatientArray& A,
    const PatientArray& B,
    const PatientArray& C)
{
    int choice;

    while (true)
    {
        clearScreen();

        cout << "========================================\n";
        cout << "                 ARRAYS\n";
        cout << "========================================\n\n";

        cout << "1. Facility A\n";
        cout << "2. Facility B\n";
        cout << "3. Facility C\n";
        cout << "4. Back\n";

        cout << "\n========================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            displayPatientArray(A, "FACILITY A - ARRAY");
        }
        else if (choice == 2)
        {
            displayPatientArray(B, "FACILITY B - ARRAY");
        }
        else if (choice == 3)
        {
            displayPatientArray(C, "FACILITY C - ARRAY");
        }
        else if (choice == 4)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cin.get();
        }
    }
}


void displayPatientList(const PatientList& list, const string& title)
{
    clearScreen();

    cout << "========================================\n";
    cout << "          " << title << "\n";
    cout << "========================================\n\n";

    if (list.getSize() == 0)
    {
        cout << "No data found.\n";
    }
    else
    {
        PatientNode* current = list.getHead();

        while (current != nullptr)
        {
            Patient p = current->data;

            cout << p.patientID << "\t"
                 << p.age << "\t"
                 << p.careType << "\t"
                 << p.lengthOfStay << "\t"
                 << p.baseCostPerHour << "\t"
                 << p.visitsPerYear << "\t"
                 << p.totalCost << endl;

            current = current->next;
        }
    }

    pressEnterToContinue();
}


void linkedListMenu( //linkedlist menu
    const PatientList& A,
    const PatientList& B,
    const PatientList& C)
{
    int choice;

    while (true)
    {
        clearScreen();

        cout << "========================================\n";
        cout << "          SINGLY LINKED LIST\n";
        cout << "========================================\n\n";

        cout << "1. Facility A\n";
        cout << "2. Facility B\n";
        cout << "3. Facility C\n";
        cout << "4. Back\n";

        cout << "\n========================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            displayPatientList(A, "FACILITY A - LINKED LIST");
        }
        else if (choice == 2)
        {
            displayPatientList(B, "FACILITY B - LINKED LIST");
        }
        else if (choice == 3)
        {
            displayPatientList(C, "FACILITY C - LINKED LIST");
        }
        else if (choice == 4)
        {
            return;
        }
        else
        {
            cout << "\nInvalid choice.";

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cin.get();
        }
    }
}



void mainMenu() //main menu
{
    cout << "======================================================\n";
    cout << "SMART HEALTHCARE & HOSPITAL PATIENT MANAGEMENT SYSTEM\n";
    cout << "======================================================\n\n";

    cout << "1. Datasets\n";
    cout << "2. Array\n";
    cout << "3. Singly Linked List\n";
    cout << "4. Healthcare Expenditure & Service Analysis\n";
    cout << "5. Sorting Experiments\n";
    cout << "6. Searching Experiments\n";
    cout << "7. Exit\n";

    cout << "\n======================================================\n";
    cout << "Enter choice: ";
}

void buildPatientArray(const Table& table, PatientArray& arr)
{
    for (int i = 1; i < table.rows; i++)
    {
        arr.add(rowToPatient(table.cells[i]));
    }
}


void buildPatientList(const Table& table, PatientList& list)
{
    for (int i = 1; i < table.rows; i++)
    {
        list.insertAtEnd(rowToPatient(table.cells[i]));
    }
}

int main()
{

string facilityAFile = "../datasets/dataset1 facility_a.csv";
string facilityBFile = "../datasets/dataset2 facility_b.csv";
string facilityCFile = "../datasets/dataset3 facility_c.csv";


    Table facilityA;
    Table facilityB;
    Table facilityC;

    readCSV(facilityAFile, facilityA);
    readCSV(facilityBFile, facilityB);
    readCSV(facilityCFile, facilityC);

    PatientArray patientArrA;
    PatientArray patientArrB;
    PatientArray patientArrC;

    buildPatientArray(facilityA, patientArrA);
    buildPatientArray(facilityB, patientArrB);
    buildPatientArray(facilityC, patientArrC);

    PatientList patientListA;
    PatientList patientListB;
    PatientList patientListC;

    buildPatientList(facilityA, patientListA);
    buildPatientList(facilityB, patientListB);
    buildPatientList(facilityC, patientListC);

    const string sortSearchOptions[2] = { "Array", "Singly Linked List" };


    int choice; //variable for the menu choice

    while (true)
    {
        clearScreen();

        mainMenu();

        cin >> choice;

        if (choice == 1)
        {
            datasetsMenu(
                facilityA,
                facilityB,
                facilityC
            );
        }


        else if (choice == 2)
        {
            arrayMenu(
                patientArrA,
                patientArrB,
                patientArrC
            );
        }


        else if (choice == 3)
        {
            linkedListMenu(
                patientListA,
                patientListB,
                patientListC
            );
        }


        else if (choice == 4)
        {
            healthcareMenu(
            patientArrA, patientArrB, patientArrC,
            patientListA, patientListB, patientListC
            );
        }


        else if (choice == 5)
        {
            PatientSorting::menu(
                patientArrA, patientArrB, patientArrC,
                patientListA, patientListB, patientListC
            );
        }

        else if (choice == 6)
        {
            SearchingMenu::menu(
                patientArrA,
                patientArrB,
                patientArrC,
                patientListA,
                patientListB,
                patientListC);
        }

        else if (choice == 7)
        {
            clearScreen();

            cout << "========================================\n";
            cout << "       Thank you for using the program\n";
            cout << "========================================\n";

            break;
        }



        else //exception handling for invalid input
        {
            cout << "\nInvalid choice. Please enter 1-7.";

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cin.get();
        }
    }

    return 0;
}