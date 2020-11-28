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
    bool bsearch(T v, const vector<T>& vec) {
        int a = 0, b = vec.size() - 1;
        while(a <= b) {
            int m = a + (b - a) / 2;
            if(vec[m] == v)
                return true;
            if(vec[m] < v)
                a = m + 1;
            else
                b = m - 1;
        }
        return false;
    }

    template<typename T>
    int first_occurence(T v, vector<T> vec) {
        int a = 0, b = vec.size() - 1;
        int res_index = -1;
        while(a <= b) {
            int m = a + (b - a) / 2;
            if(vec[m] == v) {
                res_index = m;
                b = m - 1;
            } else if(vec[m] < v) {
                a = m + 1;
            } else {
                b = m - 1;
            }
        }
        return res_index;
    }
}

bool test_search() {
    cout << "## " << __func__ << " ##" << endl;
    bool res = true;

    vector<int> vec(12);
    int range = 20;

    for(int i = 1; i <= 5; ++i) {
        for_each(vec.begin(), vec.end(), [&range](int& x) { x = rand() % range; });
        sort(vec.begin(), vec.end());

        // check answer using linear search
        int v = rand() % range;
        bool check = false;
        for_each(vec.begin(), vec.end(), [&](int x) { check |= x == v; });

        bool found = Impl::bsearch(v, vec);
        cout << "  INP: ";
        for_each(vec.begin(), vec.end(), [](int x) { cout << x << " "; });
        cout << " - find[" << v << "] => " << found << endl;

        res &= check == found;
    }

    return res;
}

bool test_first_occurence() {
    cout << "## " << __func__ << " ##" << endl;
    bool res = true;

    vector<int> vec(10);
    int range = 7;

    for(int i = 1; i <= 15; ++i) {
        for_each(vec.begin(), vec.end(), [&range](int& x) { x = rand() % range; });
        sort(vec.begin(), vec.end());

        int v = rand() % range;

        // check answer using linear search
        int ans = -1;
        for(int a = 0; a < vec.size(); ++a) {
            if(vec[a] == v) {
                ans = a;
                break;
            }
        }

        int index = Impl::first_occurence(v, vec);
        cout << "  INP: ";
        for_each(vec.begin(), vec.end(), [](int x) { cout << x << " "; });
        cout << " - find[" << v << "] => " << index << endl;

        res &= ans == index;
    }

    return res;
}

void run() {
    bool res = true;

    res &= test_search();
    res &= test_first_occurence();

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}

int main() {
    cout << "===== Stack And Queues =====" << endl;

    run();

    return 0;
}