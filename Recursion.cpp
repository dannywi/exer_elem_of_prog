#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

namespace Permutation {
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& vec) {
    out << "{ ";
    for (auto e : vec) out << e << " ";
    out << "}";
    return out;
}

void fillPermutation(int i, vector<int>& elems, vector<vector<int>>& combi) {
    if (i == elems.size() - 1) {
        // cout << " ... add " << elems << endl;
        combi.emplace_back(elems);
        return;
    }
    for (int a = i; a < elems.size(); ++a) {
        // cout << " ... i [" << i << "] - a [" << a << "] " << endl;
        swap(elems[a], elems[i]);
        fillPermutation(i + 1, elems, combi);
        swap(elems[a], elems[i]);
    }
}

// not working
// void fillPermutation2(int st_index, vector<int>& elems, vector<vector<int>>& combi) {
//     //cout << " ... idx [" << st_index << "] - " << elems << endl;
//     combi.emplace_back(elems);
//     cout << " ... elem " << elems << endl;
//     for(int i = st_index + 1; i < elems.size(); ++i) {
//         cout << " ... st [" << st_index << "] - i [" << i << "] " << endl;
//         swap(elems[i], elems[st_index]);
//         fillPermutation2(i, elems, combi);
//         swap(elems[i], elems[st_index]);
//     }
// }

vector<vector<int>> solve(vector<int> elems) {
    cout << "== Permutation " << elems << " ==" << endl;

    vector<vector<int>> combi;
    fillPermutation(0, elems, combi);

    for (auto c : combi) {
        cout << c << endl;
    }
    return combi;
}

bool test() {
    struct Test {
        vector<int> elems;
        int expected_count;
    };

    vector<Test> tests{{
        {{}, 0},
        {{1, 2, 3, 4}, 24},
        {{1, 2}, 2},
        {{1, 2, 3}, 6},
    }};

    bool res = true;
    for (auto test : tests) {
        vector<vector<int>> combi = solve(test.elems);
        res &= combi.size() == test.expected_count;
    }
    return res;
}
}  // namespace Permutation

int main() {
    cout << " ==== RECURSION ==== " << endl;
    bool res = true;
    res &= Permutation::test();

    cout << "ALL TESTS " << (res ? "PASSED" : "NOT PASSED") << endl;
    return 0;
}