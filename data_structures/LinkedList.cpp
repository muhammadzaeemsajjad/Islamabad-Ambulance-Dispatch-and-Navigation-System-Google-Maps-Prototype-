// ============================================================
//  LinkedList.cpp
//  Two singly linked lists:
//    - IncidentList  : chains IncidentReports at same timestamp
//    - TrafficList   : chains TrafficRecords  at same timestamp
//  These live INSIDE BST nodes — they are the inner layer of
//  the nested data structure (BST node → linked list → records).
// ============================================================

#pragma once
#include "DataTypes.cpp"
#include <iostream>
using namespace std;

// ── Incident Linked List ─────────────────────────────────────
class IncidentNode {
public:
    IncidentReport data;
    IncidentNode*  next;
    IncidentNode(IncidentReport r) {
        data = r;
        next = nullptr;
    }
};

class IncidentList {
public:
    IncidentNode* head;

    IncidentList() {
        head = nullptr;
    }
    void append(IncidentReport r) {
        IncidentNode* node = new IncidentNode(r);
        if (!head) {
            head = node;
            return;
        }
        IncidentNode* cur = head;
        while (cur->next) {
            cur = cur->next;
        }
        cur->next = node;
    }

    void printAll() const {
        IncidentNode* cur = head;
        bool isFirst = true;
        while (cur) {
            if (!isFirst) {
                cout << "  |                  ";
            }

            cout << " ["
                 << cur->data.incidentID
                 << " | sev:" << cur->data.severity
                 << " | "     << cur->data.description
                 << "]\n";

            cur = cur->next;
            isFirst = false;
        }
    }

    // Sum of all severity values — used by IncidentBST range query
    int sumSeverity()  {
        int total = 0;
        IncidentNode* cur = head;
        while (cur) {
            total += cur->data.severity;
            cur = cur->next;
        }
        return total;
    }
};

// ── Traffic Linked List ──────────────────────────────────────
class TrafficNode {
public:
    TrafficRecord data;
    TrafficNode*  next;
    TrafficNode(TrafficRecord r) {
        data = r;
        next = nullptr;
    }
};

class TrafficList {
public:
    TrafficNode* head;

    TrafficList() {
        head = nullptr;
    }

    void append(TrafficRecord r) {
        TrafficNode* node = new TrafficNode(r);
        if (!head) { head = node; return; }
        TrafficNode* cur = head;
        while (cur->next) {
            cur = cur->next;
        }
        cur->next = node;
    }

    // Average congestion multiplier across all records in this list
    float avgCongestion()  {
        float total = 0.0f;
        int   cnt   = 0;
        TrafficNode* cur = head;
        while (cur) {
            total += cur->data.congestionMult;
            cnt++;
            cur = cur->next;
        }

        if (cnt > 0) {
            return total / cnt;
        }else{
            return 1.0f;
        }
    }
};
