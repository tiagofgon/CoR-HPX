#ifndef ERROR_HPP
#define ERROR_HPP

#include <stdexcept>
#include <string>

namespace cor {

    class Error: std::runtime_error
    {

    public:
        Error(std::string const& what_arg);

    };

}

#endif
