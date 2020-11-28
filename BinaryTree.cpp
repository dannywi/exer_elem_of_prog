#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <deque>

using namespace std;

namespace Impl {
    template<typename T>
    class BinaryTree {
        struct Node {
            T data;
            shared_ptr<Node> left, right;
            ~Node() { cout << "    .... deleting " << data << endl; }
        };

        shared_ptr<Node> root;

    static void insert(const T& val, shared_ptr<Node>& node) {
        if(node == nullptr) {
            node = shared_ptr<Node>(new Node());
            node->data = val;
        } else if(val <= node->data) {
            insert(val, node->left);
        } else {
            insert(val, node->right);
        }
    }

    static void print(const shared_ptr<Node>& node) {
        if(node != nullptr) {
            //cout << " ** " << node->data << endl;
            print(node->left);
            cout << " ## " << node->data << endl;
            print(node->right);
            //cout << " -- " << node->data << endl;
        }
    }

    static int get_entry_count(const T& val, const shared_ptr<Node>& node) {
        if(node == nullptr)
            return 0;
        return get_entry_count(val, node->left) + get_entry_count(val, node->right) +
            (node->data == val ? 1 : 0);
    }

    static void remove(const T& val, shared_ptr<Node>& node) {
        if(node->data == val) {
            if(node->left != nullptr && node->left->data == val) {
                remove(val, node->left);
            } else {
                // put my left in my place
                // put my right in my left's most right leaf
                shared_ptr<Node> tmp = node;
                if(node->left != nullptr) {
                    node = tmp->left;
                    if(tmp->right != nullptr) {
                        shared_ptr<Node> tgt = node;
                        while(tgt->right != nullptr)
                            tgt = tgt->right;
                        tgt->right = tmp->right;
                    }
                } else {
                    node = tmp->right;
                }
            }
        } else if(val < node->data) {
            remove(val, node->left);
        } else {
            remove(val, node->right);
        }
    }

    int get_depth(const shared_ptr<Node>& node) const {
        if(node == nullptr)
            return 0;

        return 1 + max(get_depth(node->left), get_depth(node->right));
    }

    public:
        void insert(const T& val) { insert(val, root); }
        void print() const { print(root); }
        int get_entry_count(const T& val) const { return get_entry_count(val, root); }
        void remove(const T& val) { remove(val, root); }
        int get_depth() const { return get_depth(root); }
    };
}

bool test() {
    cout << "** TESTING BINARY TREE **" << endl;
    bool res = true;

    Impl::BinaryTree<int> bt;
    vector<int> inputs { 2, 3, 5, 4, 4, 3, 1, 7, 6, 8 };
    for_each(inputs.begin(), inputs.end(), [&](int x) { bt.insert(x); });

    cout << "> checking entries ..." << endl;
    res &= bt.get_entry_count(4) == 2;
    res &= bt.get_entry_count(1) == 1;
    res &= bt.get_entry_count(8) == 1;
    bt.print();

    int depth = bt.get_depth();
    cout << "> depth .... " << depth << endl;
    res &= depth == 5;

    cout << "> removing some ... " << endl;
    bt.remove(4);
    bt.remove(5);
    bt.remove(8);
    bt.print();
    res &= bt.get_entry_count(4) == 1;
    res &= bt.get_entry_count(5) == 0;
    res &= bt.get_entry_count(8) == 0;

    return res;
}

void run() {
    bool res = true;

    res &= test();

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}

int main() {
    cout << "===== Stack And Queues =====" << endl;

    run();

    return 0;
}