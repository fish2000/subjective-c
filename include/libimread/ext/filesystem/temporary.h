/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_FILESYSTEM_TEMPORARY_H_
#define LIBIMREAD_EXT_FILESYSTEM_TEMPORARY_H_

#include <string>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <cstddef>
#include <dirent.h>

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/ext/filesystem/path.h>

namespace filesystem {
    
    struct NamedTemporaryFile {
        /// As per the eponymous tempfile.NamedTemporaryFile,
        /// of the Python standard library. NOW WITH RAII!!
        
        static constexpr char tfp[] = FILESYSTEM_TEMP_FILENAME;
        static constexpr char tfs[] = FILESYSTEM_TEMP_SUFFIX;
        
        mode mm;
        char *suffix;
        char *prefix;
        bool cleanup;
        path filepath;
        
        explicit NamedTemporaryFile(const char *s = tfs, const char *p = tfp,
                                    const path &td = path::tmp(), bool c = true, mode m = mode::WRITE)
                                        :mm(m), cleanup(c), suffix(::strdup(s)), prefix(::strdup(p))
                                        ,filepath(td/::strcat(::strdup(p), s))
                                        {
                                            create();
                                        }
        explicit NamedTemporaryFile(const std::string &s, const std::string &p = tfp,
                                    const path &td = path::tmp(), bool c = true, mode m = mode::WRITE)
                                        :mm(m), cleanup(c), suffix(::strdup(s.c_str())), prefix(::strdup(p.c_str()))
                                        ,filepath(td/(p+s))
                                        {
                                            create();
                                        }
        
        inline std::string   str()  { return filepath.str(); }
        inline const char *c_str()  { return filepath.c_str(); }
        operator std::string()      { return str(); }
        operator const char*()      { return c_str(); }
        
        void create();
        void remove();
        
        ~NamedTemporaryFile() {
            if (cleanup) { remove(); }
        }
    
    };
    
    struct TemporaryDirectory {
        
        static constexpr char tdp[] = FILESYSTEM_TEMP_DIRECTORYNAME;
        static constexpr char tfp[] = FILESYSTEM_TEMP_FILENAME;
        static constexpr char tfs[] = FILESYSTEM_TEMP_SUFFIX;
        
        char *tpl;
        bool cleanup;
        path tplpath;
        path dirpath;
        
        explicit TemporaryDirectory(const char *t, bool c = true)
            :tpl(::strdup(t))
            ,cleanup(c)
            ,tplpath(path::join(path::gettmp(), path(t)))
            {
                dirpath = path(::mkdtemp(::strdup(tplpath.c_str())));
                // WTF("Creating TemporaryDirectory with params:",
                //     FF("\tt = (const char *)%s", t),
                //     FF("\tc = %s", c ? "true" : "false"),
                //     FF("\ttpl = %s", tpl),
                //     FF("\tcleanup = %s", cleanup ? "true" : "false"),
                //     FF("\tdirpath = %s", dirpath.c_str()),
                //     FF("\ttplpath = %s", tplpath.c_str())
                // );
            }
        
        explicit TemporaryDirectory(const std::string &t, bool c = true)
            :tpl(::strdup(t.c_str()))
            ,cleanup(c)
            ,tplpath(path::join(path::gettmp(), path(t)))
            {
                dirpath = path(::mkdtemp(::strdup(tplpath.c_str())));
                // WTF("Creating TemporaryDirectory with params:",
                //     FF("\tt = (const std::string &)%s", t.c_str()),
                //     FF("\tc = %s", c ? "true" : "false"),
                //     FF("\ttpl = %s", tpl),
                //     FF("\tcleanup = %s", cleanup ? "true" : "false"),
                //     FF("\tdirpath = %s", dirpath.c_str()),
                //     FF("\ttplpath = %s", tplpath.c_str())
                // );
            }
        
        operator std::string() { return dirpath.str(); }
        operator const char*() { return dirpath.c_str(); }
        
        NamedTemporaryFile get(const std::string &suffix = tfs,
                               const std::string &prefix = tfp,
                               mode m = mode::WRITE) { return NamedTemporaryFile(
                                                          suffix, prefix, dirpath, cleanup, m); }
        
        void clean();
        void remove();
        
        ~TemporaryDirectory() {
            if (cleanup) { clean(); remove(); }
        }
    
    };

} /* namespace filesystem */


#endif /// LIBIMREAD_EXT_FILESYSTEM_TEMPORARY_H_