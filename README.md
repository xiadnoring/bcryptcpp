# Bcrypt for C++

## Introduction
A c++ wrapper for [libxcrypt](https://github.com/besser82/libxcrypt) around ```bcrypt``` password hashing

## Dependencies
For compile this library, you need to install below libraries:
- [libxcrypt](https://github.com/besser82/libxcrypt) 4.4.6 or greater
- [fmt](https://github.com/fmtlib/fmt) 11.0.2 or greater

### For Arch Linux
```bash
pacman -S libxcrypt fmt
```

or

```bash
paru -S libxcrypt fmt
```

### For Ubuntu
```bash
apt-get install libcrypt-dev libfmt-dev
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
#include <bcryptcpp/bcrypt.h>
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
    valid = compare("12345", passwd);
}
catch (const bcrypt::exception::compare &e) 
{
    std::cerr << e.what() << "\n";
}
```