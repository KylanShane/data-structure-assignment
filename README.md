# data-structure-assignment

# 🏥 Smart Healthcare & Hospital Patient Management System

A custom C++ application developed to compare the performance, execution time, and memory overhead of **Arrays** versus **Singly Linked Lists** across sorting and searching algorithms. The system processes and analyzes patient registration and clinical datasets across three distinct healthcare facilities to provide actionable demographic and expenditure insights.

> **Course:** Data Structures and Algorithms (CT077-3-2-DSTR) — Lab Evaluation & Solution Work  
> **Institution:** Asia Pacific University of Technology & Innovation (APU)  
> **Language:** C++11 (Built without STL containers like `std::vector` or `std::list`)

---

## 📌 Project Overview

MetroHealth System is experiencing a surge in daily patient visits across its network. This application analyzes patient flow and healthcare resource allocation by processing three facility datasets:
* **Facility A (General Hospital):** Adult & elderly patients using Emergency, Inpatient, and Outpatient care.
* **Facility B (University Medical Center):** Students & young adults using Rehabilitation and Vaccination services.
* **Facility C (Community Health Clinic):** Residents of all ages utilizing Routine Checkups.

The system provides two separate execution models—one built strictly using **Dynamic Arrays** and the other using **Singly Linked Lists**—to conduct performance benchmark comparisons.

---

## 🛠️ Data Structures & Algorithms Implemented

### 1. Data Structures[cite: 1]
* **Singly Linked List (`Node* head`):** Custom-implemented linked structure providing $O(1)$ dynamic insertion at head/tail without pre-allocating memory[cite: 1].
* **Dynamic Array (`Patient*`):** Contiguous memory layout providing $O(1)$ index-based access, used to evaluate cache locality and fixed-size performance[cite: 1].

### 2. Sorting & Searching Algorithms[cite: 1]
* **Sorting Algorithms:** Custom implementations (e.g., Quick Sort / Merge Sort / Bubble Sort) to rank patients by **Age**, **Visit Duration (Length of Stay)**, or **Total Medical Cost**[cite: 1].
* **Searching Algorithms:** 
  * **Linear Search:** Used on unsorted datasets to filter by Age Group, Care Type, or Length of Stay thresholds[cite: 1].
  * **Binary Search:** Implemented on sorted data to achieve $O(\log n)$ lookup times[cite: 1].

---

## 📊 Demographic & Expenditure Analytics

The system categorizes patient data into 5 distinct demographic groups[cite: 1]:
1. **0–17:** Pediatrics & Adolescents[cite: 1]
2. **18–25:** Young Adults / University Students[cite: 1]
3. **26–45:** Working Adults (Early Career)[cite: 1]
4. **46–60:** Working Adults (Late Career)[cite: 1]
5. **61–100:** Senior Citizens / Geriatric Care[cite: 1]

### Analytical Features:
* **Cost Calculation:** Formula applied per record:  
  $$\text{Total Cost (MYR)} = \text{Length of Stay (Hours)} \times \text{Base Cost Per Hour} \times \text{Days Visits Per Year}$$
[cite: 1]
* **Care Type Preference:** Identifies the most requested care services per demographic[cite: 1].
* **Expenditure Metrics:** Calculates average medical cost per patient and total billing per facility[cite: 1].

---
