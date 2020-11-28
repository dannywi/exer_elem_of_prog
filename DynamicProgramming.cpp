#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

namespace Knapsack {
    struct Item {
        int weight;
        int value;

        bool operator==(const Item& that) const {
            return this->weight == that.weight && this->value && that.value;
        }

        friend ostream& operator<<(ostream& out, const Item& item) {
            out << "{ value[" << item.value << "] weight[" << item.weight << "] }";
            return out;
        }
    };

    auto solve(const vector<Item>& items, int max_weight) {
        vector<vector<vector<Item>>> matrix(items.size() + 1, vector<vector<Item>>(max_weight + 1, vector<Item>()));

        auto total_value = [&matrix](int row, int col) {
            const vector<Item>& it = matrix[row][col];
            return accumulate(it.begin(), it.end(), 0, [](int acc, const Item& x) { return acc + x.value; });
        };

        for(int i = 0, m = 1; i < items.size(); ++i, ++m) {
            const auto& item = items[i];
            for(int a = 0; a <= max_weight; ++a) matrix[m].push_back(vector<Item>());

            for(int w = 0; w <= max_weight; ++w) {
                int valueWithoutItem = total_value(m - 1, w);
                int valueWithItem = 0;
                vector<Item> listWithItem;
                if(w >= item.weight) {
                    valueWithItem = item.value;
                    listWithItem.push_back(item);
                    int remainingWeight = w - item.weight;
                    valueWithItem += total_value(m, remainingWeight);
                    for(const auto& it : matrix[m][remainingWeight])
                        listWithItem.push_back(it);
                }
                if(valueWithoutItem > valueWithItem)
                    matrix[m][w] = matrix[m - 1][w];
                else
                    matrix[m][w] = listWithItem;
            }
        }

        return matrix[items.size()][max_weight];
    }

    // 1. set matrix of items (rows) and weights (cols), patch zero row / col for buffer
    // 2. for each item:
    //    - for each weight:
    //      - compare if it's better to add me or not (if I can get in)
    //        - not using me is the matrix value one above me)
    //        - using me means add my value, and get the must-have-been-calculated best value in remaining weight column
    int solveValueOnly(const vector<Item>& items, int max_weight) {
        vector<vector<int>> matrix(items.size() + 1, vector<int>(max_weight + 1, 0));

        // mi / mw -> index of items / weight in matrix, since we have one extra row / col for buffer
        for(int mi = 1; mi <= items.size(); ++mi) {
            for(int mw = 1; mw <= max_weight; ++mw) {
                // value without me is the value above in the matrix (value up until prev item)
                int valueWithoutItem = matrix[mi - 1][mw];

                // value with me is my value plus the best use of remaining weight
                int valueWithItem = 0;
                if(mw >= items[mi - 1].weight) {
                    valueWithItem = items[mi - 1].value;
                    int remainingWeight = mw - items[mi - 1].weight;
                    valueWithItem += matrix[mi][remainingWeight];
                }

                matrix[mi][mw] = max(valueWithoutItem, valueWithItem);
            }
        }
        int ret = matrix[items.size()][max_weight];
        cout << "   ... solveValueOnly " << ret << endl;
        return ret;
    }

    bool test() {
        struct Test {
            vector<Item> in_items;
            int max_weight;
            vector<Item> expected_items;
            int expected_value;
        };

        vector<Test> tests = {
            {
                { { 5, 60 }, { 3, 50 }, { 4, 70 }, { 2, 30 }, },    5,
                { { 3, 50 }, { 2, 30 }, },                          80,
            },
            { // change item sequence, should give same answer
                { { 4, 70 }, { 3, 50 }, { 2, 30 }, { 5, 60 }, },    5,
                { { 3, 50 }, { 2, 30 }, },                          80,
            },
        };

        bool res = true;
        for(auto test : tests) {
            cout << "---------- items count " << test.in_items.size() << " max weight " << test.max_weight << endl;
            auto items = solve(test.in_items, test.max_weight);
            auto comp = [](const Item& a, const Item& b) { return a.weight < b.weight; };
            sort(items.begin(), items.end(), comp);
            sort(test.expected_items.begin(), test.expected_items.end(), comp);
            int value = accumulate(items.begin(), items.end(), 0, [](int acc, const Item& x) { return acc + x.value; });
            res &= items == test.expected_items && value == test.expected_value;

            res &= test.expected_value == solveValueOnly(test.in_items, test.max_weight);

            cout << " total value " << value << endl;
            for_each(items.begin(), items.end(), [](const Item& i) { cout << " ..... " << i << endl; });
        }

        cout << "Knapsack Tests: " << (res ? "ALL PASSED" : "FAILED") << endl;
        return res;
    }
}

namespace CoinCombi {
    int solve(const vector<int>& coins, int total) {
        vector<vector<int>> matrix; // [coin index][total index]

        // init first dummy row
        matrix.push_back(vector<int>());
        for(int t = 0; t <= total; ++t)
            matrix.back().push_back(0);

        for(int i = 1; i <= coins.size(); ++i) {
            matrix.push_back(vector<int>());
            int coin = coins[i - 1];
            matrix.back().push_back(1); // zero total can be made with zero of any coin
            for(int t = 1; t <= total; ++t) {
                int with_me = t >= coin ? matrix[i][t - coin] : 0;
                int without_me = matrix[i - 1][t];
                matrix.back().push_back(with_me + without_me);
            }
        }

        return matrix.back().back();
    }

    bool test() {
        struct Test {
            vector<int> coins;
            int total;
            int expected_combi_count;
        };

        vector<Test> tests {
            { { 2, 3, 7 }, 12, 4 },
        };

        bool res = true;
        for(auto test : tests) {
            int res = solve(test.coins, test.total);
            res &= res == test.expected_combi_count;
        }
        cout << "Coins Combinations Tests: " << (res ? "ALL PASSED" : "FAILED") << endl;
        return res;
    }
}


void run() {
    bool res = true;
    res &= Knapsack::test();
    res &= CoinCombi::test();
    cout << "--- " << (res ? "" : "NOT") << " ALL PASSED" << endl;
}

int main() {
    cout << "==== Dynamic Programming ====" << endl;
    run();
    return 0;
}