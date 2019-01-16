#include "../segment_tree.h"

/*
 * Represents a typical segment tree usage.
 * On several examples with some testing.
 */

using std_segment = tree::segment_tree<int>;
constexpr int SUM_TREE_SIZE = 10;
constexpr int SUM_TREE_DFLT = 0;

void abstract_sum_tree_test() {
    std_segment sum_tree(SUM_TREE_SIZE,
                         SUM_TREE_DFLT,
                         [](const int& lhs, const int& rhs) { return lhs + rhs; });

    for (int i = 0; i < SUM_TREE_SIZE; ++i) {
        sum_tree.leaf_update((tree::ul) i, i * i + 2 * i - 5);
    }

    assert(sum_tree.get_root_value() == 325);
}

int main() {
    abstract_sum_tree_test();
}