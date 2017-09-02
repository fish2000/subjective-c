
#include <string>
#include <numeric>
#include <algorithm>

#include <subjective-c/subjective-c.hpp>
#include <libimread/errors.hh>
#include <libimread/imageformat.hh>
#include <libimread/options.hh>
#include <libimread/symbols.hh>
#include <iod/json.hh>

#include "include/catch.hpp"

namespace {
    
    using objc::byte;
    using objc::bytevec_t;
    using im::ImageFormat;
    using im::Options;
    using stringvec_t = std::vector<std::string>;
    
    TEST_CASE("[imageformat-options] Check registered formats",
              "[imageformat-options-check-registered-formats]")
    {
        auto DMV = ImageFormat::registry();
        stringvec_t formats;
        std::string joined;
        int idx = 0,
            max = 0;
        
        std::transform(DMV.begin(), DMV.end(),
                       std::back_inserter(formats),
                    [](auto const& registrant) {
            return std::string(registrant.first);
        });
        
        joined = std::accumulate(formats.begin(), formats.end(),
                                 std::string{},
                      [&formats](std::string const& lhs,
                                 std::string const& rhs) {
            return lhs + rhs + (rhs == formats.back() ? "" : ", ");
        });
        
        WTF("REGISTRY:",
            FF("\t contains %i formats:", max = formats.size()),
            FF("\t %s", joined.c_str()));
        
        for (auto it = formats.begin();
            it != formats.end() && idx < max;
            ++it) { std::string const& format = *it;
                auto format_ptr = ImageFormat::named(format);
                Options opts = format_ptr->get_options();
                
                WTF("", FF("FORMAT: %s", format.c_str()),
                    "\nAs JSON:\n", opts.format(),
                    "\nAs encoded IOD:\n",
                    iod::json_encode(format_ptr->options),
                    iod::json_encode(format_ptr->capacity));
                
            ++idx; }
        
    }
    
};
