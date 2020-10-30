// #include <iostream>
// #include <random>
// #include <algorithm>

// #include <boost/program_options.hpp>
// namespace po = boost::program_options;

// #include "host_file_parser.hpp"
// #include "cor/services/session_manager.hpp"

// std::string random_string(std::size_t length);

// int main(int argc, char const *argv[])
// {
//     HostList hosts;
//     int np;
//     std::string app_group;
//     std::string host_file;
//     std::string module;
//     std::vector<std::string> args;

//     try {

//         po::options_description options("Allowed options");
//         options.add_options()
//             ("version", "Print version")
//             ("help", "Print help message")
//             ("debug", "Print debug info")
//             ("np", po::value<int>(&np)->default_value(1), "Number of pods")
//             ("appgroup", po::value<std::string>(&app_group)->default_value(random_string(10)), "Application group name")
//             ("hostfile", po::value<std::string>(&host_file)->default_value(""), "Provide a file with a list of hosts")
//             ("module", po::value<std::string>(&module)->default_value("a.out"), "Module name")
//             ("args", po::value<std::vector<std::string>>(&args), "Module main function arguments")
//             ;

//         po::options_description cmdline;
//         cmdline.add(options);

//         po::positional_options_description p;
//         p.add("module", 1);
//         p.add("args", -1);

//         po::variables_map vm;
//         store(po::command_line_parser(argc, argv).options(cmdline).positional(p).run(), vm);
//         notify(vm);

//         if (vm.count("version")) {
//             std::cout << "Error: Version not implemented!" << "\n";
//             return EXIT_SUCCESS;
//         }

//         if (vm.count("help")) {
//             std::cout << options << "\n";
//             return EXIT_SUCCESS;
//         }

//         if (!host_file.empty()) {
//             HostFileParser::parse(host_file, hosts);
//         } else {
//             hosts.emplace_back("localhost", "22", "4801");
//         }

//         if (vm.count("debug")) {
//             // Print program options
//             std::cout << "Number of pods: " << np << "\n";
//             std::cout << "Application group: " << app_group << "\n";
//             std::cout << "Host file: " << host_file << "\n";
//             for (auto const& host: hosts)
//                 std::cout << std::get<0>(host) << "\t" << std::get<1>(host) << "\t" << std::get<2>(host) << "\n";
//             std::cout << "Module: " << module << "\n";
//             std::cout << "Args: ";
//             for (auto const& arg: args)
//                 std::cout << arg << " ";
//             std::cout << "\n";
//             return EXIT_SUCCESS;
//         }

//     } catch (std::exception& ex) {
//         std::cerr << "Error: " << ex.what() << "\n";
//         return EXIT_FAILURE;
//     }

//     // assemble command
//     std::string cmd;
//     cmd.append("/opt/placor-hpx/bin/corx");
//     cmd.append(" ");
//     cmd.append(app_group);
//     cmd.append(" ");
//     cmd.append(app_group);
//     cmd.append(" ");
//     cmd.append(std::to_string(np));
//     cmd.append(" ");
//     cmd.append(std::to_string(0));
//     cmd.append(" ");
//     cmd.append(module);
//     for (int i = 0; i < args.size(); ++i) {
//         cmd.append(" ");
//         cmd.append(args[i]);
//     }

//     auto sm = new cor::SessionManager();

//     // Start sessions
//     for (int i = 0; i < np; ++i) {
//         auto pos = i % hosts.size();
//         auto host = hosts.at(pos);
//         sm->CreateRemoteSession(std::get<0>(host), std::get<1>(host), cmd);
//     }

//     sm->StopService();

//     delete sm;

//     return EXIT_SUCCESS;
// }

// std::string random_string(std::size_t length)
// {
//     std::string str(length, 0);

//     // default chars used to generate a random string
//     std::string const default_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

//     // create a non-deterministic random number generator
//     std::default_random_engine rng(std::random_device{}());

//     // create a random number "shaper" that will give us
//     // uniformly distributed indices into the character set
//     std::uniform_int_distribution<std::size_t> dist{0, default_chars.length()-1};

//     // generate random string
//     std::generate_n(str.begin(), length, [&]{ return default_chars[dist(rng)]; });

//     return str;
// }
