#ifndef SRC_SEGMENT_TREE_H
#define SRC_SEGMENT_TREE_H

#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

namespace tree {
    using ul = unsigned long; // tree's indexes are represented as an unsigned long value for simplicity

    /**
     * A segment tree class.
     * @tparam value a type of elements stored in a specific tree,
     * his class expects that type to be copy assignable and copy contructible.
     *
     * It should be expressed explicitly by concepts when they come in C++20.
     */
    template<typename value>
    class segment_tree {
        using tree_builder_fn = std::function<value (const value& left, const value& right)>;
        using query_fn = tree_builder_fn;

    public:
        segment_tree(ul desirable_size, value default_value, tree_builder_fn default_function)
                : dflt(default_value), fn(default_function) {
            if (desirable_size < 1) {
                throw std::out_of_range("Size of a tree should exceed 1.");
            }

            while (size < desirable_size) {
                size <<= 1;
            }

            tree.resize(2 * size, default_value);
        }

        /**
         * Updates the leaf, and traverses the tree to the top.
         * Consequently updating all the values on its path.
         * @param leaf_index an index of a node to be updated
         * @param updater a new value to combine with the previous one
         * @param result_function tells the function how to combine the previous
         * value in a leaf with @updater
         */
        virtual void leaf_update(ul leaf_index, value updater, query_fn result_function) {
            is_in_leaf_bounds(leaf_index);

            leaf_index += size; // leafs in a tree are stored at (size, ... , 2 * size - 1) positions
            tree[leaf_index] = result_function(tree[leaf_index], updater);
            leaf_index >>= 1;

            while (leaf_index) {
                tree[leaf_index] = fn(tree[left_child(leaf_index)], tree[right_child(leaf_index)]);
                leaf_index >>= 1;
            }
        }

        /**
         * Provides a default function to update a leaf that uses @fn
         */
        virtual void leaf_update(ul leaf_index, value updater) {
            leaf_update(leaf_index, updater, fn);
        }

        virtual value iterative_query(ul s_index, ul e_index, query_fn result_function) {
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

        virtual value iterative_query(ul s_index, ul e_index) {
            return iterative_query(s_index, e_index, fn);
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

        value get_leaf_value(ul index) {
            is_in_leaf_bounds(index);

            return get_node_value(index);
        }

        value get_root_value() {
            return get_node_value(1);
        }

    protected:
        void print_range(ul start, ul end) {
            for (ul index = start; index < end; ++index) {
                std::cout << tree[index + end] << " ";
            }

            std::cout << std::endl;
        }

        ul left_child(ul node) noexcept {
            return 2 * node;
        }

        ul right_child(ul node) noexcept {
            return 2 * node + 1;
        }

        value get_node_value(ul index) noexcept {
            return tree[index];
        }

        void is_in_leaf_bounds(ul leaf_index) {
            if (!(0 <= leaf_index && leaf_index < size)) {
                throw std::out_of_range("Provided index is out of range.");
            }
        }

        value dflt;
        ul size = 1;
        std::vector<value> tree; // for convenience purposes a root is stores at index 1
        tree_builder_fn fn;
    };
}

#endif //SRC_SEGMENT_TREE_H
