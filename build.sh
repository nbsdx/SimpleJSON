#!/bin/sh

mkdir -p examples/bin
mkdir -p test/bin

# Build Examples.
clang++ -std=c++11 -I. ./examples/json_example.cpp -o ./examples/bin/json_example
clang++ -std=c++11 -I. ./examples/array_example.cpp -o ./examples/bin/array_example
clang++ -std=c++11 -I. ./examples/prim_example.cpp -o ./examples/bin/prim_example
clang++ -std=c++11 -I. ./examples/init_example.cpp -o ./examples/bin/init_example
clang++ -std=c++11 -I. ./examples/load_example.cpp -o ./examples/bin/load_example
clang++ -std=c++11 -I. ./examples/iter_example.cpp -o ./examples/bin/iter_example

# Build Test Tool
clang++ -std=c++11 -I. ./test/tester.cpp -o ./test/bin/tester

echo "Done. See './examples' for examples, and './examples/bin' for the executables."
echo "To run tests, run cd test; python ./run.py"
