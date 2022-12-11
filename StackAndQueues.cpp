#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

namespace Impl {

// Integral comparisons
template <typename T>
typename enable_if<is_integral<T>::value, bool>::type less(T a, T b) {
    return a < b;
}

template <typename T>
typename enable_if<is_integral<T>::value, bool>::type more(T a, T b) {
    return a > b;
}

// Floating comparisons
template <typename T>
constexpr typename enable_if<is_floating_point<T>::value, T>::type epsilon() {
    return 1e-6;
}

template <typename T>
typename enable_if<is_floating_point<T>::value, bool>::type less(T a, T b) {
    return epsilon<T>() < b - a;
}

template <typename T>
typename enable_if<is_floating_point<T>::value, bool>::type more(T a, T b) {
    return epsilon<T>() < a - b;
}

template <typename T>
typename enable_if<is_arithmetic<T>::value, bool>::type same(T a, T b) {
    return !less(a, b) && !more(a, b);
}

template <typename T, typename = typename enable_if<is_arithmetic<T>::value>::type>
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
    size_t size_ = 0;
    T max_ = numeric_limits<T>::min();

   public:
    void push(const T& v) {
        shared_ptr<Node> n{make_shared<Node>()};
        n->data = v;
        n->prev = last_;
        n->prev_max = max_;
        last_ = n;
        ++size_;
        if (less(max_, v)) {
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
    size_t size() const {
        return size_;
    }
    T max() const {
        return max_;
    }
};

template <typename T, typename = typename enable_if<is_arithmetic<T>::value>::type>
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
    size_t size_ = 0;

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
        shared_ptr<Node> new_node{make_shared<Node>()};
        new_node->data = val;
        if (last_ != nullptr)
            last_->next = new_node;
        else
            first_ = new_node;
        last_ = new_node;
        ++size_;

        while (!max_deque_.empty() && less(max_deque_.back(), val)) max_deque_.pop_back();
        max_deque_.push_back(val);
    }

    bool empty() const {
        return first_ == nullptr;
    }
    size_t size() const {
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

    using T = double;
    static_assert(is_arithmetic<T>::value);
    Impl::StackWithMax<T> st;
    vector<T> inputs{2, 3, 4, 5, 4};
    for_each(inputs.begin(), inputs.end(), [&](T x) { st.push(x); });

    // normal operations
    cout << "size = " << st.size() << endl;
    res &= st.size() == inputs.size();

    cout << "top -> " << st.peek() << endl;
    res &= Impl::same(st.peek(), inputs.back());

    for_each(inputs.begin(), inputs.end(), [&](T x) {
        T y = st.pop();
        cout << "popped -> " << y << endl;
    });
    cout << "empty = " << st.empty() << endl;
    res &= st.empty() == true;

    // check max
    for_each(inputs.begin(), inputs.end(), [&](T x) { st.push(x); });
    cout << "max -> " << st.max() << endl;
    res &= Impl::same(st.max(), (T)5);

    st.pop();
    cout << "max -> " << st.max() << endl;
    res &= Impl::same(st.max(), (T)5);

    st.pop();
    cout << "max -> " << st.max() << endl;
    res &= Impl::same(st.max(), (T)4);

    st.pop();
    cout << "max -> " << st.max() << endl;
    res &= Impl::same(st.max(), (T)3);

    return res;
}

bool test_queue() {
    cout << "** TESTING QUEUE **" << endl;
    bool res = true;

    using T = uint16_t;
    static_assert(is_arithmetic<T>::value);
    Impl::QueueWithMax<T> qu;
    vector<T> inputs{1, 2, 3, 4, 5, 4, 3, 5, 3, 2};
    deque<T> maxtest{5, 5, 5, 5, 5, 5, 5, 5, 3, 2};
    for_each(inputs.begin(), inputs.end(), [&](T x) { qu.push(x); });

    // normal operations
    cout << "size = " << qu.size() << endl;
    res &= qu.size() == inputs.size();

    cout << "top -> " << qu.peek() << endl;
    res &= Impl::same(qu.peek(), *inputs.begin());

    for_each(inputs.begin(), inputs.end(), [&](T x) {
        T y = qu.pop();
        cout << "popped -> " << y << endl;
    });
    cout << "empty = " << qu.empty() << endl;
    res &= qu.empty() == true;

    // check max
    for_each(inputs.begin(), inputs.end(), [&](T x) { qu.push(x); });
    for_each(inputs.begin(), inputs.end(), [&](T x) {
        T max = qu.max();
        T max_raw = maxtest.front();
        maxtest.pop_front();
        cout << "max [" << max << "] vs max_raw [" << max_raw << "]" << endl;
        T y = qu.pop();
        cout << "popped -> " << y << endl;
        res &= Impl::same(max, max_raw);
    });

    return res;
}

void run() {
    bool res = true;

    assert(res &= test_stack());
    assert(res &= test_queue());

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}

int main() {
    cout << "===== Stack And Queues =====" << endl;

    run();

    return 0;
}
