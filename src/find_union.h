#ifndef SRC_FIND_UNION_H
#define SRC_FIND_UNION_H

#include <unordered_map>
#include <vector>

namespace algo_lib {
    namespace disjoint_sets {
        template<typename key>
        class find_union {
        public:
            find_union(const std::vector<key>& keys) {
                for (const auto& k: keys) {
                    parent.insert({k, k});
                    rank.insert({k, 0ULL});
                }
            }

            key find(const key current) {
                if (current != parent.at(current)) {
                    parent[current] = find(parent[current]);
                }

                return parent[current];
            }

            void union_sets(const key first, const key second) {
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
            }

            bool in_the_same_set(const key first, const key second) {
                return find(first) == find(second);
            }

            size_t number_of_sets() const {
              //
            }

        private:
            std::unordered_map<key, key> parent;
            std::unordered_map<key, unsigned long long> rank;
        };
    }
}

#endif //SRC_FIND_UNION_H
