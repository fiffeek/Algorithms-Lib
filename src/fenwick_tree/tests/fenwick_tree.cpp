#define BOOST_TEST_MODULE fenwick_tree_tests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "../src/fenwick_tree.h"

BOOST_AUTO_TEST_SUITE(simple);

    BOOST_AUTO_TEST_CASE(simple_test_case) {
        using fenwick = algo_lib::tree::fenwick_tree;
        constexpr int SIZE = 10;
        fenwick tree(SIZE);

        for (int i = 0; i < SIZE; ++i) {
            BOOST_CHECK_EQUAL(tree.sum(i), 0);
        }

        tree.add(0, 10);
        tree.add(1, 1);
        tree.add(3, 30);

        BOOST_CHECK_EQUAL(tree.sum(0), 10);
        BOOST_CHECK_EQUAL(tree.sum(1), 11);
        BOOST_CHECK_EQUAL(tree.sum(2), 11);
        BOOST_CHECK_EQUAL(tree.sum(3), 41);
        BOOST_CHECK_EQUAL(tree.interval_sum(1, 3), 31);
        BOOST_CHECK_EQUAL(tree.interval_sum(0, 2), 11);
    }

BOOST_AUTO_TEST_SUITE_END();