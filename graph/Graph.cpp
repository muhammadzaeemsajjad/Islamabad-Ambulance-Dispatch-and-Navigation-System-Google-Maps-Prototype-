// ============================================================================
//  Graph.cpp
//  The city road network.
// ============================================================================

#pragma once
#include "../data_structures/IncidentBST.cpp"
#include "../data_structures/TrafficBST.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> // Added include for std::string

using namespace std;

// ── Edge ────────────────────────────────────────────────────────────────────
// Represents a single outgoing road in a node's adjacency linked list.
struct Edge {
    int toNode;         // target Node
    float distance;
    Edge* next;         // pointer tot he next road in adjacency list

    Edge(int t, float d) {
        this->toNode = t;
        this->distance = d;
        this->next = nullptr;
    }
};

// ── GraphNode ───────────────────────────────────────────────────────────────
// Represents a single intersection in the city.
class GraphNode {
public:
    int id;
    string name; // Changed from char name[50] to std::string
    IncidentBST* incBST;       // BST that stores incident data  incident tree
    TrafficBST* trafficBST;     // BST that stores traffic data  traffic tree
    Edge* adjList;              // adjacency head. tracking outgoing roads

    GraphNode() {
        this->id = -1;
        this->incBST = nullptr;
        this->trafficBST = nullptr;
        this->adjList = nullptr;
        this->name = ""; // Clean string initialization
    }

    // Dynamic memory allocation for local data trees
    void init(int nodeID) {
        this->id = nodeID;
        this->name = ""; // Clean string initialization
        this->incBST = new IncidentBST();
        this->trafficBST = new TrafficBST();
    }

    // Prepends a new edge to the front of the linked list — O(1) complexity
    void addEdge(int to, float dist) {
        Edge* e = new Edge(to, dist);    //temporary pointer used when creating a new road
        e->next = this->adjList;
        this->adjList = e;
    }

    // Returns true if this intersection connects to at least one road
    bool hasEdges()  {
        return this->adjList != nullptr;
    }
};

// ── CityGraph ───────────────────────────────────────────────────────────────
// Manages the entire city map made up of nodes and edges.
class CityGraph {
private:
    GraphNode* nodes;  // Array holding all the graph nodes
    int V;             //Total nodes in the graph

public:
    CityGraph(int numNodes) {
        this->V = numNodes;
        this->nodes = new GraphNode[V];

        for (int i = 0; i < V; i++) {
            nodes[i].init(i);
        }
    }

    // Connects two nodes together with a bidirectional road
    void addEdge(int from, int to, float dist) {
        // Out-of-bounds safety check
        if (from < 0 || from >= V || to < 0 || to >= V) {
            return;
        }

        // Add the connection in both directions
        nodes[from].addEdge(to, dist);
        nodes[to].addEdge(from, dist);
    }

    // Inserts a danger record into the node's Incident BST
    void insertIncident(int id, IncidentReport r) {
        if (id < 0 || id >= V) {
            return;
        }

        nodes[id].incBST->insert(r);
    }

    // Inserts a traffic speed/volume record into the node's Traffic BST
    void insertTraffic(int id, TrafficRecord r) {
        if (id < 0 || id >= V) {
            return;
        }

        nodes[id].trafficBST->insert(r);
    }

    // Safely returns a reference pointer to an explicit node
    GraphNode* getNode(int id) {
        if (id < 0 || id >= V) {
            return nullptr;
        }

        return &nodes[id];
    }

    // Read-only getter for graph size
    int getSize()  {
        return V;
    }

    // Read-only lookup for node text labels
    string getNodeName(int id) {
        if (id < 0 || id >= V) {
            return "Unknown";
        }

        return nodes[id].name;
    }

    // Validates that a node exists and has working exits
    // bool isValidConnected(int id) {
    //     if (id < 0 || id >= V) {
    //         return false;
    //     }
    //
    //     return nodes[id].hasEdges();
    // }

    // Parses road layouts and landmark labels from file map text
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "  [ERROR] Cannot open graph file: " << filename << "\n";
            cout << "  [HINT] Ensure the 'data' folder is in your current working directory.\n";
            return false;
        }

        string line;
        int edgeCount = 0;

        while (getline(file, line)) {
            // Ignore blank space or comment lines
            if (line.empty() || line[0] == '#') {
                continue;
            }

            istringstream ss(line);
            string type;
            ss >> type;

            if (type == "EDGE") {
                int from, to;
                float dist;
                ss >> from >> to >> dist;

                addEdge(from, to, dist);
                edgeCount++;
            }
            else if (type == "NODE") {
                int nodeID;
                string nm;
                ss >> nodeID >> nm;

                if (nodeID >= 0 && nodeID < V) {
                    // Replaced the old strncpy blocks with safe string assignment
                    nodes[nodeID].name = nm;
                }
            }
        }

        file.close();
        cout << "  [GRAPH] Loaded " << edgeCount << " edges ("
             << edgeCount * 2 << " directed) into " << V << " nodes.\n";
        return true;
    }

    // Parses accident records and files them to the appropriate spatial trees
    bool loadIncidents(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "  [ERROR] Cannot open incidents file: " << filename << "\n";
            cout << "  [HINT] Ensure the 'data' folder is in your current working directory.\n";
            return false;
        }

        string line;
        int count = 0;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            istringstream ss(line);
            string type;
            ss >> type;

            if (type == "INCIDENT") {
                int nodeID;
                IncidentReport r;

                ss >> nodeID >> r.incidentID >> r.timestamp
                   >> r.severity >> r.description;

                insertIncident(nodeID, r);
                count++;
            }
        }

        file.close();
        cout << "  [INCIDENTS] Loaded " << count << " incident reports.\n";
        return true;
    }

    // Parses automated traffic sensor inputs from file map text
    bool loadTraffic(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "  [ERROR] Cannot open traffic file: " << filename << "\n";
            cout << "  [HINT] Ensure the 'data' folder is in your current working directory.\n";
            return false;
        }

        string line;
        int count = 0;

        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            istringstream ss(line);
            string type;
            ss >> type;

            if (type == "TRAFFIC") {
                int nodeID;
                TrafficRecord r;

                ss >> nodeID >> r.timestamp >> r.vehicleCount
                   >> r.congestionMult >> r.source;

                insertTraffic(nodeID, r);
                count++;
            }
        }

        file.close();
        cout << "  [TRAFFIC] Loaded " << count << " traffic congestion updates.\n";
        return true;
    }
};