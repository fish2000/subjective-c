
#include <map>
#include <string>
#include <iostream>
#include "libimread-config.h"
#include "docopt.h"

static const char USAGE[] = 

R"(Configuration for libimread

    Usage:
      libimread-config (--prefix          |
                        --exec-prefix     |
                        --includes        |
                        --libs            |
                        --cflags          |
                        --ldflags)
      libimread-config (-h | --help)
      libimread-config --version
    
    Options:
      --prefix        Show install prefix e.g. /usr/local.
      --exec-prefix   Show exec-prefix; should be same as the prefix.
      --includes      Show include flags
                        e.g. -I/usr/include -I/usr/local/include.
      --libs          Show library link flags
                        e.g. -limread -lHalide -framework CoreFoundation.
      --cflags        Show all compiler arguments and include flags.
      --ldflags       Show all library link flags and linker options.
      -h --help       Show this help screen.
      --version       Show version.

)";

const std::string VERSION = "libimread-config ";

int main(int argc, const char** argv) {
    using optmap_t = std::map<std::string, docopt::value>;
    using optpair_t = std::pair<std::string, docopt::value>;
    optmap_t args;
    optmap_t raw_args = docopt::docopt(USAGE,
                                       { argv + 1, argv + argc },
                                       true, /// show help
                                       VERSION + im::config::version);
    
    /// filter out all docopt parse artifacts,
    /// leaving only things beginning with "--"
    std::copy_if(raw_args.begin(), raw_args.end(),
                 std::inserter(args, args.begin()),
                 [](const optpair_t& p) { return p.first.substr(0, 2) == "--"; });
    
    // for (auto const& arg : args) {
    //     std::cout << arg.first << " --> " << arg.second << std::endl;
    // }
    
    /// print the value for the truthy option flag
    for (auto const& arg : args) {
        if (!bool(arg.second)) { continue; }
        if (arg.first == "--prefix") {
            std::cout << im::config::prefix << std::endl;
        } else if (arg.first == "--exec-prefix") {
            std::cout << im::config::exec_prefix << std::endl;
        } else if (arg.first == "--includes") {
            std::cout << im::config::includes << std::endl;
        } else if (arg.first == "--libs") {
            std::cout << im::config::libs << std::endl;
        } else if (arg.first == "--cflags") {
            std::cout << im::config::cflags << std::endl;
        } else if (arg.first == "--ldflags") {
            std::cout << im::config::ldflags << std::endl;
        }
    }

}
