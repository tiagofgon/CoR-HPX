#include <iostream>

#include <sp.h>

int main(int argc, char **argv)
{
    std::cout << (SPREAD_VERSION >> 24) << "." << ((SPREAD_VERSION >> 16) & 0xFF) << "." << (SPREAD_VERSION & 0xFF);
    return 0;
}
