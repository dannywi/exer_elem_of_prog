#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

using namespace std;

// move to another file
namespace Tools {
template <typename IT>
string print(IT iter_st, IT iter_en, char glue = ',') {
    stringstream ret;
    for (auto iter = iter_st; iter != iter_en; ++iter) {
        if (iter != iter_st) ret << glue;
        ret << *iter;
    }
    return ret.str();
}
}  // namespace Tools

namespace DeleteDuplicatesFromSorted {
// just overwrite the values in place, having to iterators
// lastly do erase on excessive positions
int deleteDuplicates(vector<int>& vec) {
    if (vec.size() < 2) return 0;

    int write_i = 1;
    for (int i = 1; i < vec.size(); ++i) {
        if (vec[i] != vec[i - 1]) vec[write_i++] = vec[i];
    }

    vec.erase(vec.begin() + write_i, vec.end());

    cout << "  ... " << Tools::print(vec.begin(), vec.end(), ',') << " ... deleted " << write_i << endl;
    return write_i;
}

void run() {
    vector<int> test{1, 2, 3, 4, 5, 5, 5, 6, 7, 7, 8, 9, 9};
    bool res = true;
    res &= deleteDuplicates(test) == 9;

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}
}  // namespace DeleteDuplicatesFromSorted

namespace LargestSubArray {
template <typename T>
ostream& operator<<(ostream& o, const vector<T>& v) {
    o << "{ ";
    for (const auto& e : v) o << e << " ";
    o << "}";
    return o;
}

template <typename T, typename enable_if<is_integral<T>::value && is_unsigned<T>::value, bool>::type = true>
vector<T> solve(const vector<T>& input) {
    // assuming we keep all zeros, the whole sequence as-is is max for unsigned
    cout << "[unsigned] ... best sequence " << input << " index 0 - " << input.size() - 1 << endl;
    return input;
}

template <typename T, typename enable_if<is_integral<T>::value && is_signed<T>::value, bool>::type = true>
vector<T> solve(const vector<T>& input) {
    T T_MIN = numeric_limits<T>::min();
    // kadane's algo
    T glob_max = T_MIN;
    T curr_max = T_MIN;
    T glob_st = 0, glob_en = 0, curr_st = 0, curr_en = 0;
    for (T i = 0; i < input.size(); ++i) {
        T x = input[i];
        T curr_en = i;
        if (curr_max == T_MIN || curr_max + x < x) {
            // reset, x is better than current running
            curr_max = x;
            curr_st = curr_en;
        } else {
            curr_max += x;
        }

        if (curr_max > glob_max) {
            glob_max = curr_max;
            glob_st = curr_st;
            glob_en = curr_en;
        }
    }

    vector<T> ret;
    // for(int i = glob_st; i <= glob_en; ++i)
    //     ret.emplace_back(input[i]);
    if (input.size() > 0) copy(input.begin() + glob_st, input.begin() + glob_en + 1, back_inserter(ret));
    // copy(input.begin() + glob_st, input.begin() + glob_en + 1, inserter(ret, ret.end()));

    cout << "[kadane] ... best sequence " << ret << " index " << glob_st << " - " << glob_en << endl;
    return ret;
}

int getMaxSumOnly(const vector<int>& input) {
    if (input.empty()) return 0;
    int currMax = input[0];
    int globalMax = input[0];
    for (int i = 1; i < input.size(); ++i) {
        int v = input[i];
        // if me alone is better than the accumulated so far and me, start fresh with me only
        currMax = max(v, currMax + v);
        // update global max if current running is bigger
        globalMax = max(currMax, globalMax);
    }
    cout << "   ... max " << globalMax << endl;
    return globalMax;
}

void run() {
    // test signed integer
    struct Test {
        vector<int> input;
        vector<int> expected_output;
    };

    vector<Test> tests = {
        {{-2, 1, -3, 4, -1, 2, 1, -5, 4}, {4, -1, 2, 1}},
        {{-3, -2, -1, -2}, {-1}},
        {{0}, {0}},
        {{}, {}},
        {{2, -3, 1, 4}, {1, 4}},
    };

    bool res = true;
    for (auto test : tests) {
        res &= solve(test.input) == test.expected_output;
        res &= getMaxSumOnly(test.input) == accumulate(test.expected_output.begin(), test.expected_output.end(), 0);
    }

    // test unsigned
    vector<vector<unsigned>> tests2{
        {1, 2, 3, 3},
        {3, 0, 2, 1, 23},
        {10, 9, 8, 7, 6, 5, 0},
    };

    for (auto const& test : tests2) {
        res &= solve(test) == test;
    }

    cout << "Tests Passed: " << (res ? "YES" : "NO") << endl;
}
}  // namespace LargestSubArray

namespace MedianOfTwoSortedArrays {
// returns 0 if both arrays empty
double getMedian(vector<int> a, vector<int> b) {
    size_t total_size = a.size() + b.size();
    if (total_size == 0) return 0;

    bool even = total_size % 2 == 0;
    size_t upto = total_size / 2;
    size_t ai = 0;
    size_t bi = 0;
    int curr = 0;
    int last = 0;
    while (ai + bi <= upto) {
        last = curr;
        if (ai == a.size()) {
            curr = b[bi++];
        } else if (bi == b.size()) {
            curr = a[ai++];
        } else if (a[ai] < b[bi]) {
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
    vector<tuple<vector<int>, vector<int>, double>> tests{
        {{1, 3, 5, 7}, {2, 4, 6}, 4},
        {{1, 3, 5, 7}, {2, 4, 6, 8}, 4.5},
        {{1, 3, 5, 7}, {}, 4},
        {{}, {2, 4, 6, 8}, 5},
        {{10, 23}, {2, 4, 6, 8}, 7},
        {{10, 20}, {}, 15},
        {{20}, {10}, 15},
        {{}, {10}, 10},
        {{}, {}, 0},
    };

    bool pass = true;
    auto eq = [](double a, double b) { return abs(a - b) < 1e-6; };
    for (const auto& t : tests) {
        double res = getMedian(get<0>(t), get<1>(t));
        pass &= eq(res, get<2>(t));
    }

    cout << "Tests Passed: " << (pass ? "YES" : "NO") << endl;
}
}  // namespace MedianOfTwoSortedArrays

namespace IncrementDigits {
void increment_one(vector<int>& v) {
    if (v.empty()) {
        v.push_back(1);
        return;
    }

    for (auto r = v.rbegin(); r != v.rend(); ++r) {
        int curr = ++(*r);
        if (curr < 10) break;
        *r = 0;
    }

    if (*v.begin() == 0) {
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

    vector<Test> tests{{
        {{1, 2, 8}, 2, {1, 3, 0}},
        {{2, 0, 2, 5}, 706, {2, 7, 3, 1}},
        {{2, 3}, 11236, {1, 1, 2, 5, 9}},
    }};

    bool res = true;
    for (auto& t : tests) {
        cout << " ... " << Tools::print(t.test.begin(), t.test.end(), ',') << " + " << t.incr;
        for (size_t i = 0; i < t.incr; ++i) increment_one(t.test);

        cout << " = " << Tools::print(t.test.begin(), t.test.end(), ',') << endl;
        res &= t.test == t.expected;
    }

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}
}  // namespace IncrementDigits

namespace Partition {
template <typename T>
void partition(vector<T>& arr, std::function<bool(T)> fn) {
    if (arr.empty()) {
        return;
    }
    size_t st = 0, en = arr.size() - 1;
    while (st < en) {
        if (fn(arr[st])) {
            ++st;
        } else {
            T tmp = arr[st];
            arr[st] = arr[en];
            arr[en] = tmp;
            --en;
        }
    }
}

template <typename T>
bool is_partitioned(const vector<T>& arr, std::function<bool(T)> fn) {
    // the latch must only change once
    optional<bool> latch;
    uint8_t switched = 0;
    for (auto i : arr) {
        if (!latch.has_value()) {
            latch = fn(i);
        } else {
            bool curr = fn(i);
            if (latch != curr) {
                if (++switched > 1) {
                    return false;
                };
                latch = curr;
            }
        }
    }
    return true;
}

void run() {
    // clang-format off
    vector<vector<int>> tests{
        {1, 2, 3, 4, 5, 6},
        {4, 5, 6, 7, 9, 0},
        {4, 6, 8, 10, 12, 14},
        {3, 3, 9, 7},
        {},
    };
    // clang-format on

    auto is_even = [](int v) { return v % 2 == 0; };

    bool res = true;
    for (auto& t : tests) {
        cout << "bef: " << Tools::print(t.begin(), t.end()) << endl;
        partition<int>(t, is_even);
        cout << "aft: " << Tools::print(t.begin(), t.end()) << endl;
        res &= is_partitioned<int>(t, is_even);
    }

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}
}  // namespace Partition

namespace BigNumber {
class BigNumber {
    // most significant digit at root (negative for minus number)
    // most significant digit can be negative for negative number
    using T = int8_t;
    list<T> val_;

    // only for internal temp vars
    BigNumber() {}

   public:
    BigNumber(const char* number) {
        from_str(number);
    };

    BigNumber operator+(const BigNumber& that) const {
        if (val_.front() < 0 || that.val_.front() < 0) {
            throw("negative (i.e. subtraction) not supported yet");
        }
        BigNumber res;
        auto this_it = val_.rbegin();
        auto that_it = that.val_.rbegin();
        bool carry = false;
        while (this_it != val_.rend() || that_it != that.val_.rend()) {
            if (this_it == val_.rend()) {
                res.val_.push_front(*that_it + (carry ? 1 : 0));
                ++that_it;
                carry = false;
            } else if (that_it == that.val_.rend()) {
                res.val_.push_front(*this_it + (carry ? 1 : 0));
                ++this_it;
                carry = false;
            } else {
                T curr = *this_it + *that_it + (carry ? 1 : 0);
                carry = false;
                if (curr > 9) {
                    curr -= 10;
                    carry = true;
                }
                res.val_.push_front(curr);
                ++this_it;
                ++that_it;
            }
        }

        if (carry) {
            res.val_.push_front(1);
        }

        return res;
    }

    BigNumber operator*(const BigNumber& that) const {
        // todo: fix, implementated as addition not working
        BigNumber res{"0"};
        uint32_t mult_this = 1;
        for (auto i = val_.rbegin(); i != val_.rend(); ++i) {
            uint32_t mult_that = 1;
            for (auto a = that.val_.rbegin(); a != that.val_.rend(); ++a) {
                if (*a > 0) {
                    for (T c = 0; c < *i * mult_this; ++c) {
                        res = res + BigNumber(to_string(*a * mult_that).c_str());
                    }
                }
                mult_that *= 10;
            }
            mult_this *= 10;
        }
        return res;
    }

    bool operator==(const BigNumber& that) const {
        auto this_it = val_.begin();
        auto that_it = that.val_.begin();
        while (this_it != val_.end() || that_it != that.val_.end()) {
            if (this_it == val_.end() || that_it == that.val_.end()) {
                return false;  // different digit count
            }
            if (*this_it != *that_it) {
                return false;
            }
            ++this_it;
            ++that_it;
        }
        return true;
    }

    bool operator!=(const BigNumber& that) const {
        return !(*this == that);
    }

    string print() const {
        string s;
        for (auto c : val_) {
            s += to_string(c);
        }
        return s;
    }

    void from_str(const char* number) {
        val_.clear();
        bool negative = false;
        if (*number == '-') {
            negative = true;
            ++number;
        }

        while (*number != '\0' && '0' <= *number && *number <= '9') {
            T digit = *number - '0';
            if (negative) {
                digit *= -1;
                negative = false;  // only invert the first one
            }
            val_.push_back(digit);
            ++number;
        }

        // regard errors as zero
        if (val_.empty()) {
            val_.push_back(0);
        }
    }
};

ostream& operator<<(ostream& os, const BigNumber& v) {
    os << v.print();
    return os;
}

void run() {
    struct Test {
        BigNumber n1, n2, result_add, result_mult;
    };

    vector<Test> tests{
        {{"1234"}, {"2000"}, {"3234"}, {"2468000"}},
        {{"9999"}, {"9999"}, {"19998"}, {"99980001"}},
        {{"9"}, {"9"}, {"18"}, {"81"}},
        {{"19"}, {"10"}, {"29"}, {"190"}},
    };

    bool res = true;
    for (const auto& t : tests) {
        cout << t.n1 << " + " << t.n2 << " = " << t.result_add << endl;
        res &= t.n1 + t.n2 == t.result_add;
        // cout << t.n1 << " * " << t.n2 << " = " << t.result_mult << endl;
        // res &= t.n1 * t.n2 == t.result_mult;
    }

    cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
}
}  // namespace BigNumber

int main() {
    cout << "===== Arrays =====" << endl;

    DeleteDuplicatesFromSorted::run();
    LargestSubArray::run();
    MedianOfTwoSortedArrays::run();
    IncrementDigits::run();
    Partition::run();
    BigNumber::run();

    return 0;
}
