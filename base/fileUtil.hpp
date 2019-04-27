#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <inttypes.h>

#define FILE_UTIL_BUFF_SIZE 64 * 1024

namespace FileUtil
{
    class ReadSmallFile
    {
    public:
        ReadSmallFile(const std::string fileName)
            : _fd(::open(fileName.c_str(), O_RDONLY | O_CLOEXEC))
            , _err(0)
        {
            _buf[0] = '\0';
            if(_fd < 0)
            {
                _err = errno;
            }
        }

        ~ReadSmallFile()
        {
            if(_fd >= 0)
            {
                ::close(_fd); // FIXME check EINTR
            }
        };

        // return errno
        template<typename String>
            int readToString(int maxSize, 
                             String  * content,
                             int64_t * fileSize,
                             int64_t * modifyTime,
                             int64_t * createTime)
            {
                static_assert(sizeof(off_t) == 8, "sizeof(off_t) not 8");
                assert(content != nullptr);
                int err = _err;
                if(_fd >= 0)
                {
                    content->clear();

                    if(fileSize)
                    {
                        struct stat statBuf;
                        if(::fstat(_fd, &statBuf) == 0)
                        {
                            if(S_ISREG(statBuf.st_mode))
                            {
                                *fileSize = statBuf.st_size;
                                content->reserve(static_cast<int>(std::min(static_cast<int64_t>(maxSize), *fileSize)));
                            }
                            else if (S_ISDIR(statBuf.st_mode))
                            {
                                err = EISDIR;
                            }

                            if(modifyTime)
                            {
                                *modifyTime = statBuf.st_mtime;
                            }

                            if(createTime)
                            {
                                *createTime = statBuf.st_ctime;
                            }
                        }
                        else
                        {
                            err = errno;
                        }
                    }

                    while(content->size() < static_cast<size_t>(maxSize))
                    {
                        size_t toRead = std::min(static_cast<size_t>(maxSize)-content->size(), sizeof(_buf));
                        ssize_t n = ::read(_fd, _buf, toRead);
                        if(n > 0)
                        {
                            content->append(_buf, n);
                        }
                        else
                        {
                            if(n < 0)
                            {
                                err = errno;
                            }
                            break;
                        }
                    }
                }

                return err;
            }

        // Read at maxium kBufferSize into _buf
        // return errno
        int readToBuffer(int * size)
        {
            int err = _err;
            if(_fd >= 0)
            {
                ssize_t n = ::pread(_fd, _buf, sizeof(_buf)-1, 0);
                if(n >= 0)
                {
                    if(size)
                    {
                        *size = static_cast<int>(n);
                    }
                    _buf[n] = '\0';
                }
                else
                {
                    err = errno;
                }
            }
            return err;
        }

        const char* buffer() const
        {
            return _buf;
        }

        // 64K
        static const int kBufferSize = FILE_UTIL_BUFF_SIZE;

    private:
        int _fd;
        int _err;
        char _buf[kBufferSize];
    };

    template<typename String>
        int readFile(const std::string& fileName,
                     int maxSize,
                     String  * content,
                     int64_t * fileSize = nullptr,
                     int64_t * modifyTime = nullptr,
                     int64_t * createTime = nullptr)
        {
            ReadSmallFile file(fileName);
            return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
        }

    template int readFile(const std::string& filename,
                          int maxSize,
                          std::string* content,
                          int64_t*, int64_t*, int64_t*);

    template int ReadSmallFile::readToString(
                                             int maxSize,
                                             std::string* content,
                                             int64_t*, int64_t*, int64_t*);

    class AppendFile
    {
    public:
        explicit AppendFile(const std::string& fileName)
            : _fp(::fopen(fileName.c_str(), "ae"))
            , _writtenBytes(0)
        {
            assert(_fp);
            ::setbuffer(_fp, _buffer, sizeof(_buffer));
        }

        ~AppendFile()
        {
            ::fclose(_fp);
        }

        void append(const char* logLine, const size_t len)
        {
            ssize_t n = write(logLine, len);
            size_t  remain = len - n;
            while(remain > 0)
            {
                size_t x = write(logLine + n, remain);
                if(x == 0)
                {
                    int err = ferror(_fp);
                    if(err)
                    {
                        fprintf(stderr, "AppendFile::append() error \n");
                    }
                    break;
                }
                n += x;
                remain = len - n;
            }

            _writtenBytes += len;
        }

        void flush()
        {
            ::fflush(_fp);
        }

        size_t writtenBytes() const
        {
            return _writtenBytes;
        }

    public:
        size_t write(const char* logLine, size_t len)
        {
            return ::fwrite_unlocked(logLine, 1, len, _fp);
        }

    public:
        FILE* _fp;
        char _buffer[FILE_UTIL_BUFF_SIZE];
        size_t _writtenBytes;
    };
}
