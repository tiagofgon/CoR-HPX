#include "cor/error.hpp"

namespace cor {

    Error::Error(std::string const& what_arg) :
        std::runtime_error(what_arg)
    {}

}
