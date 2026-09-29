#ifndef PATIENT_LIST_HPP
#define PATIENT_LIST_HPP

#include "../main/Patient.hpp"

struct PatientNode
{
    Patient data;
    PatientNode* next;

    PatientNode(Patient p)
    {
        data = p;
        next = nullptr;
    }
};

class PatientList
{
private:

    PatientNode* head;
    PatientNode* tail;
    int size;

public:

    PatientList()
    {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~PatientList()
    {
        PatientNode* current = head;

        while (current != nullptr)
        {
            PatientNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insertAtEnd(Patient p) // adds a patient to the end of the list
    {
        PatientNode* newNode = new PatientNode(p);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        size++;
    }

    PatientNode* getHead() const
    {
        return head;
    }

    int getSize() const
    {
        return size;
    }
};

#endif