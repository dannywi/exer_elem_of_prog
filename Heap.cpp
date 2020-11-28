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
    template<typename T, typename COMP, typename UNDEF>
    class Heap {
        vector<T> values;

        static int parent(int i) { return (i - 1) / 2; }
        static int left(int i) { return i * 2 + 1; }
        static int right(int i) { return i * 2 + 2; }

        void swap(int a, int b) { T tmp = values[a]; values[a] = values[b]; values[b] = tmp; }

        COMP comp;
        UNDEF undef_getter;

    public:
        Heap(COMP comp, UNDEF undef_getter) {
            this->comp = comp;
            this->undef_getter = undef_getter;
        }

        void insert(const T& v) {
            values.push_back(v);
            int i = values.size() - 1;

            while(i != 0 && !comp(values[parent(i)], values[i])) {
                swap(i, parent(i));
                i = parent(i);
            }
        }

        T top() const { return values.empty() ? undef_getter() : values.front(); }

        void pop() {
            if(values.empty())
                return;

            values[0] = values.back();
            values.pop_back();

            int i = 0;
            while(i < values.size()) {
                int smallest = i;
                if(left(i) < values.size() && !comp(values[i], values[left(i)]))
                    smallest = left(i);
                if(right(i) < values.size() && !comp(values[smallest], values[right(i)]))
                    smallest = right(i);
                if(smallest == i)
                    break;
                swap(i, smallest);
                i = smallest;
            }
        }

        bool empty() const { return values.empty(); }
        int size() const { return values.size(); }
    };

    vector<double> running_median(const vector<int>& inputs) {
        vector<double> res;
        Heap<int, function<bool(int, int)>, function<int()>> min_heap([](int a, int b) { return a < b; }, []() { return INT_MAX; });
        Heap<int, function<bool(int, int)>, function<int()>> max_heap([](int a, int b) { return a > b; }, []() { return INT_MIN; });

        cout << "  RUN: ";
        for(auto v : inputs) {
            //cout << " .. value " << v << endl;
            // build cache - insert value
            if(min_heap.empty() || min_heap.top() < v)
                min_heap.insert(v);
            else
                max_heap.insert(v);

            //cout << "   .. min - top[" << min_heap.top() << "] size[" << min_heap.size() << "], max top[" << max_heap.top() << "] size[" << max_heap.size() << "]" << endl;

            //   balance the heaps
            if(min_heap.size() > max_heap.size() + 1) {
                //cout << "   .. move from min to max " << min_heap.top() << endl;
                max_heap.insert(min_heap.top());
                min_heap.pop();
            } else if(max_heap.size() > min_heap.size()) {
                //cout << "   .. move from max to min " << max_heap.top() << endl;
                min_heap.insert(max_heap.top());
                max_heap.pop();
            }

            // fetch latest median
            double running_median = (min_heap.size() + max_heap.size()) % 2 == 0 ?
                (min_heap.top() + max_heap.top()) / 2.0 :
                min_heap.top();

            res.push_back(running_median);
            cout << running_median << " ";
            //cout << "    .. running med " << running_median << endl;
        }

        cout << endl;
        return res;
    }
}

bool test_heap() {
    cout << "## " << __func__ << " ##" << endl;
    bool res = true;

    vector<int> inputs { 2, 3, 5, 4, 4, 3, 1, 7, 6, 8 };
    cout << "  Inputs: ";
    for_each(inputs.begin(), inputs.end(), [&](int x) { cout << x << " "; });
    cout << endl;

    {
        auto less = [](int a, int b) { return a < b; };
        Impl::Heap<int, function<bool(int, int)>, function<int()>> min_hp(less, []() { return INT_MAX; }); // can just pass INT_MAX as T
        for_each(inputs.begin(), inputs.end(), [&](int x) { min_hp.insert(x); });
        cout << "  MIN: ";
        int last = INT_MIN;
        for(int i = 0; i < inputs.size(); ++i) {
            int top = min_hp.top();
            cout << top << " ";
            res &= less(last, top);
            min_hp.pop();
        };
        cout << endl;
    }

    {
        auto greater = [](int a, int b) { return a > b; };
        Impl::Heap<int, function<bool(int, int)>, function<int()>> max_hp(greater, []() { return INT_MIN; });
        for_each(inputs.begin(), inputs.end(), [&](int x) { max_hp.insert(x); });

        cout << "  MAX: ";
        int last = INT_MAX;
        for(int i = 0; i < inputs.size(); ++i) {
            int top = max_hp.top();
            cout << top << " ";
            res &= greater(last, top);
            max_hp.pop();
        };
        cout << endl;
    }

    return res;
}

bool test_running_median() {
    cout << "## " << __func__ << " ##" << endl;
    bool res = true;

    vector<int> inputs(10);

    for(int i = 0; i < 5; ++i) {
        cout << " ----------- " << i + 1 << endl;

        for_each(inputs.begin(), inputs.end(), [](auto& x) { x = rand() % 100; });
        cout << "  Inputs: ";
        for_each(inputs.begin(), inputs.end(), [&](int x) { cout << x << " "; });
        cout << endl;

        vector<int> tmp;
        vector<double> test_check;
        cout << "  ANS: ";
        for(auto v : inputs) {
            tmp.push_back(v);
            sort(tmp.begin(), tmp.end());
            double running_med = tmp.size() % 2 == 0 ?
                (tmp[tmp.size() / 2 - 1] + tmp[tmp.size() / 2]) / 2.0 :
                tmp[tmp.size() / 2];
            test_check.push_back(running_med);
            cout << running_med << " ";
        }
        cout << endl;

        res &= Impl::running_median(inputs) == test_check;
    }

    return res;
}

void run() {
    bool res = true;

    res &= test_heap();
    res &= test_running_median();

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}

int main() {
    cout << "===== Stack And Queues =====" << endl;

    run();

    string s;
    ostream o(&s);
    o << " ab " << " c ";
    o << " d ";
    o << endl;
    std::cout << o;

    return 0;
}