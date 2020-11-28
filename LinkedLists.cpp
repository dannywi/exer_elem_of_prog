#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

using namespace std;

namespace LinkedList {
    template<typename T>
    struct Node {
        T data;
        shared_ptr<Node<T>> next;
        ~Node() { cout << "deleted " << data << endl; }

        //enum SortDir { ASC, DSC };

        static bool isSorted(shared_ptr<Node<T>> node, bool asc = true) {
            return node == nullptr || node->next == nullptr||
                (asc ? node->data <= node->next->data : node->data >= node->next->data &&
                isSorted(node->next, asc));
        }

        static int size(shared_ptr<Node<T>> node) {
            return node == nullptr ? 0 : 1 + size(node->next);
        }
    };

    template<typename T>
    shared_ptr<Node<T>> create_list(const vector<T>& vals) {
        shared_ptr<Node<T>> first = nullptr;
        shared_ptr<Node<T>> last = nullptr;
        for(auto v : vals) {
            shared_ptr<Node<T>> curr(new Node<T>());
            curr->data = v;
            if(last != nullptr)
                last->next = curr;
            if(first == nullptr)
                first = curr;
            last = curr;
        }
        return first;
    }

    template<typename T>
    void print(shared_ptr<Node<T>> root) {
        if(root == nullptr)
            cout << " .. empty .. ";
        while(root != nullptr) {
            cout << root->data << " ";
            root = root->next;
        }
        cout << endl;
    }

    template<typename T>
    shared_ptr<Node<T>> mergeSortedLists(shared_ptr<Node<T>> a, shared_ptr<Node<T>> b) {

        shared_ptr<Node<T>> root;
        shared_ptr<Node<T>> last;
        while(a != nullptr || b != nullptr) {
            shared_ptr<Node<T>> curr;
            if(a == nullptr) {
                curr = b;
                b = b->next;
            } else if(b == nullptr) {
                curr = a;
                a = a->next;
            } else if(a->data <= b->data) {
                curr = a;
                a = a->next;
            } else {
                curr = b;
                b = b->next;
            }

            if(root == nullptr)
                root = curr;

            if(last != nullptr)
                last->next = curr;
            last = curr;
        }

        return root;
    }

    // recursive
    template<typename T>
    shared_ptr<Node<T>> reverse(shared_ptr<Node<T>> root) {
        std::function<void(shared_ptr<Node<T>>, shared_ptr<Node<T>>, shared_ptr<Node<T>>&)> reverse_sub =
            [&](shared_ptr<Node<T>> curr, shared_ptr<Node<T>> next, shared_ptr<Node<T>>& nr) {
            if(next != nullptr) {
                //cout << "  bef [" << curr->data << "] -> [" << next->data << "]" << endl;
                reverse_sub(next, next->next, nr);
                next->next = curr;
                curr->next = nullptr;
                //cout << "  aft [" << next->data << "] -> [" << next->next->data << "]" << endl;
            } else {
                //cout << " new root " << curr->data << endl;
                nr = curr;
            }
        };
        
        shared_ptr<Node<T>> new_root;
        reverse_sub(root, root->next, new_root);
        return new_root == nullptr ? root : new_root;
    }

    // looping
    template<typename T>
    shared_ptr<Node<T>> reverse2(shared_ptr<Node<T>> root) {
        cout << "BEF: ";
        print(root);

        shared_ptr<Node<T>> next = root->next;
        root->next = nullptr;
        shared_ptr<Node<T>> last = root;
        while(next != nullptr) {
            shared_ptr<Node<T>> tmp = next->next;
            next->next = last;
            last = next;
            next = tmp;
        }

        cout << "AFT: ";
        print(last);

        return last;
    }

    void run() {
        auto list1 = create_list( vector<int>({ 1, 5, 10, 15 }) );
        auto list2 = create_list( vector<int>({ 2, 3, 5, 7, 11, 15, 16 }) );

        bool res = true;

        auto root = mergeSortedLists(list1, list2);
        res &= Node<int>::size(root) == 11 && Node<int>::isSorted(root);
        print(root);

        root = reverse(root);
        res &= Node<int>::size(root) == 11 && Node<int>::isSorted(root, false);
        print(root);

        auto root2 = create_list( vector<int>({ 1, 5, 10, 15, 19, 32, 33 }) );
        root2 = reverse2(root2);
        res &= Node<int>::size(root2) == 7 && Node<int>::isSorted(root2, false);

        cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

int main() {
    cout << "===== Linked List =====" << endl;

    LinkedList::run();

    return 0;
}