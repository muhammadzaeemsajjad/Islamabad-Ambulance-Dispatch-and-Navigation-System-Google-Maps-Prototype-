
#pragma once
#include "../graph/Graph.cpp"
#include "../data_structures/MinHeap.cpp"
#include "../data_structures/MaxHeap.cpp"
#include "../data_structures/Queue.cpp"
#include <iostream>
#include <ctime>
using namespace std;

class NavigationEngine{
    CityGraph *graph;
    MaxHeap *alertHeap;
    CircularQueue *ticker;
    float riskMult;

    static const int MAX_EVAL = 2000;
    int evalNode[MAX_EVAL];
    float evalCost[MAX_EVAL];
    int evalCount;

    int finalPathNodes[200];
    int finalPathLen = 0;
public:
    NavigationEngine(CityGraph *g, float rm){
        this->graph = g;
        this->riskMult = rm;
        this->evalCount = 0;
        this->alertHeap = new MaxHeap(15);
        this->ticker = new CircularQueue();
    }

    // smart cost formula  W' = (baseDist x congestion) + (dangerScore x riskMult)
    float computeEdgeCost(int toID, float baseDist){
        GraphNode *node = graph->getNode(toID); // this is the target node
        if (!node){
            return baseDist;
        }

        long now = (long)time(nullptr);
        long lo = now - 86400;
        long hi = now + 3600;

        int dangerScore = node->incBST->querySeverity(lo, hi);
        float congestionMultiplier = node->trafficBST->queryCongestion(lo, hi);

        float travelCost = baseDist * congestionMultiplier;
        float hazardPenalty = (float)dangerScore * riskMult;
        return travelCost + hazardPenalty;
    }

    bool runDijkstra(int src, int dst){

        // Step 2: Initialize clean algorithm state memories
        int V = graph->getSize(); // v = total nodes
        float *dist = new float[V];
        int *prev = new int[V];
        bool *vis = new bool[V];
        evalCount = 0;

        for (int i = 0; i < V; i++){
            dist[i] = 3.4e+38f; // Standard infinity approximation placeholder
            prev[i] = -1;
            vis[i] = false;
        }

        // Step 3: Seed initial path tracking values for the source node
        dist[src] = 0.0f;
        MinHeap pq(V * 6);
        pq.insert(HeapNode(src, 0.0f));

        // Step 4: Primary Graph Traversal Loop
        while (!pq.isEmpty()){
            HeapNode cur = pq.extractMin();
            int currentNode = cur.nodeID;

            // Skip handling if this intersection was processed via an alternative shorter route
            if (vis[currentNode])
                continue;
            vis[currentNode] = true;


            if (evalCount < MAX_EVAL){
                evalNode[evalCount] = currentNode;
                evalCost[evalCount] = dist[currentNode];

                evalCount++;
            }

            // Optimization target break check
            if (currentNode == dst){
                break;
            }
            
            // Step 5: Process and evaluate all neighboring outbound road links
            Edge *edgeLink = graph->getNode(currentNode)->adjList;
            // CRITICAL SAFETY CHECK: If the intersection has no outgoing roads, skip it!

            while (edgeLink){
                int v = edgeLink->toNode;
                if (!vis[v]){
                    // Pull weights via environmental parameters mapping tracking
                    float edgeCost = computeEdgeCost(v, edgeLink->distance);
                    float newDist = dist[currentNode] + edgeCost;


                    // Relaxation rule phase logic
                    if (newDist < dist[v]){
                        dist[v] = newDist;
                        prev[v] = currentNode;
                        pq.insert(HeapNode(v, newDist));
                    }
                }
                edgeLink = edgeLink->next;
            }
        }

        // Step 6: Backtrack the target path link chain and present outputs
        bool found = (dist[dst] < 3.4e+38f);

        if (found){
            // Build path array by backtracking prev[]
            int path[200];
            int pathLen = 0;
            int cur = dst;

            // Backtrack path trace link collection
            while (cur != -1 ){ // Safety ceiling guard rail
                path[pathLen++] = cur;
                cur = prev[cur];
            }

            // 📍 ADD THIS BLOCK HERE TO SAVE COPIES GLOBALLY FOR THE SEARCH SPACE:
            finalPathLen = 0;
            for (int i = 0; i < pathLen; i++) {
                finalPathNodes[finalPathLen++] = path[i];
            }

            cout << "  [RESULT] Path cost (dist + danger + traffic): " << dist[dst] << " units\n";
            cout << "  Route: " << graph->getNodeName(path[pathLen - 1]) << "\n";

            for (int i = pathLen - 2; i >= 0; i--){
                cout << "       -> " << graph->getNodeName(path[i]) << "\n";
            }
        }
        else{
            cout << "  [RESULT] No reachable path from "
                 << graph->getNodeName(src) << " to " << graph->getNodeName(dst) << ".\n";
        }

        delete[] dist;
        delete[] prev;
        delete[] vis;
        return found;
    }

    // Logging injection tool tracking emergency hazard additions
    void insertIncident(int nodeID, IncidentReport r){
        graph->insertIncident(nodeID, r);

        long now = (long)time(nullptr); // now = current time
        int danger = graph->getNode(nodeID)->incBST->querySeverity(now - 3600, now + 86400);
        string msg = string(graph->getNodeName(nodeID)) + ": " + r.description;

        alertHeap->insert(AlertNode(nodeID, (float)danger, msg, now + 3600));

        ticker->enqueue("INCIDENT @" + graph->getNodeName(nodeID) + " | " + r.description);
        cout << "  [INCIDENT] Logged at " << graph->getNodeName(nodeID)
             << " | Sev:" << r.severity << " | " << r.description << "\n";
    }

    // Logging injection tool tracking live vehicle volume shifts
    void insertTrafficReport(int nodeID, TrafficRecord r){
        graph->insertTraffic(nodeID, r);

        ticker->enqueue("TRAFFIC @" + graph->getNodeName(nodeID) +
                        " | Congestion:" + to_string(r.congestionMult) + "x");
        cout << "  [TRAFFIC] Logged at " << graph->getNodeName(nodeID)
             << " | VehiclesPM:" << r.vehicleCount
             << " | Congestion:" << r.congestionMult << "x\n";
    }

    void printSearchSpace(){
        cout << "\n   =========== DIJKSTRA SEARCH-SPACE ===========\n";
        cout << "  |  Nodes evaluated: " << evalCount << "                         |\n";
        cout << "  |  Step | ID  | Location                  | Cost    |\n";
        cout << "  |_______|_____|___________________________|_________|\n";

        for (int i = 0; i < evalCount && i < 50; i++){
            // 1. Print Step Number with padding
            cout << "  |  " << i + 1;
            if (i + 1 < 10) cout << "    | ";
            else cout << "   | ";

            // 2. Print Node ID with padding
            int currentID = evalNode[i];
            cout << currentID;
            if (currentID < 10) cout << "   | ";
            else cout << "  | ";

            // 3. Print Location Name with padding
            string nm = graph->getNodeName(currentID);
            cout << nm;
            int pad = 26 - (int)nm.size();
            for (int p = 0; p < pad; p++) cout << " ";

            // 4. Print Calculated Step Cost
            cout << "| " << evalCost[i];

            // 5. Check if part of the final path matrix mapping
            bool isSelected = false;
            for (int p = 0; p < finalPathLen; p++) {
                if (finalPathNodes[p] == currentID) {
                    isSelected = true;
                    break;
                }
            }

            // 6. Print trailing star modifier for selected nodes
            if (isSelected) {
                cout << " *\n";
            } else {
                cout << " \n";
            }
        }
        cout << "  |_______|_____|___________________________|_________|";
    }

    void printAlerts(){
        alertHeap->display(graph);
    }
    void printTicker(){
        ticker->displayAll();
    }
    void addTicker(const string &m){
        ticker->enqueue(m);
    }

    void setRiskMult(float r){
        if (r < 0.1f){
            riskMult = 0.1f;
        }
        else if (r > 10.0f){
            riskMult = 10.0f;
        }
        else{
            riskMult = r;
        }
    }
    float getRiskMult(){
        return riskMult;
    }

    void printAllIncidents(){
        cout << "\n  ======================= ALL INCIDENTS - BST IN-ORDER ========================\n";
        for (int i = 0; i < graph->getSize(); i++){
            GraphNode *n = graph->getNode(i);
            if (!n->incBST->empty()){
                cout << "  |  Node " << i << " [" << graph->getNodeName(i) << "]:\n";
                // cout<<"  |      ";
                n->incBST->printAll();
                cout<<"  |\n";
            }
        }
        cout << "  |___________________________________________________________________________|";
    }

    void printAllTraffic(){
        cout << "\n  ============ ALL TRAFFIC DATA - BST IN-ORDER ============\n";
        for (int i = 0; i < graph->getSize(); i++){
            GraphNode *n = graph->getNode(i);

            if (!n->trafficBST->empty()){
                cout << "  |  Node " << i << " [" << graph->getNodeName(i) << "]: \n";
                cout<<"  |          ";
                n->trafficBST->printAll();
                cout<<"  |\n";
            }
        }
        cout << "  |_______________________________________________________|";
    }

    CityGraph *getGraph(){
        return graph;
    }
};