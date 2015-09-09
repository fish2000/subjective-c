// Copyright 2015 Wenzel Jakob. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if !defined(LIBIMREAD_EXT_FILESYSTEM_PATH_H_)
#define LIBIMREAD_EXT_FILESYSTEM_PATH_H_

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>
#include <regex>
#include <sstream>

#include <cctype>
#include <cstdlib>
#include <cstddef>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <dirent.h>

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>

namespace filesystem {
    
    enum class mode { READ, WRITE };
    
    /// forward declaration for these next few prototypes/templates
    class path;
    
    namespace detail {
        
        /// Deleter structures to close directory and file handles
        template <typename D>
        struct dircloser {
            constexpr dircloser() noexcept = default;
            template <typename U> dircloser(const dircloser<U>&) noexcept {};
            void operator()(D *dirhandle) { ::closedir(dirhandle); }
        };
    
        template <typename F>
        struct filecloser {
            constexpr filecloser() noexcept = default;
            template <typename U> filecloser(const filecloser<U>&) noexcept {};
            void operator()(F *filehandle) { ::fclose(filehandle); }
        };
        
    }
    
    using directory = std::unique_ptr<typename std::decay<DIR>::type, detail::dircloser<DIR>>;
    using file = std::unique_ptr<typename std::decay<FILE>::type, detail::filecloser<FILE>>;
    
    namespace detail {
        filesystem::directory ddopen(const char *c);
        filesystem::directory ddopen(const std::string &s);
        filesystem::directory ddopen(const path &p);
        
        filesystem::file ffopen(const std::string &s, mode m = mode::READ);
        
        inline const char *tmpdir() noexcept {
            /// cribbed/tweaked from boost
            const char *dirname;
            dirname = std::getenv("TMPDIR");
            if (NULL == dirname) { dirname = std::getenv("TMP"); }
            if (NULL == dirname) { dirname = std::getenv("TEMP"); }
            if (NULL == dirname) { dirname = "/tmp"; }
            return dirname;
        }
        
    }
    
    /**
     * \brief Simple class for manipulating paths on Linux/Windows/Mac OS
     *
     * This class is just a temporary workaround to avoid the heavy boost
     * dependency until boost::filesystem is integrated into the standard template
     * library at some point in the future.
     */
    class path {
        
        public:
            enum path_type {
                windows_path = 0, posix_path = 1,
                native_path = posix_path
            };
            
            path()
                :m_type(native_path)
                ,m_absolute(false)
                {}
            
            path(const path &path)
                :m_type(path.m_type)
                ,m_path(path.m_path)
                ,m_absolute(path.m_absolute)
                {}
            
            path(path &&path)
                :m_type(path.m_type)
                ,m_path(std::move(path.m_path))
                ,m_absolute(path.m_absolute)
                {}
            
            path(char *st)              { set(st); }
            path(const char *st)        { set(st); }
            path(const std::string &st) { set(st); }
            
            inline std::size_t size() const { return static_cast<std::size_t>(m_path.size()); }
            inline bool empty() const       { return m_path.empty(); }
            inline bool is_absolute() const { return m_absolute; }
            
            path make_absolute() const;
            
            template <typename P> inline
            static path absolute(P&& p) { return path(std::forward<P>(p)).make_absolute(); }
            
            bool compare_debug(const path &other) const;
            bool compare(const path &other) const;
            
            bool operator==(const path &other) const { return compare(other); }
            bool operator!=(const path &other) const { return !compare(other); }
            
            bool exists() const;
            bool is_file() const;
            bool is_directory() const;
            
            bool match(const std::regex &pattern,           bool case_sensitive=false);
            bool search(const std::regex &pattern,          bool case_sensitive=false);
            
            template <typename P> inline
            static bool match(P&& p, std::regex&& pattern,  bool case_sensitive=false) {
                return path(std::forward<P>(p)).match(
                    std::forward<std::regex>(pattern), case_sensitive);
            }
            
            template <typename P> inline
            static bool search(P&& p, std::regex&& pattern, bool case_sensitive=false) {
                return path(std::forward<P>(p)).search(
                    std::forward<std::regex>(pattern), case_sensitive);
            }
            
            std::vector<path> list(                                                         bool full_paths=false);
            std::vector<path> list(const char *pattern,                                     bool full_paths=false);
            std::vector<path> list(const std::string &pattern,                              bool full_paths=false);
            std::vector<path> list(const std::regex &pattern,   bool case_sensitive=false,  bool full_paths=false);
            
            template <typename P, typename G> inline
            static std::vector<path> list(P&& p, G&& g, bool full_paths=false) {
                return path(std::forward<P>(p)).list(std::forward<G>(g), full_paths);
            }
            
            bool remove();
            
            template <typename P> inline
            static bool remove(P&& p) {
                return path(std::forward<P>(p)).remove();
            }
            
            std::string extension() const {
                if (empty()) { return ""; }
                const std::string &last = m_path[m_path.size()-1];
                std::string::size_type pos = last.find_last_of(".");
                if (pos == std::string::npos) { return ""; }
                return last.substr(pos+1);
            }
            
            template <typename P> inline
            static std::string extension(P&& p) {
                return path(std::forward<P>(p)).extension();
            }
            
            path parent_path() const {
                path result;
                result.m_absolute = m_absolute;
                
                if (m_path.empty()) {
                    if (!m_absolute) { result.m_path.push_back(".."); }
                } else {
                    std::string::size_type idx = 0,
                                           until = m_path.size() - 1;
                    for (; idx < until; ++idx) {
                        result.m_path.push_back(m_path[idx]);
                    }
                }
                return result;
            }
            
            path join(const path &other) const {
                if (other.m_absolute) {
                    imread_raise(FileSystemError,
                        "path::operator/(): Expected a relative path!");
                }
                if (other.m_type != other.m_type) {
                    imread_raise(FileSystemError,
                        "path::operator/(): Expected a path of the same type!");
                }
                
                path result(*this);
                std::string::size_type idx = 0,
                                       max = other.m_path.size();
                
                for (; idx < max; ++idx) {
                    result.m_path.push_back(other.m_path[idx]);
                }
                return result;
            }
            
            path operator/(const path &other) const { return join(other); }
            path operator/(const char *other) const { return join(path(other)); }
            path operator/(const std::string &other) const { return join(path(other)); }
            
            template <typename P, typename Q> inline
            static path join(P&& one, Q&& theother) {
                return path(std::forward<P>(one)) / path(std::forward<Q>(theother));
            }
            
            std::string str(path_type type = native_path) const {
                std::ostringstream oss;
                char sep = (type == posix_path) ? '/' : '\\';
                if (type == posix_path && m_absolute) { oss << sep; }
                std::string::size_type idx = 0,
                                       siz = m_path.size();
                for (; idx < siz; ++idx) {
                    oss << m_path[idx];
                    if (idx + 1 < siz) { oss << sep; }
                }
                return oss.str();
            }
            
            inline const char *c_str() const { return str().c_str(); }
            
            static path getcwd() {
                char temp[PATH_MAX];
                if (::getcwd(temp, PATH_MAX) == NULL) {
                    imread_raise(FileSystemError,
                        "Internal error in getcwd():", strerror(errno));
                }
                return path(temp);
            }
            
            static path cwd()               { return path::getcwd(); }
            static path gettmp()            { return path(detail::tmpdir()); }
            static path tmp()               { return path(detail::tmpdir()); }
            
            operator std::string()          { return str(); }
            operator const char*()          { return c_str(); }
            
            void set(const std::string &str, path_type type = native_path) {
                m_type = type;
                if (type == windows_path) {
                    m_path = tokenize(str, "/\\");
                    m_absolute = str.size() >= 2 && std::isalpha(str[0]) && str[1] == ':';
                } else {
                    m_path = tokenize(str, "/");
                    m_absolute = !str.empty() && str[0] == '/';
                }
            }
            
            path &operator=(const std::string &str) { set(str); return *this; }
            path &operator=(const path &path) {
                m_type = path.m_type;
                m_path = path.m_path;
                m_absolute = path.m_absolute;
                return *this;
            }
            path &operator=(path &&path) {
                m_type = path.m_type;
                m_absolute = path.m_absolute;
                if (this != &path) {
                    m_path = std::move(path.m_path);
                } else {
                    m_path = path.m_path;
                }
                return *this;
            }
            
            friend std::ostream &operator<<(std::ostream &os, const path &path) {
                os << path.str();
                return os;
            }
            
        protected:
            static std::vector<std::string> tokenize(const std::string &source, const std::string &delim) {
                std::vector<std::string> tokens;
                std::string::size_type lastPos = 0,
                                       pos = source.find_first_of(delim, lastPos);
                
                while (lastPos != std::string::npos) {
                    if (pos != lastPos) {
                        tokens.push_back(source.substr(lastPos, pos - lastPos));
                    }
                    lastPos = pos;
                    if (lastPos == std::string::npos || lastPos + 1 == source.length()) { break; }
                    pos = source.find_first_of(delim, ++lastPos);
                }
                
                return tokens;
            }
            
        protected:
            path_type m_type;
            std::vector<std::string> m_path;
            bool m_absolute;
    };
    
    /// change directory temporarily with RAII
    struct switchdir {
        
        explicit switchdir(path nd)
            :olddir(path::cwd().str())
            ,newdir(nd.str())
            {
                mute.lock();
                ::chdir(newdir.c_str());
            }
        
        path from() const { return path(olddir); }
        
        ~switchdir() {
            ::chdir(olddir.c_str());
            mute.unlock();
        }
        
        private:
            switchdir(void);
            switchdir(const switchdir&);
            switchdir(switchdir&&);
            switchdir &operator=(const switchdir&);
            switchdir &operator=(switchdir&&);
            static std::mutex mute; /// declaration but not definition
            mutable std::string olddir;
            mutable std::string newdir;
    };
    
}; /* namespace filesystem */

#endif /// LIBIMREAD_EXT_FILESYSTEM_PATH_H_
