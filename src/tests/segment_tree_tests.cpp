#define BOOST_TEST_MODULE segment_tree
#include <boost/test/included/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "../segment_tree.h"

BOOST_AUTO_TEST_SUITE(simple);

    BOOST_AUTO_TEST_CASE(simple_test_case) {
        using std_segment = tree::segment_tree<int>;
        constexpr int SUM_TREE_SIZE = 10;
        constexpr int SUM_TREE_DFLT = 0;

        std_segment sum_tree(SUM_TREE_SIZE,
                             SUM_TREE_DFLT,
                             [](const int& lhs, const int& rhs) { return lhs + rhs; });

        for (int i = 0; i < SUM_TREE_SIZE; ++i) {
            sum_tree.leaf_update((tree::ul) i, i * i + 2 * i - 5);
        }

        BOOST_CHECK_EQUAL(sum_tree.get_root_value(), 325);
    }

BOOST_AUTO_TEST_SUITE_END();