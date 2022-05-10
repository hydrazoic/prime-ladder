#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int num_digits;
vector<int> primes;
vector<vector<int>> edges;

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i*i <= x; i++) {
        if (x % i == 0) return false;
    }
    return true;
}

void add_edges(int x) {
    vector<int> digits;
    int y = primes[x];
    while (y) {
        digits.push_back(y % 10);
        y /= 10;
    }

    for (int i = 0; i < num_digits; i++) {
        int p = pow(10, i);
        int z = primes[x] - p*digits[i];
        for (int j = 0; j < 10; j++) {
            int w = z + p*j;
            if (w == primes[x]) continue;
            auto itr = lower_bound(primes.begin(), primes.end(), w);
            if (itr != primes.end() && w == (*itr)) {
                edges[x].push_back(itr - primes.begin());
            }
        }
    }
}

vector<int> bfs(int node) {
    vector<pair<int, int>> d = vector<pair<int, int>>(primes.size(), {-1, -1});
    queue<pair<int, int>> q;
    q.push({node, -1});
    int max_dist = -1;
    int farthest = -1;

    while (!q.empty()) {
        pair<int, int> p = q.front();
        q.pop();
        if (d[p.first].first != -1) continue;
        d[p.first].second = p.second;
        int dist = (p.second == -1) ? 0 : (d[p.second].first + 1);
        d[p.first].first = dist;
        if (dist > max_dist) {
            max_dist = dist;
            farthest = p.first;
        }
        for (int e: edges[p.first]) {
            q.push({e, p.first});
        }
    }

    vector<int> ladder;
    do {
        ladder.push_back(farthest);
        farthest = d[farthest].second;
    } while (farthest != -1);
    return ladder;
}

int main() {
    cout << "Number of digits: ";
    cin >> num_digits;

    int ub = pow(10, num_digits);
    for (int x = ub / 10; x < ub; x++) {
        if (is_prime(x)) primes.push_back(x);
    }

    edges = vector<vector<int>>(primes.size());
    for (int i = 0; i < primes.size(); i++) {
        add_edges(i);
    }

    vector<int> best_ladder = {};
    for (int i = 0; i < primes.size(); i++) {
        vector<int> ladder = bfs(i);
        if (ladder.size() > best_ladder.size()) {
            best_ladder = ladder;
        }
    }

    cout << "Found ladder of length " << best_ladder.size() << " from ";
    cout << primes[best_ladder.front()] << " to ";
    cout << primes[best_ladder.back()] << "\n";
    for (int x: best_ladder) {
        cout << primes[x] << "\n";
    }
}