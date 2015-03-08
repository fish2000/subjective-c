
#ifndef IMREAD_SYMBOLS_HH
#define IMREAD_SYMBOLS_HH

#include <iod/callable_traits.hh>
#include <iod/sio.hh>
#include <iod/symbol.hh>
#include <iod/tuple_utils.hh>
#include <iod/utils.hh>
#include <iod/bind_method.hh>
#include <libimread/libimread.hpp>
#include <libimread/symbols.hpp>
using namespace iod;

/// IOD symbols for all possible options
namespace symbols {
    
    /// These are supplmented at compile-time
    /// by this next include file, which
    /// is autogenerated as part of the cmake
    /// routine, using iod_generate_symbols
    
    /// File format symbols
    iod_define_symbol(Apple);
    iod_define_symbol(BMP);
    iod_define_symbol(JPG);
    iod_define_symbol(LSM);
    iod_define_symbol(PNG);
    iod_define_symbol(PVR);
    iod_define_symbol(TIFF);
    iod_define_symbol(WebP);
    
    /// Image data symbols
    iod_define_symbol(NumPy);
    iod_define_symbol(Halide);
    iod_define_symbol(Vpp);
    iod_define_symbol(Dige);
    iod_define_symbol(CV);
    iod_define_symbol(CImg);
    iod_define_symbol(NS);
    iod_define_symbol(CG);
    iod_define_symbol(CI);
    iod_define_symbol(CA);
    
    /// Image-y, relevant data
    iod_define_symbol(metadata);              /// Generally-used string data (q.v. ImageWithMetadata, TIFF)
    iod_define_symbol(datatype);              /// Generally-used identifiers (e.g. ext/pvr.cpp#63, NumPy dtypes etc)
    iod_define_symbol(name);                  /// Halide wants to name its Image<T> objects for some reason
    iod_define_symbol(filename);              /// Optional; self-explanatory
    iod_define_symbol(filetype);              /// See also UTI.mm
    iod_define_symbol(filemagic);             /// Only two formats have 'em (but IO/lsm.cpp has a define)
    iod_define_symbol(colorspace);            /// ... and also profile, intent, etc etc you name it
    
    /// Catch-alls
    iod_define_symbol(backend);
    iod_define_symbol(suffix);
    iod_define_symbol(load);
    iod_define_symbol(save);
    iod_define_symbol(read);
    iod_define_symbol(read_multi);
    iod_define_symbol(read_metadata);
    iod_define_symbol(write);
    iod_define_symbol(write_metadata);
    iod_define_symbol(write_multi);
    
    /// Extracted from ImageFormat impls
    iod_define_symbol(quality);               /// from JPG (an int)
    iod_define_symbol(compress);              /// from TIFF (boolean)
    iod_define_symbol(compression_level);     /// from PNG (a quantity)
    //iod_define_symbol(horizontal-predictor);  /// from TIFF (boolean)
    
    /// note how the above param name has an infix hyphen, but the one before it
    /// uses an underscore. BLECH
    
    iod_define_symbol(XResolution);           /// TIFF (either an int or a float, evidently)
    iod_define_symbol(YResolution);           /// Ibid.
    iod_define_symbol(XResolutionUnit);       /// TIFF again (no YResolutionUnit, ??)
    iod_define_symbol(palette);
    iod_define_symbol(framerate);
    
}

#endif /// IMREAD_SYMBOLS_HH