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
```
chmod "+x" test_gen.py
./test_gen.py [NUMBER_OF_NODES] [FILE_FOR_PUT_TEST]
```

# How to run programm?
```
./build/task/currents
```
