// ============================================================
//  IncidentBST.cpp
//  AVL BST — one instance lives inside each graph node.
//  Sorted by timestamp. Each BST node holds an IncidentList
//  for same-timestamp incidents (BST node → linked list).
//  Used by Dijkstra to query danger severity in a time range.
// ============================================================

#pragma once
#include "LinkedList.cpp"
#include <iostream>
using namespace std;

class IncBSTNode {
public:
    long          timestamp;
    IncidentList* incidents;   // linked list of reports at this timestamp
    IncBSTNode*   left;
    IncBSTNode*   right;
    int           height;

    IncBSTNode(long ts, IncidentReport r) {
        timestamp = ts;
        left = nullptr;
        right = nullptr;
        height = 1;
        incidents = new IncidentList();
        incidents->append(r);
    }
};

class IncidentBST {
private:
    IncBSTNode* root;
    int h(IncBSTNode* n) {
        if (n != nullptr) {
            return n->height;
        } else {
            return 0;
        }
    }

    int bf(IncBSTNode* n) {
        if (n != nullptr) {
            int leftHeight = h(n->left);
            int rightHeight = h(n->right);

            return leftHeight - rightHeight;
        } else {
            return 0;
        }
    }

    void upH(IncBSTNode* n) {
        if (n != nullptr) {
            int leftHeight = h(n->left);
            int rightHeight = h(n->right);

            n->height = 1 + max(leftHeight, rightHeight);
        }
    }

    IncBSTNode* rotR(IncBSTNode* y) {
        IncBSTNode* x  = y->left;
        IncBSTNode* T2 = x->right;
        x->right = y;
        y->left  = T2;
        upH(y); upH(x);
        return x;
    }

    IncBSTNode* rotL(IncBSTNode* x) {
        IncBSTNode* y  = x->right;
        IncBSTNode* T2 = y->left;
        y->left  = x;
        x->right = T2;
        upH(x); upH(y);
        return y;
    }

    IncBSTNode* balance(IncBSTNode* n) {
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

    IncBSTNode* ins(IncBSTNode* node, IncidentReport r) {
        if (!node) {
            return new IncBSTNode(r.timestamp, r);
        }
        if (r.timestamp == node->timestamp) {
            node->incidents->append(r);
            return node;
        }
        if (r.timestamp < node->timestamp) {
            node->left  = ins(node->left,  r);
        }else {
            node->right = ins(node->right, r);
        }
        return balance(node);
    }

    // Sum severity of all incidents whose timestamp falls in [lo, hi]
    int rangeSum(IncBSTNode* n, long lo, long hi) {
        if (!n) return 0;
        int t = 0;

        if (n->timestamp >= lo && n->timestamp <= hi) {
            t += n->incidents->sumSeverity();
        }
        if (n->timestamp > lo) {
            t += rangeSum(n->left,  lo, hi);
        }
        if (n->timestamp < hi) {
            t += rangeSum(n->right, lo, hi);
        }
        return t;
    }

    void inorder(IncBSTNode* n) {
        if (!n) return;
        inorder(n->left);

        cout << "  |          [" << n->timestamp << "]:";
        n->incidents->printAll();

        inorder(n->right);
    }

public:
    IncidentBST() {
        root = nullptr;
    }

    bool empty() {
        return root == nullptr;
    }

    void insert(IncidentReport r) {
        root = ins(root, r);
    }

    // Returns total severity score in time window — used by Dijkstra edge cost
    int querySeverity(long lo, long hi) {
        return rangeSum(root, lo, hi);
    }

    void printAll() {
        inorder(root);
    }
};
