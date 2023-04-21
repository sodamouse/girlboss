#pragma once

// TODO Add unrecognized argument feedback
// TODO Print the type and default value in help message

/* ------------------------------------------------------------------------------
    The MIT License (MIT)

    Copyright (c) 2023 sodamouse

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---------------------------------------------------------------------------------
           ..
          =@@#
         .@@@@@#=:
           -#@@@@@@#+=-::.
          ...=@@@##%@@@@@@@@@%%#**+:
          ....=+-   .:+*%@@@@@@@@@@@.
                        :%@@@@#@@@@@=
                      .*@@@@@@%@@@@@%
                    :*@@@@@@@@@@@@@@@-
                  -#@@@@@%=@@@@@@@@@@*
               .=%@@@@@*-  =@@@@@@@@@@        .:--:  --.
            :=#@@@@#=:      *@@@@@@@@@. :=+*%@@@@@@@@@@@%+     :-=+=
        :=*%@@@@+:          .%@@@@@@@@@@@@@@@@@@@@@@@@@@@@*    %@@@@@@%+=:
   .%@@@@@@@@@@*             :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%++*@@@@@@@@@@@+
    -%@%+-:.*%+.              +@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@=
          .+-                 .@@@@@@@@@@%+:. .:%@@@@@@@@@@@@@@@@@@@@@@@@@@*
         .-                    :*%@@@@@%+       +%@@@@%%####*#%%%%#*++***#**.
                                   ...
    sSSSSs   .S   .S_sSSs    S.       .S_SSSs      sSSs_sSSs      sSSs    sSSs
   d%%%%SP  .SS  .SS~YS%%b   SS.     .SS~SSSSS    d%%SP~YS%%b    d%%SP   d%%SP
  d%S'      S%S  S%S   `S%b  S%S     S%S   SSSS  d%S'     `S%b  d%S'    d%S'
  S%S       S%S  S%S    S%S  S%S     S%S    S%S  S%S       S%S  S%|     S%|
  S&S       S&S  S%S    d*S  S&S     S%S SSSS%P  S&S       S&S  S&S     S&S
  S&S       S&S  S&S   .S*S  S&S     S&S  SSSY   S&S       S&S  Y&Ss    Y&Ss
  S&S       S&S  S&S_sdSSS   S&S     S&S    S&S  S&S       S&S  `S&&S   `S&&S
  S&S sSSs  S&S  S&S~YSY%b   S&S     S&S    S&S  S&S       S&S    `S*S    `S*S
  S*b `S%%  S*S  S*S   `S%b  S*b     S*S    S&S  S*b       d*S     l*S     l*S
  S*S   S%  S*S  S*S    S%S  S*S.    S*S    S*S  S*S.     .S*S    .S*P    .S*P
   SS_sSSS  S*S  S*S    S&S   SSSbs  S*S SSSSP    SSSbs_sdSSS   sSS*S   sSS*S
    Y~YSSY  S*S  S*S    SSS    YSSP  S*S  SSY      YSSP~YSSY    YSS'    YSS'
            SP   SP                  SP
            Y    Y                   Y

    Girlboss is a stb-style cli argument parser inspired by Go's Flag moduel.
    Quickstart:
        - Create `girlboss.cpp` file
        - #define GIRLBOSS_IMPL
        - Include this file

    You will then be able to include this header file in your files and use it.
-------------------------------------------------------------------------------*/

namespace Girlboss {

bool* flag_bool(const char* name, bool defaultValue, const char* docstring);
int* flag_int(const char* name, int defaultValue, const char* docstring);
double* flag_double(const char* name, double defaultValue, const char* docstring);
const char** flag_str(const char* name, const char* defaultValue, const char* docstring);
bool* flag_option(const char* name, bool defaultValue, const char* docstring);
void parse(const char* programName, int argc, char* argv[]);
void print_usage();

} // namespace Girlboss

#ifdef GIRLBOSS_IMPL

#include <cassert>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>

namespace Girlboss {
struct Flag;
}

namespace Girlboss {

enum FlagType { FLAG_BOOL, FLAG_INT, FLAG_DOUBLE, FLAG_STR, FLAG_OPTION };

struct Flag
{
    FlagType type;
    const char* name;

    union {
        bool asBool;
        int asInt;
        double asDouble;
        const char* asStr;
    };

    const char* docstring;
};

static constexpr int FLAG_CAP = 100;
static Flag FLAGS[FLAG_CAP];
static int flagIndex = 0;
const char* PROGRAM_NAME;

static char* shift_array(int& argc, char**& argv)
{
    assert(argc > 0 && "Ran out of arguments!");
    char* result = *argv;
    --argc;
    ++argv;

    return result;
}

void parse(const char* programName, int argc, char* argv[])
{
    PROGRAM_NAME = programName;
    shift_array(argc, argv);

    while (argc > 0)
    {
        char* arg = shift_array(argc, argv);
        for (int i = 0; i < flagIndex; ++i)
        {
            if (std::strcmp(arg, FLAGS[i].name) == 0)
            {
                switch (FLAGS[i].type)
                {
                case FLAG_BOOL: {
                    if (argc == 0)
                    {
                        printf("Missing positional argument: %s [MISSING]\n", FLAGS[i].name);
                        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
                        exit(-1);
                    }

                    const char* value = shift_array(argc, argv);

                    if (std::strcmp(value, "false") == 0)
                    {
                        FLAGS[i].asBool = false;
                    }

                    else if (std::strcmp(value, "true") == 0)
                    {
                        FLAGS[i].asBool = true;
                    }

                    else
                    {
                        printf("Boolean argument expects 'true' or "
                                   "'false': %s [true/false]",
                                   FLAGS[i].name);
                        exit(-1);
                    }
                }
                break;
                case FLAG_INT: {
                    if (argc == 0)
                    {
                        printf("Missing positional argument: %s [MISSING]\n", FLAGS[i].name);
                        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
                        exit(-1);
                    }

                    const char* value = shift_array(argc, argv);

                    try
                    {
                        FLAGS[i].asInt = std::stoi(value);
                    }

                    catch (const std::exception& error)
                    {
                        printf("Could not set argument '%s' with value '%s'\n", FLAGS[i].name,
                                   value);
                        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
                        exit(-1);
                    }
                }
                break;
                case FLAG_DOUBLE: {
                    if (argc == 0)
                    {
                        printf("Missing positional argument: %s [MISSING]\n", FLAGS[i].name);
                        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
                        exit(-1);
                    }

                    const char* value = shift_array(argc, argv);

                    try
                    {
                        FLAGS[i].asDouble = std::atof(value);
                    }

                    catch (const std::exception& error)
                    {
                        printf("Could not set argument '%s' with value '%s'\n", FLAGS[i].name,
                                   value);
                        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
                        exit(-1);
                    }
                }
                break;
                case FLAG_STR: {
                    if (argc == 0)
                    {
                        printf("Missing positional argument: %s [MISSING]\n", FLAGS[i].name);
                        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
                        exit(-1);
                    }

                    const char* value = shift_array(argc, argv);
                    FLAGS[i].asStr = value;
                }
                break;
                case FLAG_OPTION: {
                    FLAGS[i].asBool = !FLAGS[i].asBool;
                };
                break;
                }
            }
        }
    }
}

void print_usage()
{
    printf("Usage: %s [options]\n", PROGRAM_NAME);
    for (int i = 0; i < flagIndex; ++i)
    {
        printf("\t%s\t%s\n", FLAGS[i].name, FLAGS[i].docstring);
    }
}

static Flag* flag_new(FlagType type, const char* name, const char* docstring)
{
    Flag* flag = &FLAGS[flagIndex++];
    flag->type = type;
    flag->name = name;
    flag->docstring = docstring;

    return flag;
}

bool* flag_bool(const char* name, bool defaultValue, const char* docstring)
{
    auto* flag = flag_new(FLAG_BOOL, name, docstring);
    flag->asBool = defaultValue;

    return &flag->asBool;
}

int* flag_int(const char* name, int defaultValue, const char* docstring)
{
    auto* flag = flag_new(FLAG_INT, name, docstring);
    flag->asInt = defaultValue;

    return &flag->asInt;
}

double* flag_double(const char* name, double defaultValue, const char* docstring)
{
    auto* flag = flag_new(FLAG_DOUBLE, name, docstring);
    flag->asDouble = defaultValue;

    return &flag->asDouble;
}

const char** flag_str(const char* name, const char* defaultValue, const char* docstring)
{
    auto* flag = flag_new(FLAG_STR, name, docstring);
    flag->asStr = defaultValue;

    return &flag->asStr;
}

bool* flag_option(const char* name, bool defaultValue, const char* docstring)
{
    auto* flag = flag_new(FLAG_OPTION, name, docstring);
    flag->asBool = defaultValue;

    return &flag->asBool;
}

} // namespace Girlboss

#endif
