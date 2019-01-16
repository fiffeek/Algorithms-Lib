#ifndef SRC_INTERVAL_TREE_H
#define SRC_INTERVAL_TREE_H

#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

namespace tree {
    using ul = unsigned long;

    template<typename value>
    class IntervalTree {
        using tree_builder_fn = std::function<value (const value& left, const value& right)>;
        using query_fn = tree_builder_fn;

    public:
        IntervalTree(ul n, value dflt, tree_builder_fn fn)
                : dflt(dflt), fn(fn) {
            while (size < n) {
                size <<= 1;
            }

            tree.resize(2 * size, dflt);
        }

        void leaf_update(ul leaf_index, value updater, query_fn result_function) {
            assert(0 <= leaf_index && leaf_index < size);

            leaf_index += size;
            tree[leaf_index] = result_function(tree[leaf_index], updater);
            leaf_index >>= 1;

            while (leaf_index) {
                tree[leaf_index] = fn(tree[left_child(leaf_index)], tree[right_child(leaf_index)]);
                leaf_index >>= 1;
            }
        }

        void leaf_update(ul leaf_index, value updater) {
            leaf_update(leaf_index, updater, fn);
        }

        value iterative_query(ul s_index, ul e_index, query_fn result_function) {
            if (s_index > e_index) {
                return dflt;
            }

            s_index += size; e_index += size;
            value result = dflt;

            result = result_function(result, tree[s_index]);
            if (s_index != e_index) {
                result = result_function(result, tree[e_index]);
            }

            while (s_index / 2 < e_index / 2) {
                if (!(s_index & 1)) {
                    result = result_function(result, tree[s_index + 1]);
                }

                if (e_index & 1) {
                    result = result_function(result, tree[e_index - 1]);
                }

                s_index >>= 1;
                e_index >>= 1;
            }

            return result;
        }

        void print_leafs() {
            print_range(0, size);
        }

        void print_level_by_level() {
            ul level_size = 1;

            while (level_size <= size) {
                print_range(0, level_size);
                level_size <<= 1;
            }
        }

    private:
        void print_range(ul start, ul end) {
            for (ul index = start; index < end; ++index) {
                std::cout << tree[index + end] << " ";
            }

            std::cout << std::endl;
        }

        ul left_child(ul node) {
            return 2 * node;
        }

        ul right_child(ul node) {
            return 2 * node + 1;
        }


        value dflt;
        ul size = 1;
        std::vector<value> tree;
        tree_builder_fn fn;
    };
}

#endif //SRC_INTERVAL_TREE_H
