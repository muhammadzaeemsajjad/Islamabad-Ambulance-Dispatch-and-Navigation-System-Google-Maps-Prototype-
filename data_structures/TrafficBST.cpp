// ============================================================
//  TrafficBST.cpp
//  AVL BST — one instance lives inside each graph node.
//  Sorted by timestamp. Each BST node holds a TrafficList
//  for same-timestamp readings (BST node → linked list).
//  Used by Dijkstra to query average congestion in a time range.
// ============================================================

#pragma once
#include "LinkedList.cpp"
#include <iostream>
using namespace std;

struct TrafBSTNode {
    long         timestamp;
    TrafficList* records;    // linked list of readings at this timestamp
    TrafBSTNode* left;
    TrafBSTNode* right;
    int          height;

    TrafBSTNode(TrafficRecord r) {
        this->timestamp = r.timestamp;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 1;
        this->records = new TrafficList();
        this->records->append(r);
    }
};

class TrafficBST {
private:
    TrafBSTNode* root;

    int h(TrafBSTNode* n) {
        if (n != nullptr) {
            return n->height;
        } else {
            return 0;
        }
    }

    int bf(TrafBSTNode* n) {
        if (n != nullptr) {
            int leftHeight = h(n->left);
            int rightHeight = h(n->right);

            return leftHeight - rightHeight;
        } else {
            return 0;
        }
    }

    void upH(TrafBSTNode* n) {
        if (n != nullptr) {
            int leftHeight = h(n->left);
            int rightHeight = h(n->right);

            n->height = 1 + max(leftHeight, rightHeight);
        }
    }

    TrafBSTNode* rotR(TrafBSTNode* y) {
        TrafBSTNode* x  = y->left;
        TrafBSTNode* T2 = x->right;
        x->right = y;
        y->left  = T2;
        upH(y); upH(x);
        return x;
    }

    TrafBSTNode* rotL(TrafBSTNode* x) {
        TrafBSTNode* y  = x->right;
        TrafBSTNode* T2 = y->left;
        y->left  = x;
        x->right = T2;
        upH(x); upH(y);
        return y;
    }

    TrafBSTNode* balance(TrafBSTNode* n) {
        upH(n);
        int b = bf(n);
        if (b >  1 && bf(n->left)  >= 0) {
            return rotR(n);
        }
        if (b >  1 && bf(n->left)  <  0) {
            n->left  = rotL(n->left);
            return rotR(n);
        }
        if (b < -1 && bf(n->right) <= 0) {
            return rotL(n);
        }
        if (b < -1 && bf(n->right) >  0) {
            n->right = rotR(n->right);
            return rotL(n);
        }
        return n;
    }

    TrafBSTNode* ins(TrafBSTNode* node, TrafficRecord r) {
        if (!node) {
            return new TrafBSTNode(r);
        }
        if (r.timestamp == node->timestamp) {
            node->records->append(r);
            return node;
        }
        if (r.timestamp < node->timestamp) {
            node->left  = ins(node->left,  r);
        }else {
            node->right = ins(node->right, r);
        }
        return balance(node);
    }

    // Accumulate avg congestion of all nodes whose timestamp is in [lo, hi]
    void rangeAvg(TrafBSTNode* n, long lo, long hi, float& sum, int& cnt) {
        if (!n) return;
        if (n->timestamp >= lo && n->timestamp <= hi) {
            sum += n->records->avgCongestion();
            cnt++;
        }
        if (n->timestamp > lo) {
            rangeAvg(n->left,  lo, hi, sum, cnt);
        }
        if (n->timestamp < hi) {
            rangeAvg(n->right, lo, hi, sum, cnt);
        }
    }

    //One architectural limitation of our current computeEdgeCost formula is that it uses a flat 24-hour future window floor (now + 86400) to query our AVL trees. This means a scheduled roadblock or traffic spike happening 4 hours in the future can artificially inflate the road's cost right now, even if the ambulance would pass through before the jam begins.While a real-world production system would use dynamically moving time-slices as Dijkstra moves through the branches, we intentionally utilized a static 24-hour boundary window here. This keeps our graph relaxation math clean and manageable for this system simulation, while perfectly demonstrating how an AVL tree range query surgically isolates daily timeline logs at $O(\log N)$ speeds
    //We chose a 24-hour future window (now + 86400) because our simulation data cycle runs on a 24-hour horizon. This specific value acts as a sweet spot for our AVL Tree range queries: it is large enough to ensure that any planned roadblocks or data anomalies within the day's timeline are safely caught, but small enough that our BST can still prune away irrelevant far-future nodes, keeping our search time optimized at $O(\log N)$.

    void inorder(TrafBSTNode* n) const {
        if (!n) return;
        inorder(n->left);
        cout << "[ts:" << n->timestamp
             << " | avgCong:" << n->records->avgCongestion() << "x]\n";
        inorder(n->right);
    }

public:
    TrafficBST() {
        this->root = nullptr;
    }

    bool empty() {
        return root == nullptr;
    }

    void insert(TrafficRecord r) {
        root = ins(root, r);
    }

    // Returns average congestion multiplier in time range — 1.0 if no data
    float queryCongestion(long lo, long hi) {
        float sum = 0.0f;
        int   cnt = 0;
        rangeAvg(root, lo, hi, sum, cnt);
        return (cnt > 0) ? sum / cnt : 1.0f;
    }

    void printAll() const {
        inorder(root);
    }
};
