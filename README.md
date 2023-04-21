# Girlboss
Girlboss is a /fast/ and \easy\ header-only commandline argument parser written in C++.
It is inspired by Go's Flag module, because it's just brilliant.

## Features
- Support for bool, int, double, string, option (like bool, but only the flag name is required)
- Easily extensible to custom types
- Automatic help message generation
- Fast and simple

## Usage
Enable it in your project like any other stb style, header-only library:
- Create `girlboss.cpp` file
- Include in this file the `girlboss.hpp` header
- #define GIRLBOSS_IMPL

### Example:
``` c++
#include "girlboss.hpp"

int main(int argc, char* argv[])
{
    bool* boolFlag = Girlboss::flag_option("-o", false, "An option flag");
    int* intFlag = Girlboss::flag_int("-i", 12, "An integer flag");
    double* doubleFlag = Girlboss::flag_double("-d", false, "A double flag");
    const char** strFlag = Girlboss::flag_str("-s", nullptr, "A str flag");
    bool* optionFlag = Girlboss::flag_option("-h", false, "Display this message");

    Girlboss::parse("program name", argc, argv);

    if (*optionFlag)
        Girlboss::print_usage();

    return 0;
}
```

Output:

``` shell
Usage: program name [options]
        -o      An option flag
        -i      An integer flag
        -d      A double flag
        -s      A str flag
        -h      Display this message
```

## Why Girlboss?
Because someone paid me $100 to name it like this and include the ascii art in the file.
