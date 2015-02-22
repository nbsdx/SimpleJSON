#!/bin/sh

mkdir -p examples/bin
clang++ -std=c++11 -I. ./examples/json_example.cpp -o ./examples/bin/json_example
clang++ -std=c++11 -I. ./examples/array_example.cpp -o ./examples/bin/array_example
clang++ -std=c++11 -I. ./examples/prim_example.cpp -o ./examples/bin/prim_example
clang++ -std=c++11 -I. ./examples/init_example.cpp -o ./examples/bin/init_example
clang++ -std=c++11 -I. ./examples/load_example.cpp -o ./examples/bin/load_example

echo "Done. See './examples' for examples, and './examples/bin' for the executables."
