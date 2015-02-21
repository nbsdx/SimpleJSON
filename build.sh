#!/bin/sh

mkdir -p examples/bin
g++ -std=c++11 -I. ./examples/json_example.cpp -o ./examples/bin/json_example
g++ -std=c++11 -I. ./examples/array_example.cpp -o ./examples/bin/array_example

echo "Done. See './examples' for examples, and './examples/bin' for the executables."
