# Bcrypt for C++

## Introduction
A c++ wrapper for [libxcrypt](https://github.com/besser82/libxcrypt) around ```bcrypt``` password hashing
## Dependencies
For compile this library if you use c++ std **less than** 20, you need to install below libraries:
- [fmt](https://github.com/fmtlib/fmt) 11.0.2 or greater

### For Arch Linux
```bash
pacman -S fmt
```

or

```bash
paru -S fmt
```

### For Ubuntu
```bash
apt-get install libfmt-dev
```

## Build

### With Conan
```bash
cmake ... -DWITH_CONAN=ON
```

or 

```bash
conan create . --build=missing
```

- CMake package name: ```bcryptcpp```
- CMake target name: ```bcryptcpp::bcryptcpp```

A simple use case using the CMake file name and the global target:

```cmake
find_package(bcryptcpp REQUIRED)
# ...
target_link_libraries(YOUR_TARGET bcryptcpp::bcryptcpp)
```

For an example of use, see the ```./test``` folder

## Example

```c++
#include <bcryptcpp/bcryptcpp.h>
...
std::string passwd = "12345";
std::string hash = bcrypt::hash (passwd, bcrypt::gensalt (12));
...
bool valid = bcrypt::compare (hash, passwd);
```

```c++
#include <iostream>
...
try 
{
    // invalid hash
    valid = bcrypt::compare("12345", passwd);
}
catch (const bcrypt::exception::compare &e) 
{
    std::cerr << e.what() << "\n";
}
```