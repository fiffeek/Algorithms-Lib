# Algorithms-Lib #
<br>

## A library for algorithms showcase and usage presentation ##

Currently in development, feel free to suggest data structures
that should be featured here.

<br>

### Currently available ###
- <a href="https://en.wikipedia.org/wiki/Segment_tree">segment trees</a>
- <a href="https://en.wikipedia.org/wiki/Disjoint-set_data_structure">find and union</a>
- <a href="https://en.wikipedia.org/wiki/Ternary_search_tree">ternary search trees</a> implemented as <a href="https://en.wikipedia.org/wiki/Persistent_data_structure">a persistent tree</a>

### Technology used ###
- boost test library
- doxygen
- cmake

### ChangeLog ###
- 16.01 - add tree::segment_tree structure with tests and usage (@todo add recursive queries)
- 17.01 - add (almost) full documentation for segment_tree, change namespaces structure (algo_lib:: prefix)
- 18.01 - add algo_lib::disjoint_sets::find_union data stracture (base wireframe, not commented, working, tho) and some tests
- 19.01 - update find and union's documentation and comments, add iterative functions as a replacement for recursive ones (in algo_lib::disjoint_sets::find_union)
- 26.01 - add algo_lib::tree::TST, with documentations, without tests for the time being
