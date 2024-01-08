#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace LinkedList {
template <typename T>
struct Node {
    T data;
    shared_ptr<Node<T>> next;
    ~Node() {
        cout << "deleted " << data << endl;
    }

    static shared_ptr<Node<T>> getLast(shared_ptr<Node<T>> node) {
        shared_ptr<Node<T>> last = node;
        while (last->next) {
            last = last->next;
        }
        return last;
    }

    // enum SortDir { ASC, DSC };

    static bool isSorted(shared_ptr<Node<T>> node, bool asc = true) {
        return node == nullptr || node->next == nullptr ||
               (asc ? node->data <= node->next->data : node->data >= node->next->data && isSorted(node->next, asc));
    }

    static int size(shared_ptr<Node<T>> node) {
        return node == nullptr ? 0 : 1 + size(node->next);
    }
};

template <typename T>
shared_ptr<Node<T>> create_list(const vector<T>& vals) {
    shared_ptr<Node<T>> first = nullptr;
    shared_ptr<Node<T>> last = nullptr;
    for (auto v : vals) {
        shared_ptr<Node<T>> curr(new Node<T>());
        curr->data = v;
        if (last != nullptr) last->next = curr;
        if (first == nullptr) first = curr;
        last = curr;
    }
    return first;
}

template <typename T>
shared_ptr<Node<T>> create_list_loop(const vector<T>& vals, size_t loop_idx) {
    shared_ptr<Node<T>> head = create_list(vals);
    shared_ptr<Node<T>> last = Node<T>::getLast(head);
    loop_idx = min(vals.size() - 1, loop_idx);
    shared_ptr<Node<T>> node = head;
    for (size_t i = 0; i < loop_idx; ++i) {
        node = node->next;
    }
    last->next = node;
    return head;
}

template <typename T>
void print(shared_ptr<Node<T>> root, size_t max = 0) {
    if (root == nullptr) cout << " .. empty .. ";
    size_t cnt = 0;
    while (root != nullptr) {
        if (max != 0 && ++cnt == max) {
            break;
        }
        cout << root->data << " ";
        root = root->next;
    }
    cout << endl;
}

template <typename T>
shared_ptr<Node<T>> mergeSortedLists(shared_ptr<Node<T>> a, shared_ptr<Node<T>> b) {
    shared_ptr<Node<T>> root;
    shared_ptr<Node<T>> last;
    while (a != nullptr || b != nullptr) {
        shared_ptr<Node<T>> curr;
        if (a == nullptr) {
            curr = b;
            b = b->next;
        } else if (b == nullptr) {
            curr = a;
            a = a->next;
        } else if (a->data <= b->data) {
            curr = a;
            a = a->next;
        } else {
            curr = b;
            b = b->next;
        }

        if (root == nullptr) root = curr;

        if (last != nullptr) last->next = curr;
        last = curr;
    }

    return root;
}

// recursive
template <typename T>
shared_ptr<Node<T>> reverse(shared_ptr<Node<T>> root) {
    std::function<void(shared_ptr<Node<T>>, shared_ptr<Node<T>>, shared_ptr<Node<T>>&)> reverse_sub =
        [&](shared_ptr<Node<T>> curr, shared_ptr<Node<T>> next, shared_ptr<Node<T>>& nr) {
            if (next != nullptr) {
                // cout << "  bef [" << curr->data << "] -> [" << next->data << "]" << endl;
                reverse_sub(next, next->next, nr);
                next->next = curr;
                curr->next = nullptr;
                // cout << "  aft [" << next->data << "] -> [" << next->next->data << "]" << endl;
            } else {
                // cout << " new root " << curr->data << endl;
                nr = curr;
            }
        };

    shared_ptr<Node<T>> new_root;
    reverse_sub(root, root->next, new_root);
    return new_root == nullptr ? root : new_root;
}

// looping
template <typename T>
shared_ptr<Node<T>> reverse2(shared_ptr<Node<T>> root) {
    cout << "BEF: ";
    print(root);

    shared_ptr<Node<T>> next = root->next;
    root->next = nullptr;
    shared_ptr<Node<T>> last = root;
    while (next != nullptr) {
        shared_ptr<Node<T>> tmp = next->next;
        next->next = last;
        last = next;
        next = tmp;
    }

    cout << "AFT: ";
    print(last);

    return last;
}

template <typename T>
bool isLooping(shared_ptr<Node<T>> root) {
    shared_ptr<Node<T>> slow = root;
    shared_ptr<Node<T>> fast = root;
    while (slow && fast) {
        // cout << "slow [" << slow->data << "] fast [" << fast->data << "]" << endl;
        slow = slow->next;
        if (!slow) {
            break;
        }
        fast = fast->next;
        if (!fast) {
            break;
        }
        fast = fast->next;
        if (slow == fast) {
            return true;
        }
    }
    return false;
}

// not working yet
/*template <typename T>
int getLoopIdx(shared_ptr<Node<T>> root) {
    shared_ptr<Node<T>> slow = root;
    shared_ptr<Node<T>> fast = root;
    bool looping = false;
    while(slow && fast) {
        // cout << "slow [" << slow->data << "] fast [" << fast->data << "]" << endl;
        slow = slow->next;
        if(!slow)
            break;
        fast = fast->next;
        if(!fast)
            break;
        fast = fast->next;
        if(slow == fast) {
            looping = true;
            break;
        }
    }

    if(!looping)
        return -1;

    size_t loop_size = 0;
    while(true) {
        fast = fast->next;
        if(slow == fast)
            break;
        ++loop_size;
    }

    slow = fast = root;
    for(size_t i = 0; i < loop_size; ++i)
        fast = fast->next;
    cout << " .. loop size " << loop_size << endl;
    int idx = 0;
    while(slow != fast) {
        slow = slow->next;
        fast = fast->next;
        ++idx;
    }
    return idx;
}*/

struct Test {
    static int c;
    string s;
    Test() : s("my long long string" + std::to_string(c++)) {
        cout << "a const" << endl;
    }
    ~Test() {
        cout << "a destr" << endl;
    }
    Test(const Test&) = delete;
    Test(const Test&& o) {
        cout << "move const" << endl;
        s = o.s + "__";
    };
};
int Test::c = 23;

void run() {
    {
        Test a;
        cout << a.s << endl;
        Test b{std::move(a)};
        cout << b.s << endl;
        cout << a.s << endl;
    }
    auto list1 = create_list(vector<int>({1, 5, 10, 15}));
    auto list2 = create_list(vector<int>({2, 3, 5, 7, 11, 15, 16}));

    bool res = true;

    auto root = mergeSortedLists(list1, list2);
    res &= Node<int>::size(root) == 11 && Node<int>::isSorted(root);
    print(root);

    root = reverse(root);
    res &= Node<int>::size(root) == 11 && Node<int>::isSorted(root, false);
    print(root);

    auto root2 = create_list(vector<int>({1, 5, 10, 15, 19, 32, 33}));
    root2 = reverse2(root2);
    res &= Node<int>::size(root2) == 7 && Node<int>::isSorted(root2, false);

    auto root3 = create_list_loop(vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), 5);
    res &= isLooping(root2) == false;
    res &= isLooping(root3) == true;
    print(root3, 25);

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}
}  // namespace LinkedList

int main() {
    cout << "===== Linked List =====" << endl;

    LinkedList::run();

    return 0;
}
