#include "../src/segment_tree.h"

/*
 * Represents a typical segment tree usage.
 *
 * This example returns the number of k-inversions in a given permutation. (% MOD)
 * K-inversion is a set of indexes i(1) to i(k) that satisfies both:
 *  - 1 <= i(1) < i(2) < ... < i(k) <= n
 *  - a[i(1)] > a[i(2)] > ... > a[i(k)]
 */

constexpr long long MOD = 1000000000;

void k_inversions() {
    using Tree = algo_lib::tree::segment_tree<unsigned long>;

    unsigned long n, k;
    std::cin >> n >> k;

    std::vector<unsigned long> vi;
    std::vector<Tree> trees;
    auto modulo_adder = [&] (const unsigned long& lhs, const unsigned long& rhs) { return (lhs + rhs) % MOD; };
    auto without_leaf_update = [] (const unsigned long& acc, const unsigned long& updater) { return updater; };

    for (unsigned long i = 0; i < k; ++i) {
        trees.emplace_back(n, 0, modulo_adder);
    }

    for (unsigned long i = 0; i < n; ++i) {
        unsigned long aux;
        std::cin >> aux;

        vi.push_back(aux);
    }

    for (unsigned long x = 0; x < n; ++x) {
        for (long long j = (long) k - 1; j >= 0; j--) {
            if (j == 0) {
                trees[j].update_leaf(vi[x] - 1, 1, without_leaf_update);
                continue;
            }

            unsigned long adder = 0;
            try {
                adder = trees[j - 1].iterative_query(vi[x], n - 1);
            } catch (...) {}

            trees[j].update_leaf(vi[x] - 1, adder, without_leaf_update);
        }
    }

    std::cout << (trees[k - 1].iterative_query(0, n - 1) % MOD) << std::endl;
}

int main() {
    k_inversions();
}
