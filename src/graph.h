#ifndef SRC_GRAPH_H
#define SRC_GRAPH_H

#include <unordered_map>
#include <queue>
#include <unordered_set>

namespace algo_lib {
    namespace graph {
        template <typename T>
        class Graph {
        public:
            virtual std::unordered_set<T> get_children(T node_id) const = 0;

            virtual void connect(T node_from, T node_to) = 0;

            virtual void biconnect(T node_from, T node_to) = 0;

            virtual bool is_connected(T node_from, T node_to) = 0;

            virtual void disconnect(T node_from, T node_to) = 0;

            virtual bool exists(T node) const = 0;

            virtual void remove(T node) = 0;
        };

        template <typename T>
        class ListGraph
                : Graph<T> {
        public:
            std::unordered_set<T> get_children(const T node_id) const override {
                return G[node_id];
            }

            bool exists(T node) const override {
                return G.count(node) > 0;
            }

            bool is_connected(T node_from, T node_to) {
                if (!exists(node_from)) {
                    return false;
                }

                return G.at(node_from).count(node_to) > 0;
            }

            void connect(const T node_from, const T node_to) override {
                if (exists(node_from) || is_connected(node_from, node_to)) {
                    return;
                }

                G.at(node_from).emplace(node_to);
            }

            void biconnect(T node_from, T node_to) override {
                connect(node_from, node_to);
                connect(node_to, node_from);
            }

            void disconnect(T node_from, T node_to) override {
                if (is_connected(node_from, node_to)) {
                    return;
                }

                G.at(node_from).erase(node_to);
            }

            void remove(T node) override {
                G.erase(node);
            }

        private:
            std::unordered_map<T, std::unordered_set<T>> G;
        };

        namespace algorithms {
            namespace Detail {
                template <typename T>
                struct Edge {
                    T node_to;
                    unsigned long long distance;

                    Edge(const T& node_to, unsigned long long distance)
                    : node_to(node_to), distance(distance) {}

                    bool operator<(const Edge& e) const {
                        return e.distance < distance;
                    }
                };
            }

            template <typename T, typename Functor>
            void bfs(const Graph<T> &graph1, const T start, Functor fun) {
                std::queue<T> q;
                std::unordered_set<T> visited;

                q.push(start);
                visited.insert(start);

                while (!q.empty()) {
                    auto top = q.front();
                    fun(top);
                    q.pop();

                    const auto children = graph1.get_children(top);
                    for (const auto child: children) {
                        if (visited.count(child) == 0) {
                            visited.insert(child);
                            q.push(child);
                        }
                    }
                }
            }
        }
    }
}

#endif //SRC_GRAPH_H
