#ifndef SRC_FIND_UNION_H
#define SRC_FIND_UNION_H

#include <unordered_map>
#include <vector>

namespace algo_lib {
    namespace disjoint_sets {

        /**
         * In this particular chunk of code of fu structure I have decided to
         * implement union operation based on a tree's rank and path reducing
         * algorithm for find operation. Consequently achieving O(const) for every operation
         * (technically speaking it is not a lie, but to be honest it is O(alpha(n), whose values
         * are usually less than 5 ~ 6 for the given data)
          */
        template<typename key>
        class find_union {
        public:
            /**
             * @brief Creates an %find_union structure with no elements.
             */
            find_union() {
                _s = 0;
            }

            /**
             * @brief Makes an a set (kinda like singletons) from every key encountered in keys.
             * @param keys Any container that supports ranges iteration.
             */
            template<typename IterativeContainer>
            find_union(const IterativeContainer& keys)
                    : _s(keys.size()) {
                for (const auto& k: keys) {
                    parent.insert({k, k});
                    rank.insert({k, 0ULL});
                }
            }

            /**
             * @brief Builds an %find_union structure from a range.
             * @param b An input iterator (indicating the beginning)
             * @param e An input iterator (indicating the end)
             */
            template<typename InputIterator>
            find_union(InputIterator b, InputIterator e) {
                size_t s = 0;

                for (InputIterator it = b; it != e; ++it) {
                    parent.insert({*it, *it});
                    rank.insert({*it, 0ULL});
                    ++s;
                }

                _s = s;
            }

            /**
             * @brief Adds every key in keys to the structure. Every key becomes a separate set.
             * @param keys Any container that supports ranges iteration.
             * @return True if operation was successful, false otherwise, f.e. when any
             * of the keys already existed in a structure.
             */
            template<typename IterativeContainer>
            bool add_group(const IterativeContainer& keys) {
                if (exists_any(keys)) {
                    return false;
                }

                for (const auto& k: keys) {
                    parent.insert({k, k});
                    rank.insert({k, 0ULL});
                    _s++;
                }

                return true;
            }

            /**
             * @brief Adds every key in keys to the structure. Connects these keys with a @parent set.
             * @param keys Any container that supports ranges iteration.
             * @param parent A set element to determine the connection.
             * @return True if operation was successful, false otherwise, f.e. when any
             * of the keys already existed in a structure.
             */
            template<typename IterativeContainer>
            bool add_group_connected(const IterativeContainer& keys, const key parent) {
                if (!add_group(keys)) {
                    return false;
                }

                for (const auto& k: keys) {
                    union_sets(keys, parent);
                }

                return true;
            }

            /**
             * @brief Attempts to add a new element (as a singleton set).
             * @param temp An element.
             * @return True if operation was successful, false otherwise,
             * f.e. when the key already existed in a structure.
             */
            bool add(const key temp) {
                return add_group(std::vector{temp});
            }

            /**
             * @brief Adds a new set. Connects that set to a parent set.
             * @param temp An element to construct a new set from.
             * @param parent A set to connect a new element to.
             * @return True if operation was successful, false otherwise,
             * f.e. when the key already existed in a structure.
             */
            bool add_and_connect(const key temp, const key parent) {
                return add_group_connected(std::vector{temp}, parent);
            }

            /**
             * @brief Finds an id of a set @current element is in.
             * @param current An id of an element to find.
             * @return An id of the set @current element is in.
             */
            key find(const key current) {
                if (current != parent.at(current)) {
                    parent[current] = find(parent[current]);
                }

                return parent[current];
            }

            /**
             * Merges two sets into one.
             * @param first First set to merge.
             * @param second Second set to merge.
             */
            void union_sets(const key first, const key second) {
                _union_sets(first,
                            second,
                            std::bind(&find_union<key>::find, this, std::placeholders::_1));
            }

            /**
             * @brief Finds an id of a set @current element is in.
             * @param current An id of an element to find.
             * @return An id of the set @current element is in.
             *
             * There is a subtle difference in this function,
             * instead of using recursion to find a proper root element
             * it uses the iterative approach.
             *
             * In the future they both will support sets' labeling.
             */
            key iterative_find(key current) {
                key dup = current;

                // traverses the path to find a root
                while (dup != parent.at(dup)) {
                    dup = parent.at(dup);
                }

                // every parent on the node on that path becomes the root
                while (current != dup) {
                    current = parent[current];
                    parent[current] = dup;
                }

                return dup;
            }

            /**
             * Merges two sets into one.
             * @param first First set to merge.
             * @param second Second set to merge.
             *
             * It utilizes the iterative find
             * inside instead of the recursive one.
             */
            void iterative_union(const key first, const key second) {
                _union_sets(first,
                            second,
                            std::bind(&find_union<key>::iterative_find, this, std::placeholders::_1));
            }

            /**
             * @brief Checks whether the given elements are inside the same set.
             * @param first First element to find a set of.
             * @param second Second element to find a set of.
             * @return True if the root of both is the same, false otherwise.
             */
            bool equal(const key first, const key second) {
                return find(first) == find(second);
            }

            /**
             * @return The number of sets inside the structure.
             */
            size_t number_of_sets() const {
                return _s;
            }

        private:
            /**
             * @brief Checks whether any of the keys exists in a structure.
             * @param keys Any container that supports ranges iteration.
             * @return True if any of the keys exists in a structure, false otherwise.
             */
            template<typename IterativeContainer>
            bool exists_any(const IterativeContainer& keys) {
                for (const auto k: keys) {
                    if (parent.count(k) > 0) {
                        return true;
                    }
                }

                return false;
            }

            /**
             * Merges two sets into one.
             * @param first First set to merge.
             * @param second Second set to merge.
             * @param find A functor to find a set of a fiven element.
             */
            void _union_sets(const key first,
                            const key second,
                            std::function<key (const key current)> find) {
                key first_root = find(first);
                key second_root = find(second);

                if (first_root == second_root) {
                    return;
                }

                if (rank.at(first_root) < rank.at(second_root)) {
                    std::swap(first_root, second_root);
                }

                parent[second_root] = first_root;
                if (rank.at(first_root) == rank.at(second_root)) {
                    ++rank[first_root];
                }

                _s--;
            }

            std::unordered_map<key, key> parent;
            std::unordered_map<key, unsigned long long> rank;
            size_t _s; // a number of sets in a structure
        };
    }
}

#endif //SRC_FIND_UNION_H
