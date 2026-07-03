// ============================================================
//  MaxHeap.cpp
//  Max-heap keyed on dangerScore (float).
//  Stores active danger alerts — highest danger floats to top.
//  TTL-based: purgeExpired() removes stale alerts before display.
// ============================================================

#pragma once
#include "DataTypes.cpp"
#include <iostream>
#include <ctime>
using namespace std;

class CityGraph;

class MaxHeap {
private:
    AlertNode* heap;
    int        capacity;
    int        sz;

    int parent(int i) {
        return (i-1)/2;
    }

    int leftChild(int i) {
        return 2 * i + 1;
    }

    int rightChild(int i) {
        return 2 * i + 2;
    }

    void heapifyUp(int i) {
    // While we aren't at the root, and the current node's danger is HIGHER than its parent
    while (i > 0 && heap[i].dangerScore > heap[parent(i)].dangerScore) {
        
        swap(heap[i], heap[parent(i)]); // <--- Clean, single-line swap
        
        i = parent(i); // Move up to the parent's index
    }
}

    void heapifyDown(int i) {
       while (true)
       {
         int lg = i;
        int l  = leftChild(i);
        int r  = rightChild(i);
        if (l < sz && heap[l].dangerScore > heap[lg].dangerScore) {
            lg = l;
        }
        if (r < sz && heap[r].dangerScore > heap[lg].dangerScore) {
            lg = r;
        }
        if (lg != i) {
           swap(heap[i],heap[lg]);
           i = lg;
        }else{
            break;
        }
       }
       
    }

public:
    MaxHeap(int cap) {
        this->capacity = cap;
        this->sz = 0;
        heap = new AlertNode[cap];
    }

    void insert(AlertNode n) {
        if (sz < capacity) {
            heap[sz++] = n;
            heapifyUp(sz - 1);
        }
    }

    // Remove alerts whose TTL has expired, then re-heapify
    void purgeExpired() {
        long now = (long)time(nullptr);
        AlertNode* valid = new AlertNode[capacity];
        int newSz = 0;

        for (int i = 0; i < sz; i++) {
            if (heap[i].expiresAt > now) {
                valid[newSz++] = heap[i];
            }
        }

        for (int i = 0; i < newSz; i++) {
            heap[i] = valid[i];
        }
        sz = newSz;
        // Re-heapify bottom-up after bulk removal
        for (int i = sz / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
        delete[] valid;
    }

    void display(CityGraph* city) {
        // purgeExpired();
        cout << "\n   =========== TOP DANGER ALERTS (Max-Heap, live) =============\n";
        if (sz == 0) {
            cout << "  |  No active alerts.\n";
        }
        for (int i = 0; i < sz; i++) {
            cout << "  |  [" << city->getNodeName(heap[i].intersectionID) << "]"
                 << " danger:" << heap[i].dangerScore
                 << " | "      << heap[i].message << "\n";
        }
        cout << "  |____________________________________________________________|";
    }

    bool isEmpty() const {
        return sz == 0;
    }
    int size()    const {
        return sz;
    }
};
