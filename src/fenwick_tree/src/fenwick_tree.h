#ifndef SRC_FENWICK_TREE_H
#define SRC_FENWICK_TREE_H

#include <vector>

namespace algo_lib {
    namespace tree {
        class fenwick_tree {
        public:

            /**
             * Fenwick tree implementation for interval sums.
             * @param n number of elements stored in a tree
             */
            fenwick_tree(unsigned long n)
                    : size((int) n) {
                arr.resize(n + 1, 0);
            }

            /**
             * @param i index of an array, greater than 0 and less than n + 1
             * @return sum of arr[0, i] (inclusive)
             */
            int sum(int i) {
                int sum = 0;
                ++i;

                while (i > 0) {
                    sum += arr[i];
                    i -= leastSignificantBit(i);
                }

                return sum;
            }

            /**
             * Adds delta to arr[i]
             * @param i index to add the delta value to
             * @param delta value which should be added to arr[i]
             */
            void add(int i, int delta) {
                ++i;

                while (i < size) {
                    arr[i] += delta;
                    i += leastSignificantBit(i);
                }
            }

            /**
             * @param i left bound
             * @param j right bound
             * @return sum of an arr[i, j] (inclusive)
             */
            int interval_sum(int i, int j) {
                return sum(j) - sum(i - 1);
            }

        private:

            /**
             * @return int (in decimal) - the least significant bit of a given number
             */
            int leastSignificantBit(int i) {
                return i & -i;
            }

            int size;
            std::vector<int> arr;
        };
    }
}


#endif //SRC_FENWICK_TREE_H
