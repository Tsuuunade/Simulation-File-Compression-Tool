#include "HCTree.hpp"
#include "Helper.hpp"
#include <vector>
#include <queue>
#include <stack>
using namespace std;

// Helper function to recursively delete nodes
void deleteHCT(HCNode* node) {
    if (node != nullptr) {
        deleteHCT(node->c0);
        deleteHCT(node->c1);
        delete node;
    }
}

// Destructor
HCTree::~HCTree() {
    if (root != nullptr) {
        deleteHCT(root);
    }
}

void HCTree::build(const vector<int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    // Create leaf nodes for all characters and insert them into the priority queue
    for (unsigned int i = 0; i < freqs.size(); i++) {
        if (freqs[i] != 0) {
            HCNode* node = new HCNode(freqs[i], i);
            leaves[i] = node;
            pq.push(node);
        }
    }

    // Handle two special cases
    if (pq.size() == 0) {
        root = nullptr;
        return;
    }

    while (pq.size() != 1) {
        // Create an internal node
        HCNode* parent = new HCNode(0, 0);

        // Set left and right nodes
        parent->c0 = pq.top();
        parent->c0->p = parent;
        pq.pop();
        parent->c1 = pq.top();
        parent->c1->p = parent;
        pq.pop();

        // Push parents into pq
        parent->count = parent->c0->count + parent->c1->count;
        parent->symbol = parent->c0->symbol;
        pq.push(parent);
    }

    root = pq.top();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    // Find the leaf node containing the symbol
    HCNode* node = this->leaves[symbol];
    stack<int> bits;

    // Traverse up the tree to store each bit on  the stack
    while(node != root) {
        if (node->p->c0 == node) {
            bits.push(0);
        }
        else if (node->p->c1 == node) {
            bits.push(1);
        }
        node = node->p;
    }

    // Iterate over the stack and store into buffer
    while(bits.size() != 0) {
        out.write_bit(bits.top());
        bits.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream& in) const {
    HCNode* node = root;
    int bit;

    // Traverse from root to leaf by reading bits
    while (node->c0 != nullptr || node->c1 != nullptr) {
        bit = in.read_bit();

        // Be consistent with the encoding.
        if (bit == 1) {
            node = node->c1;
        }
        else if (bit == 0) {
            node = node->c0;
        }
        else {
            return -1;
        }
    }

    return node->symbol;
}

