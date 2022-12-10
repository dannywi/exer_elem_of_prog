#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T value;
    shared_ptr<Node<T>> next = nullptr;
};

template <typename T>
ostream& operator<<(ostream& out, shared_ptr<Node<T>> node) {
    while (node != nullptr) {
        out << node->value << " ";
        node = node->next;
    }
    return out;
}

namespace MergeSort {
template <typename T>
shared_ptr<Node<T>> merge(shared_ptr<Node<T>> left, shared_ptr<Node<T>> right) {
    shared_ptr<Node<T>> root = nullptr, last = nullptr;
    while (left != nullptr || right != nullptr) {
        shared_ptr<Node<T>> curr = nullptr;
        if (left == nullptr) {
            curr = right;
            right = right->next;
        } else if (right == nullptr) {
            curr = left;
            left = left->next;
        } else if (left->value <= right->value) {
            curr = left;
            left = left->next;
        } else {
            curr = right;
            right = right->next;
        }

        if (root == nullptr) root = curr;
        if (last != nullptr) last->next = curr;
        last = curr;
    }
    return root;
}

template <typename T>
shared_ptr<Node<T>> solve(shared_ptr<Node<T>> node) {
    if (node == nullptr || node->next == nullptr) return node;
    if (node->next->next == nullptr) {
        if (node->value > node->next->value) {
            // swap
            auto tmp = node->next;
            tmp->next = node;
            node->next = nullptr;
            node = tmp;
        }
        return node;
    }

    // find mid, split there
    shared_ptr<Node<T>> slow = node, fast = node, preslow = node;
    while (fast != nullptr && fast->next != nullptr) {
        preslow = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    auto left = node;
    auto right = slow;
    preslow->next = nullptr;

    // sort and merge
    return merge(solve(left), solve(right));
}

template <typename T>
bool checkSorted(shared_ptr<Node<T>> node) {
    while (node != nullptr && node->next != nullptr) {
        if (node->value > node->next->value) return false;
        node = node->next;
    }
    return true;
}

shared_ptr<Node<int>> generateRandom(int count) {
    int range = 90;
    // create list
    shared_ptr<Node<int>> root = nullptr, last = nullptr;
    for (int i = 0; i < count; ++i) {
        shared_ptr<Node<int>> node(new Node<int>());
        node->value = rand() % range;
        if (last != nullptr) last->next = node;
        if (root == nullptr) root = node;
        last = node;
    }
    return root;
}

bool test() {
    bool res = true;

    auto root = generateRandom(20);
    cout << "orig { " << root << " }" << endl;
    auto sorted = solve(root);
    cout << "sort { " << sorted << " }" << endl;
    res &= checkSorted(sorted);

    // do a few more
    sorted = solve(generateRandom(100));
    res &= checkSorted(sorted);
    sorted = solve(generateRandom(1));
    res &= checkSorted(sorted);
    sorted = solve(generateRandom(2));
    res &= checkSorted(sorted);
    sorted = solve(generateRandom(3));
    res &= checkSorted(sorted);
    sorted = solve(generateRandom(4));
    res &= checkSorted(sorted);
    sorted = solve(generateRandom(20000));
    res &= checkSorted(sorted);

    return res;
}
}  // namespace MergeSort

int main(int argc, char** argv) {
    cout << "==== SORTING ====" << endl;
    bool res = true;
    res &= MergeSort::test();
    cout << "ALL TESTS" << (res ? "" : "NOT") << " PASSED" << endl;
}