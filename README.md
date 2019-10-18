# shush::cpu
A CPU emulator that executes custom bytecode.

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests, "-ULIBRARY_TYPE -DLIBRARY_TYPE=SHARED" for building shared library
make
```

## Execution
```shell
cd build
./PROJECT_NAME
```

## Build documentation
```shell
doxygen
```
Documentation will be generated in the directory named `docs`.