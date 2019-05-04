/*
 * 文件工具类
 * */
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
    // 读取小文件 <64K
    class ReadSmallFile
    {
    public:
        // 64K
        static const int kBufferSize = FILE_UTIL_BUFF_SIZE;
    public:
        // ReadSmallFile类的构造函数
        // fileName 需要读取文件的文件名，以 const string 传参
        // 调用全局的系统调用 open 函数 以 "只读打开"
        // 初始化列表对错误码设置为0
        // 
        ReadSmallFile(const std::string fileName)
            : _fd(::open(fileName.c_str(), O_RDONLY | O_CLOEXEC))
            , _err(0)
        {
            // ReadSmallFile类自带一个大小为 64K 的 _buf;
            // 在构造函数中，我们将这段空间设置为空串
            _buf[0] = '\0';

            // 如果文件打开失败，open函数会返回-1 并设置对应的错误码
            if(_fd < 0)
            {
                _err = errno;
            }
        }

        // 这是 ReadSmallFile类的析构函数，对打开的文件描述符进行关闭
        ~ReadSmallFile()
        {
            // 如果_fd 大于 0 就将其关闭
            if(_fd >= 0)
            {
                ::close(_fd); // FIXME check EINTR
            }
        };

        // return errno
        // 模板函数 将文件读出，存储到 String 类型的空间里
        // maxSize: 输入型参数，指定这块 String 空间有多大
        // content: 输出型参数，不可以是空指针，用来存储文件内容
        // fileSize: 输出型参数，用来保存本文件的大小
        // modifyTime: 输出型参数，用来保存文件最后一次修改时间
        // createTime: 输出型参数，用来保存文件创建的时间
        template<typename String>
            int readToString(const int& maxSize, 
                             String  * content,
                             int64_t * fileSize,
                             int64_t * modifyTime,
                             int64_t * createTime)
            {
                // static_assert c++11 如果条件补满足，打印一条消息 
                static_assert(sizeof(off_t) == 8, "sizeof(off_t) not 8");
                // 断言一下存储文件内容的 content 是否为空
                assert(content != nullptr);
                // 将当前错误嘛拷贝给 err
                // 之后的操作，都是修改 err 
                int err = _err;

                // 只有当文件是 "打开" 的，才对文件进行读取
                if(_fd >= 0)
                {
                    // 在操作之前，先将空间清空
                    // 直接调用 clear() 函数即可
                    content->clear();

                    // 如果保存文件大小的输出型参数 fileSize 不为空，就获取文件大小
                    // 在 Linux 文件系统中没可以调用 fstat 函数，查看一个文件描述符对应文件的属性，
                    // 直接调用 stat 也是可以的，为什么选择 fstat 函数，看看这两函数原型就好了
                    //        int stat(const char *path, struct stat *buf);
                    //        int fstat(int fd, struct stat *buf);
                    // 我们没有保存文件路径，而 _fd 作为类的私有成员变量，可以直接拿到，
                    // 很方便的调用 fstat 函数
                    if(fileSize)
                    {
                        struct stat statBuf;
                        if(::fstat(_fd, &statBuf) == 0)
                        {
                            // is it a regular file 普通文件？
                            if(S_ISREG(statBuf.st_mode))
                            {
                                // st_size: total size, in bytes 
                                *fileSize = statBuf.st_size;
                                // 将 content 重置大小
                                // 读取的最大字节数
                                // 如果 maxSize 小，就读取 maxSize 
                                // 如果该文件的大小 *fileSize < maxSize 那就只读取 *fileSize大小数据
                                content->reserve(static_cast<int>(std::min(static_cast<int64_t>(maxSize), *fileSize)));
                            }
                            // directory? 目录
                            else if (S_ISDIR(statBuf.st_mode))
                            {
                                err = EISDIR;
                            }

                            // 保存修改时间的 modifyTime 如果不为空
                            if(modifyTime)
                            {
                                // 设置修改时间
                                // st_mtime: time of last modification
                                *modifyTime = statBuf.st_mtime;
                            }

                            // time of last status change 上次文件状态改变时间？
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
                        // read系统调用，返回实际读到的字节数
                        // 如果 read 失败,read函数返回 -1 并设置 errno 
                        ssize_t n = ::read(_fd, _buf, toRead);
                        if(n > 0)
                        {
                            content->append(_buf, n);
                        }
                        else
                        {
                            // read 失败
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


    private:
        int _fd;
        int _err;
        char _buf[kBufferSize];
    };

    template<typename String>
        int readFile(const std::string& fileName,
                     const int&  maxSize,
                     String  * content,
                     int64_t * fileSize = nullptr,
                     int64_t * modifyTime = nullptr,
                     int64_t * createTime = nullptr)
        {
            ReadSmallFile file(fileName);
            return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
        }

    template int readFile(const std::string& filename,
                          const int& maxSize,
                          std::string* content,
                          int64_t*, int64_t*, int64_t*);

    template int ReadSmallFile::readToString(
                                             const int& maxSize,
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
