# Algorithms-Lib #

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5d90d7cf3e1b4b5f9b55613dc29b7a4e)](https://app.codacy.com/app/vanbinhstudios/Algorithms-Lib?utm_source=github.com&utm_medium=referral&utm_content=fiffeek/Algorithms-Lib&utm_campaign=Badge_Grade_Dashboard)
[![Build Status](https://travis-ci.com/fiffeek/Algorithms-Lib.svg?branch=master)](https://travis-ci.com/fiffeek/Algorithms-Lib)

## A library for algorithms showcase and usage presentation ##

Currently in development, feel free to suggest data structures
that should be featured here.

<br>

### Currently available ###
- <a href="https://en.wikipedia.org/wiki/Segment_tree">segment trees</a>
- <a href="https://en.wikipedia.org/wiki/Disjoint-set_data_structure">find and union</a>
- <a href="https://en.wikipedia.org/wiki/Ternary_search_tree">ternary search trees</a> implemented as <a href="https://en.wikipedia.org/wiki/Persistent_data_structure">a persistent tree</a>
- <a href="https://en.wikipedia.org/wiki/Fenwick_tree">Fenwick tree</a>

### Technology used ###
- boost test library
- doxygen
- cmake
- travis ci
- codacy

<br>

## Changelog ##

## [0.0.2] - 2019-03-13
### Added
- fenwick tree implementation with sum operation on a given index and a query which returns the sum of a given interval
- fenwick tests and usage example 

## [0.0.2] - 2019-03-12
### Changed
- reorganized the project structure, add self-explanatory changelog format

## [0.0.1] - 2019-01-26
### Added
- algo_lib::tree::TST, with documentations, without tests for the time being

## [0.0.1] - 2019-01-19
### Updated
- find and union's documentation and comments, add iterative functions as a replacement for recursive ones (in algo_lib::disjoint_sets::find_union)

## [0.0.1] - 2019-01-18
### Added
- algo_lib::disjoint_sets::find_union data stracture (base wireframe, not commented, working, tho) and some tests

## [0.0.1] - 2019-01-17
### Added
- full documentation for segment_tree, change namespaces structure (algo_lib:: prefix)

## [0.0.1] - 2019-01-16
### Added
- tree::segment_tree structure with tests and usage (@todo add recursive queries)
