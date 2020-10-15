#include "host_file_parser.hpp"

#include <iostream>
#include <fstream>

void HostFileParser::parse(std::string const& filename, HostList &list)
{
    std::ifstream ifs(filename);
    if (!ifs.good()) {
        throw std::runtime_error("Couldn't open host file " + filename);
    }

    std::regex hostname(R"(([a-zA-Z0-9\.\-]+):?:?)");
    std::regex hostname_sshport(R"(([a-zA-Z0-9\.\-]+):([1-9][0-9]*|0):?)");
    std::regex hostname_spreadport(R"(([a-zA-Z0-9\.\-]+)::([1-9][0-9]*|0))");
    std::regex hostname_sshport_spreadport(R"(([a-zA-Z0-9\.\-]+):([1-9][0-9]*|0):([1-9][0-9]*|0))");

    int lineno = 0;
    std::string line;
    while (std::getline(ifs, line))
    {
        lineno++;
        line = std::regex_replace(line, std::regex("^ +| +$| +"), "");

        if (std::regex_match(line, hostname)) {
            
            std::string hostname = line.substr(0, line.find(":"));
            list.emplace_back(hostname, "22", "4801");

        } else if (std::regex_match(line, hostname_sshport)) {
            
            auto del1 = line.find(":");
            auto del2 = line.find(":", del1+1);
            std::string hostname = line.substr(0, del1);
            std::string ssh_port;
            if (del2 == std::string::npos)
                ssh_port = line.substr(del1+1);
            else
                ssh_port = line.substr(del1+1, del2-del1-1);
            list.emplace_back(hostname, ssh_port, "4801");

        } else if (std::regex_match(line, hostname_spreadport)) {
            
            auto del1 = line.find(":");
            auto del2 = line.find(":", del1+1);
            std::string hostname = line.substr(0, del1);
            std::string spread_port = line.substr(del2+1);
            list.emplace_back(hostname, "22", spread_port);

        } else if (std::regex_match(line, hostname_sshport_spreadport)) {

            auto del1 = line.find(":");
            auto del2 = line.find(":", del1+1);
            std::string hostname = line.substr(0, del1);
            std::string ssh_port = line.substr(del1+1, del2-del1-1);
            std::string spread_port = line.substr(del2+1);
            list.emplace_back(hostname, ssh_port, spread_port);

        } else {
            throw std::runtime_error("Failed to parse line " + std::to_string(lineno) +  " from host file " + filename);
        }
    }
}
