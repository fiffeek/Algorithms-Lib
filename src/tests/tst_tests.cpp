#define BOOST_TEST_MODULE tst_tests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include "../ternary_search_tree.h"

template <typename T>
using TST = algo_lib::tree::TST<T>;

BOOST_AUTO_TEST_SUITE(simple);

    BOOST_AUTO_TEST_CASE(simple_test_case) {
        TST<char> f{};

        TST<char> a{"test"};
        auto b = a + "dddd";
        auto c = b + "tedd";

        auto d = c.left().center() + "fft";

        BOOST_CHECK_EQUAL(a.prefix("d"), "");

        BOOST_CHECK_EQUAL(b.prefix("d"), "d");
        BOOST_CHECK_EQUAL(b.prefix("td"), "t");
        BOOST_CHECK_EQUAL(b.prefix("de"), "d");
        BOOST_CHECK_EQUAL(b.prefix("ted"), "te");

        BOOST_CHECK_EQUAL(c.prefix("test"), "test");
        BOOST_CHECK_EQUAL(c.prefix("ted"), "ted");
        BOOST_CHECK_EQUAL(c.prefix("tedd"), "tedd");
        BOOST_CHECK_EQUAL(c.prefix("ff"), "");
        BOOST_CHECK_EQUAL(c.prefix("dff"), "d");

        BOOST_CHECK_EQUAL(d.prefix("ddddd"), "ddd");
        BOOST_CHECK_EQUAL(d.prefix("fft"), "fft");



        BOOST_CHECK_EQUAL(a.size(), 4);
        BOOST_CHECK_EQUAL(b.size(), 8);
        BOOST_CHECK_EQUAL(c.size(), 10);
        BOOST_CHECK_EQUAL(d.size(), 6);
    }

    BOOST_AUTO_TEST_CASE(moving_from_node) {
        auto t = TST<char>{};

        auto t2 = t + "1";

        BOOST_CHECK_EQUAL(t2.empty(), false);
        BOOST_CHECK_EQUAL(t2.left().empty(), true);
        BOOST_CHECK_EQUAL(t2.right().empty(), true);
        BOOST_CHECK_EQUAL(t2.center().empty(), true);

        auto t3 = t2 + "0";
        auto t4 = t3 + "2";

        BOOST_CHECK_EQUAL(t4.empty(), false);
        BOOST_CHECK_EQUAL(t4.left().empty(), false);
        BOOST_CHECK_EQUAL(t4.right().empty(), false);
        BOOST_CHECK_EQUAL(t4.center().empty(), true);

        BOOST_CHECK_THROW(t4.center().center(), std::logic_error);
    }

BOOST_AUTO_TEST_SUITE_END();