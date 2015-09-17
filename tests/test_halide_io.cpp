
#include <libimread/libimread.hpp>
#include <libimread/base.hh>
#include <libimread/halide.hh>
#include <libimread/IO/jpeg.hh>
#include <libimread/IO/png.hh>

#include "include/catch.hpp"

#define D(pth) "/Users/fish/Dropbox/libimread/tests/data/" pth
#define T(pth) "/tmp/" pth

namespace {
    
    using namespace Halide;
    // using namespace im::format;
    using filesystem::path;
    using U8Image = im::HybridImage<uint8_t>;
    
    TEST_CASE("[halide-io] Read PNG files",
              "[halide-read-png]")
    {
        U8Image halim = im::halide::read(D("roses_512_rrt_srgb.png"));
        U8Image halim2 = im::halide::read(D("marci_512_srgb.png"));
        U8Image halim3 = im::halide::read(D("marci_512_srgb8.png"));
    }
    
    TEST_CASE("[halide-io] Read a PNG and rewrite it as a JPEG using tmpwrite()",
              "[halide-read-jpeg-write-png-tmpwrite]")
    {
        using im::format::JPG;
        
        U8Image halim = im::halide::read(D("roses_512_rrt_srgb.png"));
        auto tf = im::halide::tmpwrite<JPG>(halim);
        CHECK(path::remove(tf));
        
        U8Image halim2 = im::halide::read(D("marci_512_srgb.png"));
        auto tf2 = im::halide::tmpwrite<JPG>(halim2);
        CHECK(path::remove(tf2));
        
        U8Image halim3 = im::halide::read(D("marci_512_srgb8.png"));
        auto tf3 = im::halide::tmpwrite<JPG>(halim3);
        CHECK(path::remove(tf3));
    }
    
    TEST_CASE("[halide-io] Read JPEG files",
              "[halide-read-jpeg]")
    {
        U8Image halim = im::halide::read(D("tumblr_mgq73sTl6z1qb9r7fo1_r1_500.jpg"));
        U8Image halim2 = im::halide::read(D("IMG_4332.jpg"));
        U8Image halim3 = im::halide::read(D("IMG_7333.jpeg"));
    }
    
    TEST_CASE("[halide-io] Read a JPEG and rewrite it as a PNG using tmpwrite()",
              "[halide-read-jpeg-write-png-tmpwrite]")
    {
        using im::format::PNG;
        
        U8Image halim = im::halide::read(D("tumblr_mgq73sTl6z1qb9r7fo1_r1_500.jpg"));
        auto tf = im::halide::tmpwrite<PNG>(halim);
        CHECK(path::remove(tf));
        
        U8Image halim2 = im::halide::read(D("IMG_4332.jpg"));
        auto tf2 = im::halide::tmpwrite<PNG>(halim2);
        CHECK(path::remove(tf2));
        
        U8Image halim3 = im::halide::read(D("IMG_7333.jpeg"));
        auto tf3 = im::halide::tmpwrite<PNG>(halim3);
        CHECK(path::remove(tf3));
        
        U8Image halim4 = im::halide::read(D("10954288_342637995941364_1354507656_n.jpg"));
        auto tf4 = im::halide::tmpwrite<PNG>(halim4);
        CHECK(path::remove(tf4));
    }
    
    TEST_CASE("[halide-io] Read a TIFF, rewrite it as a PNG using tmpwrite()",
              "[halide-read-tiff-write-png-tmpwrite]")
    {
        using im::format::PNG;
        
        U8Image halim = im::halide::read(D("ptlobos.tif"));
        auto tf = im::halide::tmpwrite<PNG>(halim);
        CHECK(path::remove(tf));
    }
    
    TEST_CASE("[halide-io] Write multiple formats as PPM",
              "[halide-read-tiff-write-ppm]")
    {
        im::fs::TemporaryDirectory td("test-halide-io");
        
        U8Image halim = im::halide::read(D("ptlobos.tif"));
        im::halide::write(halim, td.dirpath/"PPM_DUG986.ppm");
        U8Image halim2 = im::halide::read(td.dirpath/"PPM_DUG986.ppm");
        im::halide::write(halim2, td.dirpath/"PPM_YO_DOGG222.png");
        
        U8Image halim3 = im::halide::read(D("tumblr_mgq73sTl6z1qb9r7fo1_r1_500.jpg"));
        im::halide::write(halim3, td.dirpath/"PPM_OH_DOGGGGG.ppm");
        U8Image halim4 = im::halide::read(td.dirpath/"PPM_OH_DOGGGGG.ppm");
        im::halide::write(halim4, td.dirpath/"PPM_IMG_DOGGGGGGGGG.png");
    }
    
    TEST_CASE("[halide-io] Check the dimensions of an image",
              "[halide-read-jpg-png-image-dims]")
    {
        U8Image halim = im::halide::read(D("tumblr_mgq73sTl6z1qb9r7fo1_r1_500.jpg"));
        U8Image halim2 = im::halide::read(D("marci_512_srgb8.png"));
    }
    
}