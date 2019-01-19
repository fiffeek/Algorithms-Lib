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
            find_union(const std::vector<key>& keys)
                    : _s(keys.size()) {
                for (const auto& k: keys) {
                    parent.insert({k, k});
                    rank.insert({k, 0ULL});
                }
            }

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

            key find(const key current) {
                if (current != parent.at(current)) {
                    parent[current] = find(parent[current]);
                }

                return parent[current];
            }

            void union_sets(const key first, const key second) {
                _union_sets(first,
                            second,
                            std::bind(&find_union<key>::find, this, std::placeholders::_1));
            }

            key iterative_find(key current) {
                key dup = current;

                while (dup != parent.at(dup)) {
                    dup = parent.at(dup);
                }

                while (current != dup) {
                    current = parent[current];
                    parent[current] = dup;
                }

                return dup;
            }

            void iterative_union(const key first, const key second) {
                _union_sets(first,
                            second,
                            std::bind(&find_union<key>::iterative_find, this, std::placeholders::_1));
            }

            bool equal(const key first, const key second) {
                return find(first) == find(second);
            }

            size_t number_of_sets() const {
                return _s;
            }

        private:
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
            size_t _s;
        };
    }
}

#endif //SRC_FIND_UNION_H
