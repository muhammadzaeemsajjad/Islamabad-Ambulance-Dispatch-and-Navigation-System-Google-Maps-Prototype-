// ============================================================
//  AmbulanceFleet.cpp
//  Ambulance struct and a singly linked list of ambulances.
//  Fleet starts at PIMS Hospital (node 0).
//  getAvailable() returns the first idle ambulance found.
// ============================================================

#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Ambulance {
    int ambID;
    bool available;
    int currentNode;
    int assignedPatient;
    int destinationNode;
    string status;
    Ambulance* next;

    Ambulance(int id) {
        ambID = id;
        this->available = true;
        this->currentNode = 0;
        this->assignedPatient = -1;
        this->destinationNode = -1;
        this->status = "IDLE";
        this->next = nullptr;
    }
};

class AmbulanceFleet {
private:
    Ambulance* head;
    int count;

public:
    AmbulanceFleet(){
    this->head = nullptr;
        this->count = 0;
    }

    void addAmbulance(int id) {
        Ambulance* a = new Ambulance(id);
        a->next = head;
        head = a;
        count++;
    }

    Ambulance* getAvailable() {
        Ambulance* cur = head;
        while (cur) {
            if (cur->available) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void moveToEnd(Ambulance* target) {
        if (!head || !target || !head->next) return;

        if (head == target) {
            head = head->next; // Move head pointer if target is at the front
        } else {
            Ambulance* prev = head;
            while (prev->next && prev->next != target) {
                prev = prev->next;
            }
            // If we found the target in the list, bypass it
            if (prev->next == target) {
                prev->next = target->next;
            } else {
                return; // Target wasn't actually in the list
            }
        }
    }
};

