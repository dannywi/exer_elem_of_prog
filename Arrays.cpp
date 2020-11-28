#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std;
// move to another file
namespace Tools {
    template<typename IT>
    string print(IT iter_st, IT iter_en, char glue) {
        stringstream ret;
        for(auto iter = iter_st; iter != iter_en; ++iter) {
            if(iter != iter_st)
                ret << glue;
            ret << *iter;
        }
        return ret.str();
    }
}

namespace DeleteDuplicatesFromSorted {

    int deleteDuplicates(vector<int>& vec) {
        if(vec.size() < 2)
            return 0;

        int write_i = 1;
        for(int i = 1; i < vec.size(); ++i) {
            if(vec[i] != vec[i - 1])
                vec[write_i++] = vec[i];
        }

        vec.erase(vec.begin() + write_i, vec.end());

        cout << "  ... " << Tools::print(vec.begin(), vec.end(), ',') << " ... deleted " << write_i << endl;
        return write_i;
    }

    void run() {
        vector<int> test {1, 2, 3, 4, 5, 5, 5, 6, 7, 7, 8, 9, 9};
        bool res = true;
        res &= deleteDuplicates(test) == 9;

        cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

namespace LargestSubArray {
    ostream& operator<<(ostream& o, const vector<int>& v) {
        o << "{ ";
        for(const auto& e : v)
            o << e << " ";
        o << "}";
        return o;
    }

    vector<int> solve(const vector<int>& input) {
        // kadane's algo
        int glob_max = numeric_limits<int>::min();
        int curr_max = numeric_limits<int>::min();
        int glob_st = 0, glob_en = 0, curr_st = 0, curr_en = 0;
        for(int i = 0; i < input.size(); ++i) {
            int x = input[i];
            int curr_en = i;
            if(curr_max == numeric_limits<int>::min() || curr_max + x < x) {
                // reset, x is better than current running
                curr_max = x;
                curr_st = curr_en;
            } else {
                curr_max += x;
            }

            if(curr_max > glob_max) {
                glob_max = curr_max;
                glob_st = curr_st;
                glob_en = curr_en;
            }
        }

        vector<int> ret;
        //for(int i = glob_st; i <= glob_en; ++i)
        //    ret.emplace_back(input[i]);
        if(input.size() > 0)
            copy(input.begin() + glob_st, input.begin() + glob_en + 1, back_inserter(ret));
            //copy(input.begin() + glob_st, input.begin() + glob_en + 1, inserter(ret, ret.end()));

        cout << " ... best sequence " << ret << " index " << glob_st << " - " << glob_en << endl;
        return ret;
    }

    void run() {
        struct Test {
            vector<int> input;
            vector<int> expected_output;
        };

        vector<Test> tests = {
            { {-2, 1, -3, 4, -1, 2, 1, -5, 4 }, { 4, -1, 2, 1 } },
            { {-3, -2, -1, -2 }, { -1 } },
            { { 0 }, { 0 } },
            { {}, {} },
            { { 2, -3, 1, 4 }, { 1, 4 }},
        };

        bool res = true;
        for(auto test: tests)
            res &= solve(test.input) == test.expected_output;

        cout << "Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

namespace MedianOfTwoSortedArrays {
    // returns 0 if both arrays empty
    double getMedian(vector<int> a, vector<int> b) {
        size_t total_size = a.size() + b.size();
        if(total_size == 0)
            return 0;

        bool even = total_size % 2 == 0;
        size_t upto = total_size / 2;
        size_t ai = 0;
        size_t bi = 0;
        int curr = 0;
        int last = 0;
        while(ai + bi <= upto) {
            last = curr;
            if(ai == a.size()) {
                curr = b[bi++];
            } else if(bi == b.size()) {
                curr = a[ai++];
            } else if(a[ai] < b[bi]) {
                curr = a[ai++];
            } else {
                curr = b[bi++];
            }
        }

        double res = even ? (last + curr) / 2.0 : curr;
        cout << "  median = " << res << endl;
        return res;
    }

    void run() {
        // vector 1, vector 2, expected median
        vector<tuple<vector<int>,vector<int>,double>> tests {
            { {1, 3, 5, 7}, {2, 4, 6}, 4 },
            { {1, 3, 5, 7}, {2, 4, 6, 8}, 4.5 },
            { {1, 3, 5, 7}, {}, 4 },
            { {}, {2, 4, 6, 8}, 5 },
            { {10, 23}, {2, 4, 6, 8}, 7 },
            { {10, 20}, {}, 15 },
            { {20}, {10}, 15 },
            { {}, {10}, 10 },
            { {}, {}, 0 },
        };

        bool pass = true;
        auto eq = [](double a, double b) { return abs(a - b) < 1e-6; };
        for(const auto& t : tests) {
            double res = getMedian(get<0>(t), get<1>(t));
            pass &= eq(res, get<2>(t));
        }

        cout << "Tests Passed: " << (pass ? "YES" : "NO") << endl;
    }
}


namespace IncrementDigits {
    void increment_one(vector<int>& v) {
        if(v.empty()) {
            v.push_back(1);
            return;
        }

        int carry = 0;
        for(auto r = v.rbegin(); r != v.rend(); ++r) {
            int curr = ++(*r);
            if(curr < 10)
                break;
            *r = 0;
        }

        if(*v.begin() == 0) {
            reverse(v.begin(), v.end());
            v.push_back(1);
            reverse(v.begin(), v.end());
        }

    }

    void run() {
        struct Test {
            vector<int> test;
            int incr;
            vector<int> expected;
        };

        vector<Test> tests {
            {
                { { 1, 2, 8 },        2,    { 1, 3, 0 } },
                { { 2, 0, 2, 5 },   706,    { 2, 7, 3, 1 } },
                { { 2, 3 },       11236,    { 1, 1, 2, 5, 9 } },
            }
        };

        bool res = true;
        for(auto& t : tests) {
            cout << " ... " << Tools::print(t.test.begin(), t.test.end(), ',') << " + " << t.incr;
            for(size_t i = 0; i < t.incr; ++i)
                increment_one(t.test);

            cout << " = " << Tools::print(t.test.begin(), t.test.end(), ',') << endl;
            res &= t.test == t.expected;
        }

        cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

int main() {
    cout << "===== Arrays =====" << endl;

    DeleteDuplicatesFromSorted::run();
    LargestSubArray::run();
    MedianOfTwoSortedArrays::run();
    IncrementDigits::run();

    return 0;
}