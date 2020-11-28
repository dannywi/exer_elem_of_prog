#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

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

namespace StrToInt {
    struct Case { int i; string s; };

    int stoi(string s) {
        if(s.empty())
            throw("NaN");

        bool isNeg = s[0] == '-';
        if( isNeg && s.size() == 1)
            throw("NaN");

        int res = 0;
        for(int i = isNeg ? 1 : 0; i < s.size(); ++i) {
            int cur = s[i] - '0';
            if(cur < 0 || cur > 9)
                throw("NaN");
            res = res * 10 + cur;
        }

        if(isNeg)
            res *= -1;

        cout << " -- str [" << s << "] => int [" << res << "]" << endl;
        return res;
    }

    string itos(int i) {
        string s;
        int x = i;
        do {
            char c = '0' + abs(x % 10);
            s += c;
            x /= 10;
        } while(x != 0);

        if(i < 0)
            s += '-';

        //return { s.rbegin(), s.rend() };
        reverse(s.begin(), s.end());
        cout << " -- int [" << i << "] => str [" << s << "]" << endl;
        return s;
    }

    void run() {
        vector<Case> test {
            { 123,          "123" },
            { 234234000,    "234234000" },
            { -23000,       "-23000" },
            { 0,            "0" },
            { 2,            "2" },
            { -10,          "-10" },
        };

        bool res = true;

        for_each(test.begin(), test.end(), [&](Case c) { res &= StrToInt::stoi(c.s) == c.i; });
        for_each(test.begin(), test.end(), [&](Case c) { res &= StrToInt::itos(c.i) == c.s; });

        //string t = "abcd";
        //string tt = {t.rbegin(), t.end()};
        //cout << t << " - " << tt << endl;
        cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

namespace InvertStr {
    string invert(const string& input) {
        string res = input;
        int l = 0, r = input.size() - 1;
        while(l < r)
            swap(res[l++], res[r--]);

        cout << " -- [" << input << "] => [" << res << "]" << endl;
        return res;
    }

    void run() {
        struct Case { string input; string expected_output; };
        vector<Case> tests = {
            { "abcd", "dcba" },
            { "abc", "cba" },
            { "ab", "ba" },
            { "a", "a" },
            { "", "" },
        };

        bool res = true;
        for(auto test : tests) {
            res &= invert(test.input) == test.expected_output;
        }

        cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

namespace ConvertBase {
    string solve(const string& str, int fr, int to) {
        if(str.size() == 0)
            return "";

        string ret;
        bool neg = str[0] == '-';
        int st_index = (neg ? 1 : 0);

        // get decimal rep
        int num = 0;
        for(int i = st_index; i < str.size(); ++i) {
            char c = str[i];
            int digit = isdigit(c) ? c - '0' : c - 'A' + 10;
            num = num * fr + digit;
        }

        // construct target rep
        int base10 = num;
        do {
            int digit = num % to;
            ret += digit >= 10 ? 'A' + digit - 10 : '0' + digit;
            num /= to;
        } while(num > 0);

        if(neg)
            ret += '-';

        reverse(ret.begin(), ret.end());
        cout << " -- [base" << fr << "] " << str << " => (" << base10 << ") => [base" << to << "] " << ret << endl;
        return ret;
    }

    void run() {
        struct Test { string fr_str; int fr; int to; string to_str; };
        vector<Test> tests = {
            { "12", 10, 16, "C" },
            { "AB", 16, 2, "10101011" },
            { "-23000", 10, 16, "-59D8" },
            { "0", 10, 16, "0" },
        };

        bool res = true;
        for(auto test : tests) {
            res &= solve(test.fr_str, test.fr, test.to) == test.to_str;
            res &= solve(test.to_str, test.to, test.fr) == test.fr_str;
        }

        cout << "All Tests Passed: " << (res ? "YES" : "NO") << endl;
    }
}

int main() {
    cout << "===== Strings =====" << endl;

    StrToInt::run();
    InvertStr::run();
    ConvertBase::run();

    return 0;
}