#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include "Patient.hpp"
#include "../Array/PatientArray.hpp"

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


vector<vector<string>> readCSV(const string& filename) //dataset readers
{
    vector<vector<string>> data;

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Cannot open file: "
             << filename << endl;

        return data;
    }

    string line;

    while (getline(file, line))
    {
        vector<string> row;
        string value;

        stringstream ss(line);

        while (getline(ss, value, ','))
        {
            row.push_back(value);
        }

        data.push_back(row);
    }

    file.close();

    return data;
}



void displayData( //to display the data from the csv
    const vector<vector<string>>& data,
    const string& title)
{
    clearScreen();

    cout << "========================================\n";
    cout << "          " << title << "\n";
    cout << "========================================\n\n";

    if (data.empty())
    {
        cout << "No data found.\n";
    }
    else
    {
        for (const auto& row : data)
        {
            for (const auto& value : row)
            {
                cout << value << "\t";
            }

            cout << endl;
        }
    }

    pressEnterToContinue();
}



vector<vector<string>> combineData(  //to combine the data from the three facilities
    const vector<vector<string>>& A,
    const vector<vector<string>>& B,
    const vector<vector<string>>& C)
{
    vector<vector<string>> combined;

    for (const auto& row : A)
    {
        combined.push_back(row);
    }

    for (const auto& row : B)
    {
        combined.push_back(row);
    }

    for (const auto& row : C)
    {
        combined.push_back(row);
    }

    return combined;
}



void displayArray( //array section
    const vector<vector<string>>& data,
    const string& title)
{
    clearScreen();

    cout << "========================================\n";
    cout << "              " << title << "\n";
    cout << "========================================\n\n";

    if (data.empty())
    {
        cout << "No data found.\n";
    }
    else
    {
        for (const auto& row : data)
        {
            for (const auto& value : row)
            {
                cout << value << "\t";
            }

            cout << endl;
        }
    }

    pressEnterToContinue();
}


void arrayMenu( //array menu
    const vector<vector<string>>& A,
    const vector<vector<string>>& B,
    const vector<vector<string>>& C)
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
        cout << "4. All data combined\n";
        cout << "5. Back\n";

        cout << "\n========================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            cout << "\nFeature under development."; //add choice here for array
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        else if (choice == 5)
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



struct Node //linkedlist
{
    vector<string> data;
    Node* next;

    Node(vector<string> row)
    {
        data = row;
        next = nullptr;
    }
};


class SinglyLinkedList //linkedlist
{
private:

    Node* head;

public:

    SinglyLinkedList()
    {
        head = nullptr;
    }

    ~SinglyLinkedList()
    {
        clear();
    }



    void insert(vector<string> row) //insert function for the linked list
    {
        Node* newNode = new Node(row);

        if (head == nullptr)
        {
            head = newNode;
            return;
        }

        Node* current = head;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        current->next = newNode;
    }


    void display(const string& title) //the display function for the linked list
    {
        clearScreen();

        cout << "========================================\n";
        cout << "          " << title << "\n";
        cout << "========================================\n\n";

        if (head == nullptr)
        {
            cout << "No data found.\n";
        }
        else
        {
            Node* current = head;

            while (current != nullptr)
            {
                for (const auto& value : current->data)
                {
                    cout << value << "\t";
                }

                cout << endl;

                current = current->next;
            }
        }

        pressEnterToContinue();
    }


    void clear() //clear option
    {
        Node* current = head;

        while (current != nullptr)
        {
            Node* temp = current;

            current = current->next;

            delete temp;
        }

        head = nullptr;
    }
};


void linkedListMenu( //linkedlist menu
    const vector<vector<string>>& A,
    const vector<vector<string>>& B,
    const vector<vector<string>>& C)
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
        cout << "4. All data combined\n";
        cout << "5. Back\n";

        cout << "\n========================================\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            cout << "\nFeature under development."; //add choice here for linked list
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        else if (choice == 5)
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
    cout << "========================================\n";
    cout << "       FACILITY DATA MANAGEMENT\n";
    cout << "========================================\n\n";

    cout << "1. Facility A dataset\n";
    cout << "2. Facility B dataset\n";
    cout << "3. Facility C dataset\n";
    cout << "4. All data combined\n";
    cout << "5. Arrays\n";
    cout << "6. SinglyLinkedList\n";
    cout << "7. Exit\n";

    cout << "\n========================================\n";
    cout << "Enter choice: ";
}



int main()
{

string facilityAFile = "../datasets/dataset1 facility_a.csv";
string facilityBFile = "../datasets/dataset2 facility_b.csv";
string facilityCFile = "../datasets/dataset3 facility_c.csv";


    vector<vector<string>> facilityA =
        readCSV(facilityAFile);

    vector<vector<string>> facilityB =
        readCSV(facilityBFile);

    vector<vector<string>> facilityC =
        readCSV(facilityCFile);


    int choice; //variable for the menu choice

    while (true)
    {
        clearScreen();

        mainMenu();

        cin >> choice;

        if (choice == 1)
        {
            displayData(
                facilityA,
                "FACILITY A DATASET"
            );
        }

        else if (choice == 2)
        {
            displayData(
                facilityB,
                "FACILITY B DATASET"
            );
        }

        else if (choice == 3)
        {
            displayData(
                facilityC,
                "FACILITY C DATASET"
            );
        }


        else if (choice == 4)
        {
            vector<vector<string>> combined =
                combineData(
                    facilityA,
                    facilityB,
                    facilityC
                );

            displayData(
                combined,
                "ALL DATA COMBINED"
            );
        }


        else if (choice == 5)
        {
            arrayMenu(
                facilityA,
                facilityB,
                facilityC
            );
        }


        else if (choice == 6)
        {
            linkedListMenu(
                facilityA,
                facilityB,
                facilityC
            );
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