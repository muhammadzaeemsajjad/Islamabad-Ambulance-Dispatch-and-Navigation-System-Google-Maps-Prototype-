// ============================================================
//  DataTypes.cpp
//  Plain data structs shared across the entire system.
//  No logic, no methods — just data definitions.
//  Every other file includes this first.
// ============================================================

#pragma once
#include <string>
using namespace std;

struct IncidentReport {
    string incidentID;
    long   timestamp;
    int    severity;      // 1-10
    string description;
};

struct TrafficRecord {
    long   timestamp;
    int    vehicleCount;      // vehicles per minute observed
    float  congestionMult;    // 1.0 = free flow, >1 = congested
    string source;            // "sensor", "report", "api"
};

struct PatientCall {
    int    callID;
    int    patientNode;
    int    urgency;           // 1-10 (10 = critical)
    string patientName;
    string condition;
    long   callTime;
    bool   dispatched;
    bool   completed;

    PatientCall() {
        callID = -1;
        patientNode = -1;
        urgency = 0;
        patientName = "";
        condition = "";
        callTime = 0;
        dispatched = false;
        completed = false;
    }
};

struct HeapNode {
    int   nodeID;
    float cost;
    HeapNode() : nodeID(-1), cost(3.4e+38f) {}
    HeapNode(int id, float c) : nodeID(id), cost(c) {}
};

struct AlertNode {
    int    intersectionID;
    float  dangerScore;
    string message;
    long   expiresAt;

    AlertNode() {
        intersectionID = -1;
        dangerScore = 0;
        expiresAt = 0;
    }
    AlertNode(int id, float d, const string& msg, long exp) {
        intersectionID = id;
        dangerScore = d;
        message = msg;
        expiresAt = exp;
    }
};
