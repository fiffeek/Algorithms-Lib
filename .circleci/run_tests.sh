#!/bin/bash

parentdir="$(dirname "$PWD")"
files=`find $parentdir/src/*/tests/*.cpp`
for file in $(echo $files | tr "\n" "\n")
do
  echo "$file"
  g++ -std=c++17 $file -o ./tmp
  ./tmp
done

rm tmp
