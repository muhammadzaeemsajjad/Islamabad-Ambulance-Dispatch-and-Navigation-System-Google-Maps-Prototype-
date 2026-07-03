# 🚑 Islamabad Ambulance Dispatch & Navigation System
### 📍 A High-Fidelity Google Maps Prototype for Emergency Routing in Islamabad
---

## 📖 Project Overview
Developed as a core **Data Structures (SP26)** project at **COMSATS University Islamabad**, this system is an end-to-end, console-based intelligent emergency routing and ambulance dispatch simulation. It maps **50 prominent locations across Islamabad** and dynamically optimizes travel paths during critical medical emergencies.

To demonstrate a rigorous understanding of low-level software engineering, **the Standard Template Library (STL) is completely omitted**. Every data structure—from adjacency lists and priority queues to Binary Search Trees and circular buffers—is written entirely from scratch in pure C++.

---

## 🏗️ Technical Architecture & Key Features

### 1. 🔀 Hazard & Traffic-Aware Routing Architecture
The core navigation engine implements a multi-criteria cost optimization function layered over a custom **Dijkstra's Algorithm** (using an explicit Min-Heap). Instead of relying on static physical distances, edge weights are calculated dynamically using:
$$\text{Total Travel Cost} = (\text{Base Distance} \times \text{Congestion Multiplier}) + (\text{Danger Score} \times \text{Risk Multiplier})$$

* **Dynamic Congestion Multipliers:** Pulled in real-time from active node traffic reports.
* **Live Hazard Penalties:** Evaluates active incidents (protests, accidents, blockades) to penalize dangerous routes and redirect ambulances through safer paths.
* **Search Space Visualization:** Tracks and outputs the full internal traversal path and cost progression of the search algorithm.

### 2. 🗂️ The 4-Layer Nested Data Infrastructure
Data is isolated and mapped using a clean, layered architectural chain:
1.  **CityGraph (Graph Layer):** Array-backed adjacency list of 50 major Islamabad nodes.
2.  **GraphNode (Node Layer):** Each node contains local descriptive metrics and holds dedicated query models.
3.  **BST Layer (`IncidentBST` & `TrafficBST`):** High-performance Binary Search Trees sorted by Epoch timestamps that support lightning-fast regional queries.
4.  **LinkedList Layer (Data Storage):** Individual nodes inside the BST map out chronological linked chains of concrete logs.

### 3. 🚨 Double-Heap Triage & Streaming Updates
* **Patient Priority Queue (Min-Heap):** Triage system that ranks incoming patient calls based on medical acuity (Triage Category 1–5, arrival times, and response thresholds).
* **Active Danger Alerts (Max-Heap):** Prioritizes city-wide safety advisories based on severity scores, keeping dispatchers informed of active high-risk areas.
* **Live News Ticker (Circular Queue):** A FIFO ring buffer that scrolls through the latest dispatch updates and active traffic/incident logs.

---

## 🗺️ Represented Islamabad Locations (50 Nodes)
The road network represents a carefully mapped topological model of Islamabad, featuring:
* **Command Base (Node 0):** **PIMS Hospital** (The central dispatch base where ambulances originate and return).
* **Sectors & Markazes:** G-8, G-9, F-10, F-8, F-7, G-10, F-11, I-8, G-11, Blue Area, F-6 Supermarket, Aabpara, I-9, G-6, E-7, E-11, I-10, D-12, E-12, H-8, H-9, H-11.
* **Major Gateways & Points:** Faizabad Interchange, Zero Point, Tarnol Interchange, Rawalpindi Saddar, Bahria Town Phase 1, PWD Colony, Bhara Kahu, Saidpur Village, Pakistan Secretariat, Shakarparian, Golra Mor, Koral Chowk, Margalla Road, Diplomatic Enclave, and more.

---

## 📂 File Directory Structure
```text
Project_FINAL_1/
├── main.cpp                     # System coordinator, initialization, and application loops
├── README.md                    # System documentation
├── data/                        # Static text databases for initial hydration
│   ├── graph.txt                # 50-node road network topology & distances
│   ├── incidents.txt            # Pre-loaded city incident records
│   └── traffic.txt              # Pre-loaded traffic density points
├── data_structures/             # Custom low-level data structure implementations
│   ├── DataTypes.cpp            # Core entities (Patient, TrafficLog, IncidentLog, Edges)
│   ├── LinkedList.cpp           # Linked list wrapper used across structures
│   ├── IncidentBST.cpp          # Timestamp-indexed BST for active emergency events
│   ├── TrafficBST.cpp           # Timestamp-indexed BST for dynamic road congestion
│   ├── MinHeap.cpp              # Priority Queue for patient triage and Dijkstra expansion
│   ├── MaxHeap.cpp              # Severity-ordered heap for live critical warnings
│   └── Queue.cpp                # Circular ring buffer handling the news ticker feed
├── graph/
│   └── Graph.cpp                # Adjacency list based city-wide route framework
├── navigation/
│   └── NavigationEngine.cpp     # Dynamic cost computation and Augmented Dijkstra execution
├── dispatch/
│   ├── AmbulanceFleet.cpp       # Tracking state, availability, and paths of responders
│   └── DispatchSim.cpp          # State machine linking patients to top-tier dispatchers
└── ui/
    └── Menu.cpp                 # Terminal interface rendering layouts, headers, and UI
```

---

## 🕹️ Interactive System Commands
The application provides a comprehensive 14-point control console:
1.  **Find Danger+Traffic-Aware Shortest Path:** Run the custom Dijkstra engine between any two points.
2.  **Register Patient Emergency Call:** Queue incoming medical incidents by category.
3.  **View Patient Call Queue:** View the current patient priority queue (Min-Heap representation).
4.  **Dispatch Ambulance:** Command an available unit at PIMS base to route to a patient and return.
5.  **Report New Incident / 6. Report Traffic Congestion:** Dynamically update the city map mid-execution.
6.  **View Active Danger Alerts:** Inspect current high-priority urban hazards.
7.  **View News Ticker:** Displays rolling event telemetry logs.
8.  **View All Incidents / 10. View Traffic Data:** In-order traversal of data BSTs.
9.  **View Dijkstra Search Space:** Prints exact tracking metrics for the last path calculated.
10. **Adjust Risk Multiplier:** Tune system sensitivity to active hazard zones.
11. **Run Full System Demo:** Executes an automated test scenario demonstrating triage, routing, and resolution.

---

## ⚡ Quick Start & Compilation

Ensure you have a modern C++ compiler (`g++` supporting C++11 or higher) installed on your system.

### Compiling via Terminal
Navigate to the root directory and build using the following command:
```bash
g++ main.cpp -o AmbulanceSystem
```

### Running the System
```bash
./AmbulanceSystem
```

---

## 👥 Project Contributors
* **Group Members (BSE - SP25):** * `M.Zaeem Sajjad` | `Aneeq` | `Shahzil` | `Zaryab`
* **Institution:** Department of Computer Science, COMSATS University Islamabad.

