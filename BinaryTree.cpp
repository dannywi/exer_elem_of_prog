#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace Impl {
template <typename T>
class BinaryTree {
    struct Node {
        T data;
        shared_ptr<Node> left, right;
        ~Node() {
            cout << "    .... deleting " << data << endl;
        }
    };

    shared_ptr<Node> root;

    static void insert(const T& val, shared_ptr<Node>& node) {
        if (node == nullptr) {
            node = shared_ptr<Node>(new Node());
            node->data = val;
        } else if (val <= node->data) {
            insert(val, node->left);
        } else {
            insert(val, node->right);
        }
    }

    static int get_entry_count(const T& val, const shared_ptr<Node>& node) {
        if (node == nullptr) return 0;
        return get_entry_count(val, node->left) + get_entry_count(val, node->right) + (node->data == val ? 1 : 0);
    }

    static void remove(const T& val, shared_ptr<Node>& node) {
        if (node->data == val) {
            if (node->left != nullptr && node->left->data == val) {
                remove(val, node->left);
            } else {
                // put my left in my place
                // put my right in my left's most right leaf
                shared_ptr<Node> tmp = node;
                if (node->left != nullptr) {
                    node = tmp->left;
                    if (tmp->right != nullptr) {
                        shared_ptr<Node> tgt = node;
                        while (tgt->right != nullptr) tgt = tgt->right;
                        tgt->right = tmp->right;
                    }
                } else {
                    node = tmp->right;
                }
            }
        } else if (val < node->data) {
            remove(val, node->left);
        } else {
            remove(val, node->right);
        }
    }

    int get_depth(const shared_ptr<Node>& node) const {
        if (node == nullptr) return 0;

        return 1 + max(get_depth(node->left), get_depth(node->right));
    }

    static void invert(shared_ptr<Node>& node) {
        if (node == nullptr) return;
        invert(node->left);
        invert(node->right);

        swap(node->left, node->right);
    }

   public:
    void insert(const T& val) {
        insert(val, root);
    }
    int get_entry_count(const T& val) const {
        return get_entry_count(val, root);
    }
    void remove(const T& val) {
        remove(val, root);
    }
    int get_depth() const {
        return get_depth(root);
    }

    BinaryTree<T> deepCopy() const {
        BinaryTree<T> copy = BinaryTree<T>();
        function<void(const shared_ptr<Node>&, shared_ptr<Node>&)> deepCopy = [&](const shared_ptr<Node>& src,
                                                                                  shared_ptr<Node>& dup) {
            if (src == nullptr) return;
            dup = make_shared<Node>();
            dup->data = src->data;
            deepCopy(src->left, dup->left);
            deepCopy(src->right, dup->right);
        };
        deepCopy(root, copy.root);
        return copy;
    }

    vector<T> get_elem_inorder() const {
        vector<T> vec;
        function<void(shared_ptr<Node>)> traverse = [&](shared_ptr<Node> n) {
            if (n == nullptr) return;
            traverse(n->left);
            vec.emplace_back(n->data);
            traverse(n->right);
        };
        traverse(root);
        return vec;
    }

    void print() const {
        auto vec = get_elem_inorder();
        cout << vec.size() << " { ";
        for_each(vec.begin(), vec.end(), [](T v) { cout << v << " "; });
        cout << "}" << endl;
    }

    BinaryTree<T> get_inverted() const {
        BinaryTree<T> copy = deepCopy();
        invert(copy.root);
        return copy;
    }
};
}  // namespace Impl

bool test() {
    cout << "** TESTING BINARY TREE **" << endl;
    bool res = true;

    Impl::BinaryTree<int> bt;
    vector<int> inputs{2, 3, 5, 4, 4, 3, 1, 7, 6, 8};
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

bool test_inversion() {
    cout << "** TESTING INVERSION **" << endl;
    Impl::BinaryTree<int> bt;
    vector<int> inputs{2, 3, 5, 4, 4, 3, 1, 7, 6, 8};
    for_each(inputs.begin(), inputs.end(), [&](int x) { bt.insert(x); });

    cout << "orig: ";
    bt.print();
    vector<int> bt_v = bt.get_elem_inorder();

    Impl::BinaryTree<int> inverted = bt.get_inverted();
    cout << "invr: ";
    inverted.print();
    vector<int> inverted_v = inverted.get_elem_inorder();

    bool res = true;
    for (int i = 0; i < bt_v.size(); ++i) {
        // cout << " ... " << bt_v[i] << " - " << inverted_v[bt_v.size() - 1 - i] << endl;
        res &= bt_v[i] == inverted_v[bt_v.size() - 1 - i];
    }

    return res;
}

void run() {
    bool res = true;

    res &= test();
    res &= test_inversion();

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}

int main() {
    cout << "===== Stack And Queues =====" << endl;

    run();

    return 0;
}