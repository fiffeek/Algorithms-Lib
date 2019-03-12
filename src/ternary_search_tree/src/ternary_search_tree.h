#ifndef SRC_TST_H
#define SRC_TST_H

#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

namespace algo_lib {
    namespace tree {
        /**
         * Detail namespace for some helper functions.
         * In this case it provides fold template for given Iterators.
         *
         * (stolen from Boost lib)
         */
        namespace detail {
            /**
             * @brief Folds over a contiguous sequence of elements and returns the accumulated value.
             * @tparam Iter Iterator type
             * @tparam Acc Accumulator type
             * @tparam Functor Function type to fold over
             * @param first Starting element in a sequence
             * @param last Last element in a sequence
             * @param acc Accumulator
             * @param functor Function to fold over
             * @return functor(...functor(functor(acc, *first), *(first + 1))..., *(last - 1))
             *
             * Note that this  + 1 or - 1 is just to show the use cases,
             * in reality it is implemented by using std::next function
             */
            template<typename Iter, typename Acc, typename Functor>
            Acc fold(Iter first, Iter last, Acc acc, Functor functor) {
                return first != last ? fold(std::next(first, 1), last, functor(acc, *first), functor) : acc;
            }
        }

        /**
         * @brief Ternary Search Tree (in short TST) implemented as a persistent data structure.
         * @tparam C Type of elements stored in a tree. It is essential for that type to have comparators implemented.
         */
        template<typename C = char>
        class TST {
        private:
            /**
             * Private Node structure that is not visible outside.
             * Basically, even though it could be omitted it is easier to
             * check some of the boundaries like 'empty tree' with it.
             */
            struct Node {
                const std::shared_ptr<const Node> left_tst;
                const std::shared_ptr<const Node> middle_tst;
                const std::shared_ptr<const Node> right_tst;
                const C data;
                const bool is_end; ///< indicated the end of a sequence

                Node(const std::shared_ptr<const Node> &l,
                     const std::shared_ptr<const Node> &m,
                     const std::shared_ptr<const Node> &r,
                     const C &v,
                     const bool is_word)
                        : left_tst(l), middle_tst(m), right_tst(r), data(v), is_end(is_word) {}
            };

            const std::shared_ptr<const Node> tst_data;

            /**
             * Raises an error when something is trying to access an empty tree.
             */
            void throw_logic_error_if_violated() const {
                if (empty()) {
                    throw std::logic_error("Empty tree violation.");
                }
            }

            TST(const std::shared_ptr<const Node> tst_data)
                    : tst_data(tst_data) {}

            TST(const TST &left, const TST &middle, const TST &right, const C &value, const bool end_of_word)
                    : tst_data(std::make_shared<const Node>(
                    left.tst_data,
                    middle.tst_data,
                    right.tst_data,
                    value,
                    end_of_word)
            ) {}

            /**
             * @brief For each sequence t in TST calculates the number of common elements in a given sequence and t.
             * @param str Sequence of elements, must end with '\0'
             * @return Number of common elements indicated by brief.
             */
            size_t prefix_calculator(const C *str) const {
                return (str[0] && empty()) || (str[0] == '\0' && !empty()) ? 0 :
                       str[0] == '\0' ? 0
                                      : str[0] == value() ? 1 + center().prefix_calculator(str + 1)
                                                          : str[0] > value() ? right().prefix_calculator(str)
                                                                             : left().prefix_calculator(str);
            }

        public:
            TST() = default;

            TST(const std::basic_string<C> &str)
                    : TST(str.c_str()) {}

            TST(const C *str)
                    : TST(str[0] ? TST(TST(), TST(str + 1), TST(), str[0], str[1] == '\0')
                                 : TST()) {}

            TST operator+(const std::basic_string<C> &str) const {
                return this->operator+(str.c_str());
            }

            /**
             * @brief Adds a sequence of elements to a given TST and
             * return a new one with elements from the previous and a new sequence.
             * @param str Sequence of elements to add, must end with '\0'
             * @return New TST
             */
            TST operator+(const C *str) const {
                return empty() ? TST(str)
                               : str[0] ? str[0] > value() ? TST(left(), center(), right() + (str), value(), word())
                                                           : str[0] < value() ? TST(left() + (str), center(), right(),
                                                                                    value(), word())
                                                                              : TST(left(), center() + (str + 1),
                                                                                    right(), value(),
                                                                                    word() || str[1] == '\0')
                                        : TST(left(), center(), right(), value(), word());
            }

            C value() const {
                throw_logic_error_if_violated();
                return tst_data->data;
            }

            /**
             * @brief Returns whether any sequence previously added to a TST ends in the given node.
             */
            bool word() const {
                throw_logic_error_if_violated();
                return tst_data->is_end;
            }

            TST left() const {
                throw_logic_error_if_violated();
                return TST(tst_data->left_tst);
            }

            TST center() const {
                throw_logic_error_if_violated();
                return TST(tst_data->middle_tst);
            }

            TST right() const {
                throw_logic_error_if_violated();
                return TST(tst_data->right_tst);
            }

            bool empty() const {
                return tst_data == nullptr;
            }

            /**
             * @brief Checks whether a given sequence is present in a tree.
             * @param str Sequence of elements, must end with '\0'
             * @return One boolean, true if the given sequence is present, false otherwise.
             */
            bool exist(const std::basic_string<C> &str) const {
                return exist(str.c_str());
            }

            /**
             * @brief Searches for the longest common prefix of a given sequence and every sequence in TST.
             * @param str Sequence of elements
             * @return The longest common prefix of a given sequence and every sequence in TST
             *
             * f.e.
             * When the TST consists of (and is build with string literals):
             * "category", "functor", "theory"
             *
             * TST.prefix("catamorphism") == "cat"
             */
            std::basic_string<C> prefix(const std::basic_string<C> &str) const {
                return str.substr(0, prefix_calculator(str.c_str()));
            }

            /**
             * @brief Folds a tree over a given function returning the accumulated value.
             * @tparam Acc Accumulator object type
             * @tparam Functor Function object type
             * @param acc Accumulated value
             * @param functor Function to fold over (takes over 2 arguments: (acc, C) -> acc)
             * @return Accumulated value over all nodes in a tree.
             *
             * NOTE:
             * It traverses the tree reverse in-order traversal. That is, first right ten middle then left.
             */
            template<typename Acc, typename Functor>
            Acc fold(Acc acc, Functor functor) const {
                return empty() ? acc
                               : functor(left().fold(center().fold(right().fold(
                                acc,
                                functor), functor), functor), value());
            }

            size_t size() const {
                return this->fold((size_t) 0,
                                  [](const size_t acc, C) { return acc + 1; });
            }

            bool exist(const C *str) const {
                return str[0] == '\0' || empty() ? false
                                                 : str[1] == '\0' ? str[0] == value() ? word()
                                                                                      : str[0] > value()
                                                                                        ? right().exist(str)
                                                                                        : left().exist(str)
                                                                  : str[0] == value() ? center().exist(str + 1)
                                                                                      : str[0] > value()
                                                                                        ? right().exist(str)
                                                                                        : left().exist(str);
            }
        };
    }
}

#endif //SRC_TST_H
