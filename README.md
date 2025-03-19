# TouHou-cpp
Level editor for TouHou-cpp project.

# Build dependencies

- cmake (3.16 minimum)
- g++ (c++17 should be supported; or any other compiler you want, might not work)
- ninja (or any other generator you want, might not work)
- Qt6 (6.4.2)
- ProtoBuf (3.21 used)
- clang-tidy (1:18 is used; can be disabled in CMakeLists.txt)

# Generate proto files

Do this if you want to edit code (isn`t needed for building the project)

```
protoc --proto_path=./src/proto/ --cpp_out=./src/editor/ ./src/proto/game.proto
```

# Build
```
cmake -B build/
cmake --build build/
```
Or for debug build:
```
cmake -B build/ -DENABLE_DEBUG=True
cmake --build build/
```
