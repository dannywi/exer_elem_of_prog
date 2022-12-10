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
class StackWithMax {
    struct Node {
        T data;
        T prev_max;
        shared_ptr<Node> prev;
        ~Node() {
            cout << " .. deleting .. " << data << endl;
        }
    };

    shared_ptr<Node> last_;
    int size_ = 0;
    T max_ = 0;  // assuming T can do more/less than

   public:
    void push(const T& v) {
        shared_ptr<Node> n(new Node());
        n->data = v;
        n->prev = last_;
        n->prev_max = max_;
        last_ = n;
        ++size_;
        if (max_ < v) {
            max_ = v;
        }
    }

    T pop() {
        if (last_ == nullptr) throw("empty");
        shared_ptr<Node> old_last = last_;
        last_ = last_->prev;
        max_ = old_last->prev_max;
        --size_;
        return old_last->data;
    }

    T peek() const {
        if (last_ == nullptr) throw("empty");
        return last_->data;
    }

    bool empty() const {
        return last_ == nullptr;
    }
    int size() const {
        return size_;
    }
    T max() const {
        return max_;
    }
};

template <typename T>
class QueueWithMax {
    struct Node {
        T data;
        shared_ptr<Node> next = nullptr;
        ~Node() {
            cout << " .. deleting .. " << data << endl;
        }
    };

    shared_ptr<Node> first_ = nullptr;
    shared_ptr<Node> last_ = nullptr;
    int size_ = 0;

    deque<T> max_deque_;

    void check_empty() const {
        if (first_ == nullptr) throw runtime_error("empty queue");
    }

   public:
    T peek() const {
        check_empty();
        return first_->data;
    }

    T pop() {
        check_empty();
        shared_ptr<Node> tmp = first_;
        if (first_ == last_) last_ = nullptr;
        // cout << " ... trying to pop " << first_->data << endl;
        first_ = first_->next;
        --size_;

        if (tmp->data == max_deque_.front()) {
            cout << "   removed max " << tmp->data << endl;
            max_deque_.pop_front();
        }
        return tmp->data;
    }

    void push(const T& val) {
        shared_ptr<Node> new_node(new Node());
        new_node->data = val;
        if (last_ != nullptr)
            last_->next = new_node;
        else
            first_ = new_node;
        last_ = new_node;
        ++size_;

        while (!max_deque_.empty() && max_deque_.back() < val) max_deque_.pop_back();
        max_deque_.push_back(val);
    }

    bool empty() const {
        return first_ == nullptr;
    }
    int size() const {
        return size_;
    }
    T max() const {
        check_empty();
        return max_deque_.front();
    }
};
}  // namespace Impl

bool test_stack() {
    cout << "** TESTING STACK **" << endl;
    bool res = true;

    Impl::StackWithMax<int> st;
    vector<int> inputs{2, 3, 4, 5, 4};
    for_each(inputs.begin(), inputs.end(), [&](int x) { st.push(x); });

    // normal operations
    cout << "size = " << st.size() << endl;
    res &= st.size() == inputs.size();

    cout << "top -> " << st.peek() << endl;
    res &= st.peek() == inputs.back();

    for_each(inputs.begin(), inputs.end(), [&](int x) {
        int y = st.pop();
        cout << "popped -> " << y << endl;
    });
    cout << "empty = " << st.empty() << endl;
    res &= st.empty() == true;

    // check max
    for_each(inputs.begin(), inputs.end(), [&](int x) { st.push(x); });
    cout << "max -> " << st.max() << endl;
    res &= st.max() == 5;

    st.pop();
    cout << "max -> " << st.max() << endl;
    res &= st.max() == 5;

    st.pop();
    cout << "max -> " << st.max() << endl;
    res &= st.max() == 4;

    st.pop();
    cout << "max -> " << st.max() << endl;
    res &= st.max() == 3;

    return res;
}

bool test_queue() {
    cout << "** TESTING QUEUE **" << endl;
    bool res = true;

    Impl::QueueWithMax<int> qu;
    vector<int> inputs{1, 2, 3, 4, 5, 4, 3, 5, 3, 2};
    deque<int> maxtest{5, 5, 5, 5, 5, 5, 5, 5, 3, 2};
    for_each(inputs.begin(), inputs.end(), [&](int x) { qu.push(x); });

    // normal operations
    cout << "size = " << qu.size() << endl;
    res &= qu.size() == inputs.size();

    cout << "top -> " << qu.peek() << endl;
    res &= qu.peek() == *inputs.begin();

    for_each(inputs.begin(), inputs.end(), [&](int x) {
        int y = qu.pop();
        cout << "popped -> " << y << endl;
    });
    cout << "empty = " << qu.empty() << endl;
    res &= qu.empty() == true;

    // check max
    for_each(inputs.begin(), inputs.end(), [&](int x) { qu.push(x); });
    for_each(inputs.begin(), inputs.end(), [&](int x) {
        int max = qu.max();
        int max_raw = maxtest.front();
        maxtest.pop_front();
        cout << "max [" << max << "] vs max_raw [" << max_raw << "]" << endl;
        int y = qu.pop();
        cout << "popped -> " << y << endl;
        res &= max == max_raw;
    });

    return res;
}

void run() {
    bool res = true;

    res &= test_stack();
    res &= test_queue();

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}

int main() {
    cout << "===== Stack And Queues =====" << endl;

    run();

    return 0;
}