#include "../interval_tree.h"

int main() {
    tree::IntervalTree<int> tree{4, 0, [](const int& left, const int& right) { return left + right; }};
    tree.leaf_update(0, 10);
    tree.leaf_update(1, 2);
    tree.leaf_update(2, 5);
    tree.leaf_update(0, 2);
    tree.print_level_by_level();
    std:: cout << tree.iterative_query(0, 1, [](const int& left, const int& right) { return left + right; });
}