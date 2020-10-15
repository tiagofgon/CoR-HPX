#ifndef HOST_FILE_PARSER
#define HOST_FILE_PARSER

#include <vector>
#include <tuple>
#include <string>
#include <regex>

using HostList = std::vector<std::tuple<std::string, std::string, std::string>>;

class HostFileParser
{

public:
    static void parse(std::string const& filename, HostList &list);

};

#endif
