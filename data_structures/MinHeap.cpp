// ============================================================
//  MinHeap.cpp
//  Min-heap keyed on cost (float).
//  Used as the priority queue inside Dijkstra's algorithm —
//  always extracts the lowest-cost unvisited node next.
// ============================================================

#pragma once
#include "DataTypes.cpp"

class MinHeap {
private:
    HeapNode* heap;
    int       capacity;
    int       sz;

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
        while (i > 0 && heap[i].cost < heap[parent(i)].cost) {
            swap(heap[i],heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
       while (true)
       {
        int s = i;
        int l = leftChild(i);
        int r = rightChild(i);
        if (l < sz && heap[l].cost < heap[s].cost) s = l;
        if (r < sz && heap[r].cost < heap[s].cost) s = r;
        if (s != i) {
            swap(heap[i],heap[s]);
            i = s;
        }else{
            break;
        }
       }
       
    }

public:
    MinHeap(int cap) : capacity(cap), sz(0) {
        heap = new HeapNode[cap];
    }

    void insert(HeapNode n) {
        if (sz < capacity) {
            heap[sz++] = n;
            heapifyUp(sz - 1);
        }
    }

    HeapNode extractMin() {
        HeapNode t = heap[0];
        heap[0]    = heap[--sz];
        heapifyDown(0);
        return t;
    }

    bool isEmpty()  {
        return sz == 0;
    }
};
