/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <cstring>
#include <vector>
#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/imageformat.hh>
#include <iod/json.hh>

namespace im {
    
    bool match_magic(byte_source* src, const char* magic, const std::size_t n) {
        if (!src->can_seek()) { return false; }
        std::vector<byte> buf;
        buf.resize(n);
        const int n_read = static_cast<int>(src->read(&buf.front(), n));
        src->seek_relative(-n_read);
        return (n_read == n && std::memcmp(&buf.front(), magic, n) == 0);
    }
    
    bool match_magic(byte_source* src, const std::string& magic) {
        return match_magic(src, magic.c_str(), magic.size());
    }
    
    DECLARE_FORMAT_OPTIONS(ImageFormat);
    
    std::string ImageFormat::get_suffix() const   { return ImageFormat::options.suffix; }
    std::string ImageFormat::get_mimetype() const { return ImageFormat::options.mimetype; }
    
    /// including <iod/json.hh> along with Halide.h will cause a conflict --
    /// -- some macro called `user_error` I believe -- that won't compile.
    /// This static method is defined in here for this reason.
    options_map ImageFormat::encode_options(options_t which_options) {
        return options_map::parse(iod::json_encode(which_options));
    }
    
    options_map ImageFormat::get_options() const {
        return options_map();
    }
    
    options_map ImageFormat::add_options(options_map const& opts) const {
        return options_map().update(opts);
    }
    
    ImageFormat::~ImageFormat() {}
    
    std::unique_ptr<Image> ImageFormat::read(byte_source* src,
                                             ImageFactory* factory,
                                             const options_map &opts) {
        imread_raise_default(NotImplementedError);
    }
    
    ImageList ImageFormat::read_multi(byte_source* src,
                                      ImageFactory* factory,
                                      const options_map& opts) {
        imread_raise_default(NotImplementedError);
    }
    
    void ImageFormat::write(Image& input,
                            byte_sink* output,
                            const options_map& opts) {
        imread_raise_default(NotImplementedError);
    }
    
    void ImageFormat::write_multi(ImageList& input,
                                  byte_sink* output,
                                  const options_map& opts) {
        imread_raise_default(NotImplementedError);
    }
    
    bool ImageFormat::format_can_read() const noexcept           { return false; }
    bool ImageFormat::format_can_read_multi() const noexcept     { return false; }
    bool ImageFormat::format_can_read_metadata() const noexcept  { return false; }
    bool ImageFormat::format_can_write() const noexcept          { return false; }
    bool ImageFormat::format_can_write_multi() const noexcept    { return false; }
    bool ImageFormat::format_can_write_metadata() const noexcept { return false; }
    
}
