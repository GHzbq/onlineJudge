#pragma once

#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <memory>

class ReadConfig
{
public:
    ReadConfig(const char* fileName)
    {
        loadFile(fileName);
    }

    ~ReadConfig()
    {}

    char* getConfigName(const char* name)
    {
        if(!_isReadOk)
        {
            return nullptr;
        }

        char* value = nullptr;
        // 此处完全可以用 auto 代替
        // 但为了更加直观，我们直接用的是 全称
        std::map<std::string, std::string>::iterator it = _config_map.find(name);
        if(it != _config_map.end())
        {
            value = (char*)it->second.c_str();
        }
        return value;
    }

    int setConfigValue(const char* name, const char* value)
    {
        if(!_isReadOk)
        {
            return -1;
        }

        std::map<std::string, std::string>::iterator it = _config_map.find(name);
        if(it != _config_map.end())
        {
            it->second = value;
        }
        else
        {
            _config_map.insert(std::make_pair(name, value));
        }

        return writeFile();
    }
private:
    void loadFile(const char* fileName)
    {
        _config_map.clear();
        _config_file.append(fileName);
        FILE* fp = ::fopen(fileName, "r");
        if(!fp)
            return;
        char buf[256];
        for(;;)
        {
            // C库的fgets函数可以读取一行
            char* p = ::fgets(buf, 256, fp);
            if(!p)
                break;

            size_t len = ::strlen(buf);
            if(buf[len-1] == '\n')
                buf[len-1] = '\0'; // remove \n at the end

            // 我们约定配置文件的注释是 '#' 开头的字符串
            char* ch = strchr(buf, '#'); // remove string start with # 
            if(ch)
                *ch = '\0';

            // 如果这行是空行或者是被处理过的注释，直接跳过
            if(::strlen(buf) == 0)
                continue;

            parseLine(buf);
        }

        ::fclose(fp);
        _isReadOk = true;
    }

    int writeFile(const char* fileName = nullptr)
    {
        FILE* fp = nullptr;
        if(fileName == nullptr)
        {
            fp = ::fopen(_config_file.c_str(), "w");
        }
        else
        {
            fp = ::fopen(fileName, "w");
        }

        if(fp == nullptr)
        {
            return -1;
        }

        char szPair[128] = {0};
        std::map<std::string, std::string>::iterator it = _config_map.begin();
        for(; it != _config_map.end(); ++it)
        {
            memset(szPair, 0, sizeof(szPair));
            snprintf(szPair, sizeof(szPair), "%s=%s\n", it->first.c_str(), it->second.c_str());
            size_t ret = ::fwrite(szPair, ::strlen(szPair), 1, fp);
            if(ret != 1)
            {
                fclose(fp);
                return -1;
            }
        }
        fclose(fp);
        return 0;
    }

    void parseLine(char* line)
    {
        char* p = strchr(line, '=');
        if(p == nullptr)
            return;

        *p = '\0';
        char* key = trimSpace(line);
        char* value = trimSpace(p+1);
        if(key && value)
        {
            _config_map.insert(std::make_pair(key, value));
        }
    }

    char* trimSpace(char* name)
    {
        // remove starting space or tab 
        char* startPos = name;
        while((*startPos == ' ') || (*startPos) == '\t')
        {
            ++startPos;
        }

        // remove ending space or tab
        char* endPos = name + ::strlen(name) - 1;
        while((*endPos == ' ') || (*endPos == '\t'))
        {
            *endPos = '\0';
            --endPos;
        }

        int len = (int)(endPos-startPos) + 1;
        if(len <= 0)
        {
            return nullptr;
        }

        return startPos;
    }
private:
    bool                               _isReadOk;
    std::map<std::string, std::string> _config_map;
    std::string                        _config_file;
};  
