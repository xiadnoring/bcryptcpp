# Example

## Build

Prepare dependencies
```bash
conan install . --output-folder=CMakeFiles
```

Generate build files
```bash
cmake . -DCMAKE_BUILD_TYPE=Release
```
Build
```bash
make
```

## Result

```bash
./test
Test#0 OK
Test#1 OK
```