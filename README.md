# Electical Circuits problem
My solution of electical circuits problem.

# How to clone?
```
git clone --recurse-submodules git@github.com:LegendaryHog/ElcCir.git
```

# How to build?
```
cmake -B build/ -DCMAKE_CXX_COMPILER=[your c++ compiler] -DCMAKE_BUILD_TYPE=Release

cmake --build build/ --target unit_tests    # building all unit tests
cmkae --build build/ --target currents      # building problem solution
cmake --build build/ --target whole_project # building the unit_tests and currents
```

# How to run google tests?
```
ctest --test-dir build/
```

# How to generate end to end tests?
In directory `end_to_end/`:
```
chmod "+x" test_gen.py
./test_gen.py number_of_nodes filename [--ans]
```
Script will generate test in `filename`. If you add flag `--ans` script will generate answer in `filename.ans`

# How to run end to end tests?
```
chmod "+x" test_cmp.py
./test_cmp.py build_dir test1 [test2, test3, ...]
```
Script compares output of program and answer. Scripit searches answet in `test{i}.ans`.

# Example of end to end testing from build to result.

If you want to use my scripts you need `numpy`, `math` and `termcolor` libraries, you can install these libraries by these commands:
```
pip install numpy
pip install math
pip install termcolor
```

```
cmake -B build/ -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release

cmake --build build/ --target currents

cd end_to_end/

for i in $(seq 1 1 20); ./test_gen.py 20 test${i}.dat --ans

./test_cmp.py ../build/ *.dat
```
Result:

![image](.readme/runnernormal.png)

If error occurred:

![image](.readme/runnererr.png)

# How to run programm?
```
./build/task/currents
```
