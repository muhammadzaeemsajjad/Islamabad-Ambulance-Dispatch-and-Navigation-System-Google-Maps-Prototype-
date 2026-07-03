// ============================================================
//  Queue.cpp
//  Two queue data structures:
//
//  CircularQueue — fixed-size ring buffer for the news ticker.
//                  Overwrites oldest entry when full (50 slots).
//
//  PatientQueue  — max-heap keyed on urgency (1-10).
//                  Highest urgency patient is always served first.
//                  Used by the ambulance dispatch system.
// ============================================================

#pragma once
#include "DataTypes.cpp"
#include <iostream>
#include <ctime>
using namespace std;

class CityGraph;

// ── Circular Queue (News Ticker) ─────────────────────────────
class CircularQueue {
private:
    static const int CAP = 50;
    string buf[CAP];
    int    head, tail, cnt;

public:
    CircularQueue() {
        this->head = 0;
        this->tail = 0;
        this->cnt = 0;
    }

    void enqueue(const string& msg) {
        buf[tail] = msg;
        tail      = (tail + 1) % CAP;
        if (cnt < CAP) cnt++;
        else           head = (head + 1) % CAP;   // overwrite oldest
    }

    bool isEmpty() {
        return cnt == 0;
    }

    int  size() {
        return cnt;
    }

    void displayAll()  {
        if (cnt == 0) {
            cout << "  [TICKER] No messages.\n";
            return;
        }

        cout << "\n   ============ NEWS TICKER (last " << cnt << " messages) ============\n";

        for (int i = 0; i < cnt; i++) {
            cout << "  |  " << i + 1 << ". " << buf[(head + i) % CAP] << "\n";
        }
        cout << "  |_______________________________________________________|";
    }
};

// ── Patient Queue (Max-Heap by urgency) ──────────────────────
class PatientQueue {
private:
    PatientCall* heap;
    int capacity;
    int sz;
    int nextID;

    int parent(int i) {
        return (i - 1) / 2;
    }

    int leftChild(int i) {
        return 2 * i + 1;
    }

    int rightChild(int i) {
        return 2 * i + 2;
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[i].urgency > heap[parent(i)].urgency) {
            swap(heap[i],heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int largest = i;
        int l       = leftChild(i);
        int r       = rightChild(i);
        if (l < sz && heap[l].urgency > heap[largest].urgency) {
            largest = l;
        }
        if (r < sz && heap[r].urgency > heap[largest].urgency) {
            largest = r;
        }
        if (largest != i) {
            PatientCall tmp = heap[i];
            heap[i]         = heap[largest];
            heap[largest]   = tmp;
            heapifyDown(largest);
        }
    }

public:
    PatientQueue(int cap) {
        this->capacity = cap;
        this->sz = 0;
        this->nextID = 1001;
        this->heap = new PatientCall[cap];

    }

    int addCall(int node, int urgency, const string& name, const string& condition, CityGraph* city) {
        if (sz >= capacity) {
            cout << "  [QUEUE FULL] Cannot accept more calls!\n";
            return -1;
        }
        if (node < 0 || node > 49) {
            cout << "  [ERROR] Invalid patient location node: " << node << "\n";
            return -1;
        }

        PatientCall p;
        p.callID      = nextID++;
        p.patientNode = node;
        p.urgency     = (urgency < 1) ? 1 : (urgency > 10) ? 10 : urgency;
        p.patientName = name;
        p.condition   = condition;
        p.callTime    = (long)time(nullptr);
        p.dispatched  = false;
        p.completed   = false;
        heap[sz++]    = p;
        heapifyUp(sz - 1);
        cout << "  [CALL " << p.callID << "] Registered: " << name
             << " at "     << city->getNodeName(node)                  //jugjyfjfikfuf
             << " | Urgency: " << p.urgency << "/10\n";
        return p.callID;
    }

    // Extract highest-urgency pending patient
    PatientCall extractNext() {
        PatientCall empty;
        if (sz == 0) return empty;
        PatientCall top = heap[0];
        heap[0]         = heap[--sz];
        heapifyDown(0);
        return top;
    }

    PatientCall peekNext()  {
        PatientCall empty;
        if (sz == 0) return empty;
        return heap[0];
    }

    bool isEmpty()  {
        return sz == 0;
    }

    int  size()     {
        return sz;
    }

    void displayAll(CityGraph* city) {
        if (sz == 0) {
            cout << "  [QUEUE] No pending patient calls.\n";
            return;
        }
        cout << "\n   ========================== PATIENT CALL QUEUE (by urgency) ==========================\n";
        // Sorted display copy — does not modify the actual heap
        PatientCall* tmp = new PatientCall[sz];

        for (int i = 0; i < sz; i++) {
            tmp[i] = heap[i];
        }

        for (int i = 0; i < sz - 1; i++) {
            for (int j = 0; j < sz - i - 1; j++) {
                if (tmp[j].urgency < tmp[j + 1].urgency) {
                    PatientCall t = tmp[j];
                    tmp[j]        = tmp[j + 1];
                    tmp[j + 1]    = t;
                }
            }
        }
        for (int i = 0; i < sz; i++) {
            cout << "  |  [CALL " << tmp[i].callID << "] "
                 << tmp[i].patientName
                 << " | Node " << tmp[i].patientNode
                 << " ("       << city->getNodeName(tmp[i].patientNode) << ")"
                 << " | Urgency:" << tmp[i].urgency
                 << " | "         << tmp[i].condition << "\n";
        }
        cout << "  |_____________________________________________________________________________________|";
        delete[] tmp;
    }
};
