# 🚀 Space Debris Tracker

A simulation-based **C + Data Structures & Algorithms (DSA)** project that helps monitor, analyze, and manage space debris in orbit. This project demonstrates how multiple data structures can work together in a real-world problem-solving scenario.

---
AUTHOR
ALLU SAI GANESH-AP24110010154



## 📌 Overview

Space debris is a growing issue for satellites and space missions. This project tracks debris objects, calculates risk levels, detects collisions, and performs orbital simulations.

The system is developed using **C programming language** and implements important DSA concepts.

---

## ✨ Features

### 1️⃣ Add Debris Object
- Enter:
  - Debris ID
  - Velocity
  - Altitude
  - Distance
- Automatically calculates risk factor.

### 2️⃣ Display All Debris
Shows all stored debris data including:
- ID
- Velocity
- Altitude
- Distance
- Risk Level

### 3️⃣ Highest Risk Detection
Uses **Max Heap** to quickly find the most dangerous debris.

### 4️⃣ Spatial Search
Uses **KD Tree** to locate debris near:
- Specific altitude
- Specific distance

### 5️⃣ Orbit Simulation
Uses **Circular Queue** to simulate debris movement step-by-step.

### 6️⃣ Collision Detection
Checks possible collisions based on:
- Distance difference < 500 km
- Velocity difference < 0.5 km/s

---

## 🧠 Data Structures Used

| Data Structure | Purpose |
|--------------|---------|
| Linked List | Dynamic storage of debris |
| Hash Table | Fast search by debris ID |
| Max Heap | Highest risk debris retrieval |
| KD Tree | Spatial searching |
| Circular Queue | Orbit simulation |

---

## 📐 Risk Formula

```text
Risk = (velocity²) / (altitude × distance)
