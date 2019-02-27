//////////////////////
// FileName: tool.hpp
// Autor:    zbq
//////////////////////

#pragma once

#include <iostream>
#include <string>
#include <stdint.h>
#include <fstream>

#include <sys/time.h>

namespace  Tool
{
    class TimeUtil
    {
    public:
        static int64_t GetTimeStamp()
        {
            int64_t ret = 0;
            timeval st_time;
            gettimeofday(&st_time, nullptr/* 时区 */);
            ret = st_time.tv_sec;

            return ret;
        }

        static int64_t GetTimeStampMS()
        {
            int64_t ret = 0;
            timeval st_time;

            gettimeofday(&st_time, nullptr);
            ret = st_time.tv_sec * 1000 + st_time.tv_usec / 1000;

            return ret;
        }
    }; // end of TimeUtil
    
    enum logLevel
    {
        INFO,
        WARING,
        ERROR,
        FATAL,
    };

    inline std::ostream& Log(logLevel level, 
                             const std::string& fileName, const int lineNum)
    {
        std::string prefix = "[";
        if(level == INFO)
        {
            prefix += 'I';
        }
        else if (level == WARING)
        {
            prefix += 'W';
        }
        else if(level == ERROR)
        {
            prefix += 'E';
        }
        else
        {
            prefix += 'F';
        }

        prefix += std::to_string(TimeUtil::GetTimeStamp());
        prefix += ' ';
        prefix += fileName;
        prefix += ':';
        prefix += std::to_string(lineNum);
        prefix += ']';
        prefix += ' ';

        std::cout << prefix ;

        return std::cout;
    }

#define LOG(level) Log((level), __FILE__, __LINE__)

    class FileUtil
    {
    public:
        static bool Read(const std::string& filePath, 
                         std::string* content)
        {
            content->clear();
            std::ifstream file(filePath.c_str());
            
            if(!file.is_open())
            {
                return false;
            }

            std::string line;
            while(std::getline(file, line))
            {
                *content += line + '\n';
            }

            file.close();
            return true;
        }

        static bool Write(const std::string& filePath,
                          const std::string& content)
        {
            std::ofstream file(filePath.c_str());
            if(!file.is_open())
            {
                return false;
            }

            file.write(content.c_str(), content.size());

            return true;
        }
    };
}
