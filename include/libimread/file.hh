/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)
#ifndef LPC_FILE_H_INCLUDE_GUARD_WED_FEB__1_16_34_50_WET_2012
#define LPC_FILE_H_INCLUDE_GUARD_WED_FEB__1_16_34_50_WET_2012

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cerrno>
#include <sstream>
#include <cstring>
#include <string>

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/seekable.hh>

namespace im {
    
    bool file_exists(char *path);
    bool file_exists(const char *path);
    bool file_exists(std::string path);
    bool file_exists(const std::string &path);
    
    class fd_source_sink : public byte_source, public byte_sink {
        
        public:
            fd_source_sink() {}
            fd_source_sink(int fd)
                :fd_(fd)
                { }
            
            virtual ~fd_source_sink() {
                ::close(fd_);
            }
            
            virtual std::size_t read(byte *buffer, std::size_t n) {
                return ::read(fd_, buffer, n);
            }
            
            virtual bool can_seek() const { return true; }
            virtual std::size_t seek_absolute(std::size_t pos) { return ::lseek(fd_, pos, SEEK_SET); }
            virtual std::size_t seek_relative(int delta) { return ::lseek(fd_, delta, SEEK_CUR); }
            virtual std::size_t seek_end(int delta) { return ::lseek(fd_, delta, SEEK_END); }
            
            virtual std::size_t write(const void *buffer, std::size_t n) {
                return ::write(fd_, buffer, n);
            }
            
            virtual std::vector<byte> full_data() {
                std::size_t orig = this->seek_relative(0);
                
                struct stat info;
                int result = ::fstat(fd_, &info);
                if (result == -1) {
                    throw CannotReadError("ERROR:",
                        "fstat() returned -1", std::strerror(errno));
                }
                
                std::vector<byte> res(info.st_size * sizeof(byte));
                this->seek_absolute(0);
                this->read(&res[0], res.size());
                this->seek_absolute(orig);
                return res;
            }
            
            virtual int fd() { return fd_; }
            virtual void fd(int fd) { fd_ = fd; }
            
        private:
            int fd_;
    };
    
    enum class Mode {
        READ, WRITE
    };
    
    class file_source_sink : public fd_source_sink {
        private:
            std::unique_ptr<char[]> pth;
            Mode md;
            
            static constexpr int READ_FLAGS = O_RDONLY | O_NONBLOCK;
            static constexpr int WRITE_FLAGS = O_CREAT | O_WRONLY | O_TRUNC | O_EXLOCK | O_SYMLINK;
            int open_read(char *p) { return ::open(p, READ_FLAGS); }
            int open_write(char *p, int m=0644) { return ::open(p, WRITE_FLAGS, m); }
        
        public:
            file_source_sink(Mode fmode = Mode::READ)
                :fd_source_sink(), md(fmode)
                {}
            
            file_source_sink(char *cpath, Mode fmode = Mode::READ)
                :fd_source_sink(), md(fmode)
                {
                    int _fd = -1;
                    if (md == Mode::READ) {
                        _fd = open_read(cpath);
                    } else if (md == Mode::WRITE) {
                        _fd = open_write(cpath);
                    }
                    if (_fd < 0) {
                        std::ostringstream out;
                        out << "ERROR: im::file_source_sink(): file read failure:\n"
                            << "\t::open(\"" << cpath 
                            << "\"\n," << ((md == Mode::READ) ? "O_RDONLY | O_NONBLOCK"
                                                              : "O_CREAT | O_WRONLY | O_TRUNC | O_EXLOCK | O_SYMLINK")
                            << ") returned negative: " << _fd << "\n"
                            << "ERROR MESSAGE: " << std::strerror(errno) << "\n";
                        throw CannotReadError(out.str());
                    }
                    this->fd(_fd);
                    pth = std::make_unique<char[]>(std::strlen(cpath)+1);
                    std::strcpy(pth.get(), cpath);
                }
            
            file_source_sink(const char *ccpath, Mode fmode = Mode::READ)
                :file_source_sink(const_cast<char *>(ccpath), fmode)
                {}
            
            file_source_sink(std::string &spath, Mode fmode = Mode::READ)
                :file_source_sink(spath.c_str(), fmode)
                {}
            
            file_source_sink(const std::string &cspath, Mode fmode = Mode::READ)
                :file_source_sink(cspath.c_str(), fmode)
                {}
            
            char *path() const { return pth.get(); }
            bool exists() const;
            
            Mode mode() { return md; }
            void mode(Mode m) { md = m; }
    };
    
    class FileSource : public file_source_sink {
        public:
            FileSource()
                :file_source_sink()
                {}
            FileSource(char *cpath)
                :file_source_sink(cpath)
                {}
            FileSource(const char *ccpath)
                :file_source_sink(ccpath)
                {}
            FileSource(std::string &spath)
                :file_source_sink(spath)
                {}
            FileSource(const std::string &cspath)
                :file_source_sink(cspath)
                {}
    };
    
    class FileSink : public file_source_sink {
        public:
            FileSink()
                :file_source_sink(Mode::WRITE)
                {}
            FileSink(char *cpath)
                :file_source_sink(cpath, Mode::WRITE)
                {}
            FileSink(const char *ccpath)
                :file_source_sink(ccpath, Mode::WRITE)
                {}
            FileSink(std::string &spath)
                :file_source_sink(spath, Mode::WRITE)
                {}
            FileSink(const std::string &cspath)
                :file_source_sink(cspath, Mode::WRITE)
                {}
    };
    

}

#endif // LPC_FILE_H_INCLUDE_GUARD_WED_FEB__1_16_34_50_WET_2012
