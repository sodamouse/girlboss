#define GIRLBOSS_IMPL
#include "girlboss.hpp"

int main(int argc, char* argv[])
{
    using namespace Girlboss;
    bool* flagA = flag_bool("-b", false, "Some flag");
    int* flagB = flag_int("-i", 1000, "Some flag");
    double* flagC = flag_double("-d", 3.14, "Some flag");
    const char** flagD = flag_str("-s", "some string", "Some flag");
    bool* flagE = flag_option("-b", false, "Some flag");

    (void)flagA;
    (void)flagB;
    (void)flagC;
    (void)flagD;
    (void)flagE;

    parse("test", argc, argv);

    print_usage();

    return 0;
}
