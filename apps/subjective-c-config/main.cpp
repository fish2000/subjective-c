
#include <map>
#include <string>
#include <iostream>
#include "subjective-c-config.h"
#include "docopt.h"

static const char USAGE[] = R"([subjective-c-config] Configuration for subjective-c

    Usage:
      subjective-c-config (--prefix          |
                           --exec-prefix     |
                           --includes        |
                           --libs            |
                           --cflags          |
                           --ldflags)
      subjective-c-config (-h | --help)
      subjective-c-config --version
    
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

const std::string VERSION = "[subjective-c-config]{ subjective-c " + objc::config::version + " }";

int main(int argc, const char** argv) {
    using value_t = docopt::value;
    using optmap_t = std::map<std::string, value_t>;
    using optpair_t = std::pair<std::string, value_t>;
    value_t truth(true);
    optmap_t args;
    optmap_t raw_args = docopt::docopt(USAGE, { argv + 1, argv + argc },
                                       true, /// show help
                                       VERSION);
    
    /// filter out all docopt parse artifacts,
    /// leaving only things beginning with "--"
    std::copy_if(raw_args.begin(),
                 raw_args.end(),
                 std::inserter(args, args.begin()),
              [](optpair_t const& p) { return p.first.substr(0, 2) == "--"; });
    
    // for (auto const& arg : args) {
    //     std::cout << arg.first << " --> " << arg.second << std::endl;
    // }
    
    /// print the value for the truthy option flag
    for (auto const& arg : args) {
        if (arg.second == truth) {
            if (arg.first == "--prefix") {
                std::cout << objc::config::prefix << std::endl;
                break;
            } else if (arg.first == "--exec-prefix") {
                std::cout << objc::config::exec_prefix << std::endl;
                break;
            } else if (arg.first == "--includes") {
                std::cout << objc::config::includes << std::endl;
                break;
            } else if (arg.first == "--libs") {
                std::cout << objc::config::libs << std::endl;
                break;
            } else if (arg.first == "--cflags") {
                std::cout << objc::config::cflags << std::endl;
                break;
            } else if (arg.first == "--ldflags") {
                std::cout << objc::config::ldflags << std::endl;
                break;
            }
        }
    }

}
