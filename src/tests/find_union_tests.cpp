#define BOOST_TEST_MODULE find_union_tests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "../find_union.h"

BOOST_AUTO_TEST_SUITE(simple);

    BOOST_AUTO_TEST_CASE(simple_test_case) {
        using union_set_type = algo_lib::disjoint_sets::find_union<int>;
        constexpr int SIZE = 100;

        std::vector<int> keys(SIZE);
        std::iota(keys.begin(), keys.end(), 0);
        union_set_type sets(keys);

        for (int i = 0; i < SIZE - 1; ++i) {
            sets.union_sets(i, i + 1);
        }

        auto compare = sets.find(0);
        for (int i = 0; i < SIZE; ++i) {
            BOOST_CHECK_EQUAL(sets.find(i), compare);
        }
    }

BOOST_AUTO_TEST_SUITE_END();