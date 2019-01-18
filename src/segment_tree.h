#ifndef SRC_SEGMENT_TREE_H
#define SRC_SEGMENT_TREE_H

#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

namespace algo_lib {
    namespace tree {
        using ul = unsigned long; // tree's indexes are represented as an unsigned long data_type for simplicity

        /**
         * ABC for all segment trees to implements.
         * Provides the query function template for derived classes.
         *
         * I decided that the values should be passed by value to every function.
         * Although, technically it is not the most efficient solution (due to the number of copies),
         * most of the use cases I have ever encountered used one to two ints as the @data_type,
         * so passing smaller structures like these (any which has <= 8 bytes) should be faster.
         */
        template<typename data_type>
        class segment_tree_interface {
        public:
            using update_fn = std::function<data_type(const data_type &left, const data_type &right)>;

            virtual void update_leaf(ul leaf_index, data_type updater, update_fn result_function) = 0;

            virtual data_type iterative_query(ul s_index, ul e_index, update_fn result_function) = 0;

            virtual data_type get_leaf_value(ul index) const = 0;

            virtual data_type get_root_value() const = 0;

            virtual void print_level_by_level() const = 0;

            virtual ~segment_tree_interface() = default;
        };

        /**
         * A segment tree class.
         * @tparam data_type a type of elements stored in a specific tree,
         * this class expects that type to be copy assignable and copy constructible.
         *
         * It should be expressed explicitly by concepts when they come in C++20.
         */
        template<typename data_type>
        class segment_tree
                : segment_tree_interface<data_type> {
            static_assert(std::is_copy_assignable<data_type>::value && std::is_copy_constructible<data_type>::value);
            using tree_builder_fn = typename segment_tree_interface<data_type>::update_fn;
            using query_fn = tree_builder_fn; // just an alias for the query, cause they are 'different' in meaning

        public:
            /**
             * This ctor takes up approximately O(log(@desirable_size))
             * time to construct the object.
             */
            segment_tree(const ul desirable_size,
                         data_type default_value,
                         const tree_builder_fn default_function)
                    : default_argument(default_value), default_function(default_function) {
                if (desirable_size < 1) {
                    throw std::out_of_range("Size of a tree should exceed 1.");
                }

                while (size < desirable_size) {
                    size <<= 1;
                }

                tree.resize(size << (ul) 1, default_value);
            }

            /**
             * Updates the leaf, and traverses the tree to the top.
             * Consequently updating all the values on its path.
             * @param leaf_index an index of a node to be updated
             * @param updater a new value to combine with the previous one
             * @param result_function tells the function how to combine the previous
             * value in a leaf with @updater
             */
            void update_leaf(ul leaf_index,
                             data_type updater,
                             const query_fn result_function) override {
                is_in_leaf_bounds(leaf_index);

                leaf_index += size; // leafs in a tree are stored at (size, ... , 2 * size - 1) positions
                tree[leaf_index] = result_function(tree[leaf_index], updater);
                leaf_index >>= 1;

                while (leaf_index) {
                    tree[leaf_index] = default_function(tree[left_child(leaf_index)], tree[right_child(leaf_index)]);
                    leaf_index >>= 1;
                }
            }

            /**
             * Provides a default function to update a leaf that uses @fn
             */
            virtual void leaf_update(ul leaf_index, const data_type &updater) {
                update_leaf(leaf_index, updater, default_function);
            }

            /**
             * Iterates over a tree in a non-recursive manner, calculating @result_function
             * over all of the O(log(n)) intervals that [s_index, e_index] is being divided into.
             * @param s_index a start index for the resulting interval
             * @param e_index an end index for the resulting interval
             * @param result_function function to accumulate the result into, the left argument is an accumulator
             */
            data_type iterative_query(ul s_index, ul e_index, const query_fn result_function) override {
                if (s_index > e_index) {
                    throw std::out_of_range("Indexes are overlapping, a start is greater than the end.");
                }

                s_index += size;
                e_index += size;
                data_type result = default_argument;

                result = result_function(result, tree[s_index]);
                // if start index is the same as end index there is no need to do anything with the result
                if (s_index != e_index) {
                    result = result_function(result, tree[e_index]);
                }

                // traverses the tree from both nodes until they meet
                while (s_index / 2 < e_index / 2) {
                    /* if start index is a left child (so start index is not an odd number) there is a segment match
                     * so answer is changing because a sibling (the right child of this node's parent) is inside the
                     * interval */
                    if (!(s_index & (ul) 1)) { // one would be treated as signed integer so there is a need to cast
                        result = result_function(result, tree[s_index + 1]);
                    }

                    /* the same idea as above is written down here, but in this case there is a match when
                     * the node indicated by the end index is on the even position */
                    if (e_index & (ul) 1) {
                        result = result_function(result, tree[e_index - 1]);
                    }

                    s_index >>= 1;
                    e_index >>= 1;
                }

                return result;
            }

            /* Query with a result_function set to the default one which is defined in a class. */
            virtual data_type iterative_query(ul start_index, ul end_index) {
                return iterative_query(start_index, end_index, default_function);
            }

            void print_leafs() const {
                print_range(0, size);
            }

            /**
             * Prints the tree structure level by level,
             * starting from the root as the first line on the
             * standard output stream.
             */
            void print_level_by_level() const override {
                ul level_size = 1;

                while (level_size <= size) {
                    print_range(0, level_size);
                    level_size <<= 1;
                }
            }

            data_type get_leaf_value(const ul index) const override {
                is_in_leaf_bounds(index);

                return get_node_value(index);
            }

            /**
             * @return a value stored in a root of a tree, in this implementation
             * it is the data stored at the 1 index in a vector.
             */
            data_type get_root_value() const override {
                return get_node_value(1);
            }

            virtual data_type get_node_value(const ul index) const {
                return tree[index];
            }

            virtual ~segment_tree() = default;

        protected:
            data_type default_argument;
            ul size = 1;
            std::vector<data_type> tree; // for convenience purposes a root is stores at index 1
            tree_builder_fn default_function;

        private:
            ul left_child(const ul node) const noexcept {
                return 2 * node;
            }

            ul right_child(const ul node) const noexcept {
                return 2 * node + 1;
            }

            /**
             * Leafs are represented in bounds of (0, size - 1)
             * @param leaf_index the index value to be checked if it exceeds the bounds or not
             */
            void is_in_leaf_bounds(const ul leaf_index) const {
                if (!(0 <= leaf_index && leaf_index < size)) {
                    throw std::out_of_range("Provided index is out of range.");
                }
            }

            /**
             * Prints the contiguous chunk of a tree (not necessary all on the same level).
             */
            void print_range(const ul start, const ul end) const {
                for (ul index = start; index < end; ++index) {
                    std::cout << tree[index + end] << " ";
                }

                std::cout << std::endl;
            }
        };
    }
}

#endif //SRC_SEGMENT_TREE_H
