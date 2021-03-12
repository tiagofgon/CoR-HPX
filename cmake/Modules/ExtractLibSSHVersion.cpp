#include <iostream>

#include <libssh/libssh.h>

int main(int argc, char **argv)
{
    std::cout << LIBSSH_VERSION_MAJOR << "." << LIBSSH_VERSION_MINOR << "." << LIBSSH_VERSION_MICRO;
    return 0;
}
