#ifndef PATIENT_ARRAY_HPP
#define PATIENT_ARRAY_HPP

#include "../main/Patient.hpp"

class PatientArray
{
private:

    Patient* data;
    int size;
    int capacity;

public:

    PatientArray()
    {
        capacity = 10;
        size = 0;
        data = new Patient[capacity];
    }

    PatientArray(const PatientArray&) = delete;
    PatientArray& operator=(const PatientArray&) = delete;

    ~PatientArray()
    {
        delete[] data;
    }

    void add(const Patient& p) // adds a patient, growing the array if full
    {
        if (size == capacity)
        {
            capacity *= 2;

            Patient* newData = new Patient[capacity];

            for (int i = 0; i < size; i++)
            {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
        }

        data[size] = p;
        size++;
    }

    Patient get(int index) const
    {
        return data[index];
    }

    void set(int index, const Patient& p)
    {
        data[index] = p;
    }

    int getSize() const
    {
        return size;
    }
};

#endif