

#include "data_structures/DataTypes.cpp"
#include "data_structures/LinkedList.cpp"
#include "data_structures/IncidentBST.cpp"
#include "data_structures/TrafficBST.cpp"
#include "graph/Graph.cpp"
#include "data_structures/MinHeap.cpp"
#include "data_structures/MaxHeap.cpp"
#include "data_structures/Queue.cpp"
#include "navigation/NavigationEngine.cpp"
#include "dispatch/AmbulanceFleet.cpp"
#include "dispatch/DispatchSim.cpp"
#include "ui/Menu.cpp"

#include <iostream>
#include <limits>
#include <ctime>
using namespace std;

// ============================================================
//  Forward declarations
// ============================================================
void handleFindPath       (NavigationEngine&, CityGraph*);
void handleRegisterCall   (NavigationEngine&, PatientQueue&, CityGraph*);
void handleViewQueue      (PatientQueue&, CityGraph*);
void handleReportIncident (NavigationEngine&);
void handleReportTraffic  (NavigationEngine&);
void handleRiskMultiplier (NavigationEngine&);
void handleMapReference   (CityGraph*);
void handleFullDemo       (NavigationEngine&, PatientQueue&, CityGraph*);

// ============================================================
//  System initialization
// ============================================================
CityGraph* initCity() {
    cout << "\n  ------------Loading Islamabad city graph------------\n";
    CityGraph* city = new CityGraph(50);
    city->loadFromFile("data/graph.txt");
    city->loadIncidents("data/incidents.txt");
    city->loadTraffic("data/traffic.txt");
    return city;
}

NavigationEngine initEngine(CityGraph* city) {
    NavigationEngine engine(city, 2.0f);
    engine.addTicker("System online. PIMS Hospital dispatch active.");
    engine.addTicker("50 Islamabad nodes loaded. Risk multiplier: 2.0");
    return engine;
}

AmbulanceFleet initFleet() {
    AmbulanceFleet fleet;
    fleet.addAmbulance(3);
    fleet.addAmbulance(2);
    fleet.addAmbulance(1);
    cout << "  [FLEET] 3 ambulances ready at PIMS Hospital.\n";
    return fleet;
}

PatientQueue initQueue(NavigationEngine& engine, CityGraph* city) {
    PatientQueue queue(20);
    queue.addCall(25, 9, "Ali_Khan",   "Cardiac_arrest",      city);
    queue.addCall(40, 7, "Sara_Malik", "Road_accident_injury", city);
    engine.addTicker("2 pre-loaded patient calls in queue.");
    return queue;
}

// ============================================================
//  Menu handlers
// ============================================================
void handleFindPath(NavigationEngine& engine, CityGraph* city) {
    int s, d;
    cout << "\n  Source node      (0-49): "; cin >> s;
    cout << "  Destination node (0-49): "; cin >> d;

    if (s < 0 || s > 49 || d < 0 || d > 49 || s == d) {
        cout << "  [ERROR] Invalid nodes.\n";
    } else {
        cout << "\n  " << city->getNodeName(s) << " to " << city->getNodeName(d) << "\n";
        engine.runDijkstra(s, d);
    }
    pressEnter();
}

void handleRegisterCall(NavigationEngine& engine, PatientQueue& queue, CityGraph* city) {
    int node, urgency;
    string name, condition;

    cout << "\n  Patient location node (0-49): "; cin >> node;
    if (node < 0 || node > 49) {
        cout << "  [ERROR] Invalid node.\n"; pressEnter(); return;
    }
    cout << "  Patient name (no spaces):     "; cin >> name;
    cout << "  Condition (no spaces):        "; cin >> condition;
    cout << "  Urgency (1-10):               "; cin >> urgency;

    queue.addCall(node, urgency, name, condition, city);
    engine.addTicker("CALL: " + name + " at " + string(city->getNodeName(node)) +
                     " | Urgency:" + to_string(urgency));
    pressEnter();
}

void handleViewQueue(PatientQueue& queue, CityGraph* city) {
    queue.displayAll(city);
    pressEnter();
}

void handleReportIncident(NavigationEngine& engine) {
    int nodeID, sev;
    string id, desc;

    cout << "\n  Node (1-49):               "; cin >> nodeID;
    if (nodeID < 1 || nodeID > 49) {
        cout << "  [ERROR] Invalid node.\n"; pressEnter(); return;
    }
    cout << "  Incident ID (e.g. INC-X): "; cin >> id;
    cout << "  Severity (1-10):           "; cin >> sev;
    if (sev < 1 || sev > 10) sev = 5;
    cout << "  Description (no spaces):   "; cin >> desc;

    IncidentReport r;
    r.incidentID  = id;
    r.timestamp   = (long)time(nullptr);
    r.severity    = sev;
    r.description = desc;
    engine.insertIncident(nodeID, r);
    pressEnter();
}

void handleReportTraffic(NavigationEngine& engine) {
    int   nodeID, vpm;
    float mult;

    cout << "\n  Node (1-49):                                   "; cin >> nodeID;
    if (nodeID < 1 || nodeID > 49) {
        cout << "  [ERROR] Invalid node.\n"; pressEnter(); return;
    }
    cout << "  Vehicles/minute observed:                      "; cin >> vpm;
    cout << "  Congestion multiplier (1.0=clear, 3.0=heavy): "; cin >> mult;
    mult = max(1.0f, min(5.0f, mult));

    TrafficRecord tr;
    tr.timestamp      = (long)time(nullptr);
    tr.vehicleCount   = vpm;
    tr.congestionMult = mult;
    tr.source         = "user_report";
    engine.insertTrafficReport(nodeID, tr);
    pressEnter();
}

void handleRiskMultiplier(NavigationEngine& engine) {
    float r;
    cout << "\n  Current: " << engine.getRiskMult() << "\n";
    cout << "  New value (0.1-10.0): "; cin >> r;
    engine.setRiskMult(r);
    cout << "  [OK] Risk multiplier set to " << engine.getRiskMult() << "\n";
    engine.addTicker("Risk multiplier changed to " + to_string(engine.getRiskMult()));
    pressEnter();
}

void handleMapReference(CityGraph* city) {
    cout << "\n  ================= ISLAMABAD NODE MAP ==================\n";
    for (int i = 0; i < 50; i++)
        cout << "  |  [" << i << "] " << city->getNodeName(i) << "\n";
    cout << "  |____________________________________________________|\n";
    pressEnter();
}

void handleFullDemo(NavigationEngine& engine, PatientQueue& queue, CityGraph* city) {
    long now = (long)time(nullptr);

    cout << "\n==================================\n";
    cout << "         FULL SYSTEM DEMO\n";
    cout << "==================================\n";

    cout << "\n[1/6] Danger+Traffic path: PIMS to Attock_Bridge\n";
    engine.runDijkstra(0, 49);

    cout << "\n[2/6] Inserting live incidents...\n";
    engine.insertIncident(10, {"DEMO-01", now, 9,  "Power_line_down_Blue_Area"});
    engine.insertIncident(30, {"DEMO-02", now, 10, "Bridge_CRITICAL_Secretariat"});
    engine.insertIncident(25, {"DEMO-03", now, 7,  "Gas_leak_Rawalpindi_Saddar"});

    cout << "\n[3/6] Traffic congestion reports...\n";
    engine.insertTrafficReport(15, {now, 120, 3.2f, "sensor"});
    engine.insertTrafficReport(40, {now,  85, 2.1f, "sensor"});

    cout << "\n[4/6] Active danger alerts:\n";
    engine.printAlerts();

    cout << "\n[5/6] Patient calls:\n";
    queue.displayAll(city);

    cout << "\n[6/6] Recalculate with higher risk (4.0):\n";
    engine.setRiskMult(4.0f);
    engine.runDijkstra(0, 49);
    engine.setRiskMult(2.0f);

    engine.addTicker("Full demo complete. All features exercised.");
    engine.printTicker();

    cout << "\n==================================\n";
    cout << "           DEMO COMPLETE\n";
    cout << "==================================";
    pressEnter();
}

// ============================================================
//  Main
// ============================================================
int main() {
    printBanner();

    CityGraph* city = initCity();
    NavigationEngine engine = initEngine(city);
    AmbulanceFleet fleet = initFleet();
    PatientQueue queue = initQueue(engine, city);

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            cout<<"  [ERROR] Enter a number";
            continue;
        }

        switch (choice) {
            case  1: handleFindPath       (engine, city); break;
            case  2: handleRegisterCall   (engine, queue, city); break;
            case  3: handleViewQueue      (queue, city); break;
            case  4: cin.ignore(1000, '\n');
                     simulateAmbulanceDispatch(&engine, &queue, &fleet); break;
            case  5: handleReportIncident (engine); break;
            case  6: handleReportTraffic  (engine); break;
            case  7: engine.printAlerts(); pressEnter(); break;
            case  8: engine.printTicker(); pressEnter(); break;
            case  9: engine.printAllIncidents(); pressEnter(); break;
            case 10: engine.printAllTraffic();   pressEnter(); break;
            case 11: engine.printSearchSpace();  pressEnter(); break;
            case 12: handleRiskMultiplier (engine); break;
            case 13: handleMapReference   (city); break;
            case 14: handleFullDemo       (engine, queue, city); break;
            case  0: delete city;
                     cout << "  [DONE] Goodbye from PIMS Dispatch!\n\n"; break;
            default: cout << "  [ERROR] Invalid option. Enter 0-14.\n"; pressEnter();
        }
    }

    return 0;
}