// ============================================================================
//  DispatchSim.cpp
//  Ambulance dispatch simulation.
//  Pulls highest-urgency patient from PatientQueue,
//  finds an available ambulance from the fleet,
//  calls NavigationEngine to route PIMS → Patient,
//  then automatically returns the ambulance to PIMS.
// ============================================================================

#pragma once
#include "AmbulanceFleet.cpp"
#include "../navigation/NavigationEngine.cpp"
#include "../data_structures/Queue.cpp"
#include "../ui/Menu.cpp"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// Dispatch: route PIMS -> patient location
void dispatchAmbulance(NavigationEngine *engine, Ambulance *amb, PatientCall &call) {
    // Replaced const char* with standard string safely
    string destinationName = engine->getGraph()->getNodeName(call.patientNode);

    cout << "\n  ________________________________________________\n";
    cout << "  DISPATCHING AMBULANCE-" << amb->ambID
         << " to " << call.patientName
         << " at " << destinationName << "\n";
    cout << "  ________________________________________________\n";

    cout << "\n  [ROUTE] PIMS Hospital to Patient Location:\n";

    // Run pathfinding from PIMS Base (Node 0) to patient location
    bool ok = engine->runDijkstra(0, call.patientNode);

    if (!ok) {
        cout << "  [DISPATCH FAILED] Cannot reach patient location.\n";

        // Cleaned up explicit string constructor casts since destinationName is now a string
        string tickerMsg = "DISPATCH FAILED: " + call.patientName + " unreachable at " + destinationName;
        engine->addTicker(tickerMsg);
        return;
    }

    // Update vehicle tracking variables to "En Route" state
    amb->available = false;
    amb->assignedPatient = call.callID;
    amb->destinationNode = call.patientNode;
    amb->status = "EN_ROUTE_TO_PATIENT";

    string tickerSuccess = "AMBULANCE-" + to_string(amb->ambID) + " dispatched to " + call.patientName + " at " + destinationName;
    engine->addTicker(tickerSuccess);

    cout << "\n  [STATUS] AMBULANCE-" << amb->ambID << " en route.\n";
}

// Return: route currentNode -> PIMS, mark ambulance idle
void returnAmbulance(NavigationEngine *engine, Ambulance *amb) {
    // Replaced const char* with standard string safely
    string currentLocName = engine->getGraph()->getNodeName(amb->currentNode);

    cout << "\n  [RETURN] AMBULANCE-" << amb->ambID
         << " returning to PIMS from "
         << currentLocName << "\n";

    // Run pathfinding from patient location back to PIMS Base (Node 0)
    engine->runDijkstra(amb->currentNode, 0);

    // Reset vehicle tracking variables back to original baseline "Idle" state
    amb->available = true;
    amb->currentNode = 0;
    amb->assignedPatient = -1;
    amb->destinationNode = -1;
    amb->status = "IDLE";

    engine->addTicker("AMBULANCE-" + to_string(amb->ambID) + " returned to PIMS. IDLE.");
}

// Full dispatch simulation — pulls from queue, picks ambulance, routes, optional return
void simulateAmbulanceDispatch(NavigationEngine *engine, PatientQueue *pq, AmbulanceFleet *fleet) {
    cout << "\n  =====================================================\n";
    cout << "            AMBULANCE DISPATCH SIMULATION\n";
    cout << "            Base: PIMS Hospital (Node 0)\n";
    cout << "  =====================================================\n";

    // Step 1: Safety check to ensure there are patients waiting
    if (pq->isEmpty()) {
        cout << "\n  [QUEUE EMPTY] No pending patient calls.";
        pressEnter();
        return;
    }

    // Step 2: Print queue snapshot to console screen
    pq->displayAll(engine->getGraph());

    cout << "\n  [DISPATCH] Processing urgent call...\n";

    // Step 3: Extract the patient with the highest medical priority ranking
    PatientCall call = pq->extractNext();
    
    // Step 4: Look for an available ambulance vehicle inside the garage
    Ambulance *amb = fleet->getAvailable();
    
    // if (!amb) {
    //     cout << "  [ERROR] All ambulances currently deployed!\n";
    //     cout << "  Returning patient call to queue.\n";
    //
    //     // Re-insert patient call back into tree to prevent drop-loss
    //     pq->addCall(call.patientNode, call.urgency, call.patientName, call.condition, engine->getGraph());
    //
    //     pressEnter();
    //     return;
    // }

    // Step 5: Execute travel from base out to scene
    dispatchAmbulance(engine, amb, call);

    // Step 6: Automatically simulate collection phase and run return trip
    amb->currentNode = call.patientNode;
    amb->status = "RETURNING";

    cout << "\n  [PICKUP] Patient collected. Returning to PIMS...\n";

    returnAmbulance(engine, amb);

    cout << "  AMBULANCE-" << amb->ambID << " back at PIMS. Mission complete.";

    pressEnter();
}