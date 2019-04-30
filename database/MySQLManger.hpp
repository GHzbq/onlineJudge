/*
 * 核心类：mysqlManager
 * */
#pragma once

#include <memory>
#include <map>
#include <vector>
#include <sstream>

#include "./dataBaseMysql.hpp"
#include "../base/Singleton.h"

#define MAX_CMD_LEN 8192

struct STableField
{
public:
    STableField()
    {}

    STableField(std::string strName, std::string strType, std::string strIndex)
        : _strName(strName)
        , _strType(strType)
        , _strIndex(strIndex)
    {}

public:
    std::string _strName;
    std::string _strType;
    std::string _strIndex;
};

struct STableInfo
{
public:
    STableInfo()
    {}

    std::string strEngineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8 comment '设置存储引擎为MyISAM 字符集为utf8'";

    STableInfo(std::string strName)
        : _strName(strName)
    {}

public:
    std::string _strName;
    std::map<std::string, STableField> _mapFiled;
    std::string _strKeyString;
    std::string _strEndineAndCharSet;
};

class mysqlManager
{
public:
    mysqlManager(const std::string& charactSet = "utf8")
        : _strCharactSet(charactSet)
    {
        // 初始化表
        // 1. 编译信息表
        {
            STableInfo info;

            info._strName = "compileinfo";
            info._mapFiled["solution_id"] = { "solution_id", "int(11) NOT NULL DEFAULT 0 comment '题目ID'", "int(11)"};
            info._mapFiled["error"] = {"error", "text comment '错误信息'", "text"};

            info._strKeyString = "PAIMARY KEY (solution_id) comment '将 solution_id 设置为主键'";
            info._strEndineAndCharSet = "ENGIE=MyISAM DEFAULT CHARSET=utf8 comment '设置存储引擎为 MyISAM 字符集为 utf8'";

            _vecTableInfo.push_back(info);
        }

        // 2. 登录信息表
        {
            STableInfo info;
            info._strName = "loginlog";

            info._mapFiled["user_id"] = {"user_id", "varchar(48) NOT NULL DEFAULT '' comment '用户提交账号'", "varchar(48)"};
            info._mapFiled["password"] = {"password", "varchar(40) DEFAULT NULL comment '用户提交的密码'", "varchar(48)"};
            info._mapFiled["ip"] = {"ip", "varchar(46) DEFAULT NULL comment '本次提交用户的IP地址'", "varchar(46)"};
            info._mapFiled["time"] = {"time", "datetime DEFAULT NULL comment '提交的时间'", "datetime"};

            info._strKeyString = "KEY `user_log_index` (`user_id`,`time`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);
        }

        // 3. OJ题目信息表
        {
            STableInfo info;
            info._strName = "problem";

            info._mapFiled["problem_id"] = {"problem_id", "int(11) NOT NULL AUTO_INCREMENT comment '题目ID 自增长'", "int(11)"};
            info._mapFiled["title"] = {"title", "varchar(200) NOT NULL DEFAULT '' comment '题目标题'", "varchar(200)"};
            info._mapFiled["description"] = {"description", "text comment '题目的描述'", "text"};
            info._mapFiled["input"] = {"input", "text comment '输入描述'", "text"};
            info._mapFiled["output"] = {"output", "text comment '输出描述'", "text"};
            info._mapFiled["sample_input"] = {"sample_input", "text comment '样例输入'", "text"};
            info._mapFiled["sample_output"] = {"sample_output", "text comment '样例输出'", "text"};
            info._mapFiled["spj"] = {"spj", "char(1) NOT NULL DEFAULT '0'", "char(1)"};
            info._mapFiled["hint"] = {"hint", "text", "text"};
            info._mapFiled["source"] = {"source", "varchar(100) DEFAULT NULL", "varchar(100)"};
            info._mapFiled["in_date"] = {"in_date", "datetime DEFAULT NULL", "datetime"};
            info._mapFiled["time_limit"] = {"time_limit", "int(11) NOT NULL DEFAULT 0 comment '本题的时间限制'", "int(11)"};
            info._mapFiled["memory_limit"] = {"memory_limit", "int(11) NOT NULL DEFAULT 0 comment '本题的内存限制'", "int(11)"};
            info._mapFiled["defunct"] = {"defunct", "char(1) NOT NULL DEFAULT 'N'", "char(1)"};
            info._mapFiled["accepted"] = {"accepted", "int(11) DEFAULT '0' comment '通过人数'", "int(11)"};
            info._mapFiled["submit"] = {"submit", "int(11) DEFAULT '0' comment '提交人数'", "int(11)"};
            info._mapFiled["solved"] = {"solved", "int(11) DEFAULT '0' comment '解决人数'", "int(11)"};
            
            info._strKeyString = "PRIMARY KEY (`problem_id`) comment  '设置 题目ID 为主键'";
            info._strEndineAndCharSet = "ENGINE=MyISAM AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 comment '设置存储引擎为MyISAM,字符集默认 utf8 , 自增长起始值为1000'";

            _vecTableInfo.push_back(info);
        }

        // 4. solution 表
        {
            STableInfo info;

            info._strName = "solution";

            info._mapFiled["solution_id"] = {"solution_id", "int(11) NOT NULL AUTO_INCREMENT", "int(11)"};
            info._mapFiled["problem_id"] = {"problem_id", "int(11) NOT NULL DEFAULT 0", "int(11)"};
            info._mapFiled["user_id"] = {"user_id", "char(48) NOT NULL", "char(48)"};
            info._mapFiled["time"] = {"time", "int(11) NOT NULL DEFAULT 0", "int(11)"};
            info._mapFiled["memory"] = {"memory", "int(11) NOT NULL DEFAULT 0", "int(11)"};
            info._mapFiled["in_date"] = {"in_date", "datetime NOT NULL DEFAULT '2016-05-13 19:24:00'", "datetime"};
            info._mapFiled["result"] = {"result", "smallint(6) NOT NULL DEFAULT '0'", "smallint(6)"};
            info._mapFiled["language"] = {"language", "INT UNSIGNED NOT NULL DEFAULT '0'", "INT UNSIGNED"};
            info._mapFiled["ip"] = {"ip", "char(46) NOT NULL", "char(46)"};
            info._mapFiled["contest_id"] = {"contest_id", "int(11) DEFAULT NULL", "int(11)"};
            info._mapFiled["valid"] = {"valid", "tinyint(4) NOT NULL DEFAULT '1'", "tinyint(4)"};
            info._mapFiled["num"] = {"num", "tinyint(4) NOT NULL DEFAULT '-1'", "tinyint(4)"};
            info._mapFiled["code_length"] = {"code_length", "int(11) NOT NULL DEFAULT 0", "int(11)"};
            info._mapFiled["judgetime"] = {"judgetime", "timestamp NULL DEFAULT CURRENT_TIMESTAMP", "timestamp"};
            info._mapFiled["pass_rate"] = {"pass_rate", "DECIMAL(3,2) UNSIGNED NOT NULL DEFAULT 0", "DECIMAL(3,2) UNSIGNED"};
            info._mapFiled["lint_error"] = {"lint_error", "int UNSIGNED NOT NULL DEFAULT 0", "int UNSIGNED"};
            info._mapFiled["judger"] = {"judger", "CHAR(16) NOT NULL DEFAULT 'LOCAL'", "CHAR(16)"};

            info._strKeyString = "PRIMARY KEY (`solution_id`), KEY `uid` (`user_id`), KEY `pid` (`problem_id`), KEY `res` (`result`), KEY `cid` (`contest_id`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM row_format=fixed AUTO_INCREMENT=1001 DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);

        }

        // source_code 表
        {
            STableInfo info;
            info._strName = "source_code";

            info._mapFiled["solution_id"] = {"solution_id", "int(11) NOT NULL", "int(11)"};
            info._mapFiled["source"] = {"source", "text NOT NULL", "text"};

            info._strKeyString = "PRIMARY KEY (`solution_id`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);
        }

        // source_code_user 表
        {
            STableInfo info;
            info._strName = "source_code_user";

            info._mapFiled["solution_id"] = {"solution_id", "int(11) NOT NULL", "int(11)"};
            info._mapFiled["source"] = {"source", "text NOT NULL", "text"};

            info._strKeyString = "PRIMARY KEY (`solution_id`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);

        }

        // users 表
        {
            STableInfo info;
            info._strName = "users";

            info._mapFiled["user_id"] = {"user_id", "varchar(48) NOT NULL DEFAULT ''", "varchar(48)"};
            info._mapFiled["email"] = {"email", "varchar(100) DEFAULT NULL", "varchar(100)"};
            info._mapFiled["submit"] = {"submit", "int(11) DEFAULT '0'", "int(11)"};
            info._mapFiled["solved"] = {"solved", "int(11) DEFAULT '0'", "int(11)"};
            info._mapFiled["defunct"] = {"defunct", "char(1) NOT NULL DEFAULT 'N'", "char(1)"};
            info._mapFiled["ip"] = {"ip", "varchar(46) NOT NULL DEFAULT ''", "varchar(46)"};
            info._mapFiled["accesstime"] = {"accesstime", "datetime DEFAULT NULL", "datetime"};
            info._mapFiled["volume"] = {"volume", "int(11) NOT NULL DEFAULT '1'", "int(11)"};
            info._mapFiled["language"] = {"language", "int(11) NOT NULL DEFAULT '1'", "int(11)"};
            info._mapFiled["password"] = {"password", "varchar(32) DEFAULT NULL", "varchar(32)"};
            info._mapFiled["reg_time"] = {"reg_time", "datetime DEFAULT NULL", "datetime"};
            info._mapFiled["nick"] = {"nick", "varchar(20) NOT NULL DEFAULT ''", "varchar(20)"};
            info._mapFiled["school"] = {"school", "varchar(20) NOT NULL DEFAULT ''", "varchar(20)"};
            
            info._strKeyString = "PRIMARY KEY (`user_id`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);

        }

        // online 表
        {
            STableInfo info;
            info._strName = "online";

            info._mapFiled["hash"] = {"hash", "varchar(32) collate utf8_unicode_ci NOT NULL", "varchar(32)"};
            info._mapFiled["ip"] = {"ip", "varchar(46) character set utf8 NOT NULL default ''", "varchar(32)"};
            info._mapFiled["ua"] = {"ua", "varchar(255) character set utf8 NOT NULL default ''", "varchar(255)"};
            info._mapFiled["refer"] = {"refer", "varchar(255) collate utf8_unicode_ci default NULL", "varchar(255)"};
            info._mapFiled["lastmove"] = {"lastmove", "int(10) NOT NULL", "int(10)"};
            info._mapFiled["firsttime"] = {"firsttime", "int(10) default NULL", "int(10)"};
            info._mapFiled["uri"] = {"uri", "varchar(255) collate utf8_unicode_ci default NULL", "varchar(255)"};
            
            info._strKeyString = "PRIMARY KEY  (`hash`), UNIQUE KEY `hash` (`hash`)";
            info._strEndineAndCharSet = "ENGINE=MEMORY DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci";

            _vecTableInfo.push_back(info);
        }

        // runtimeinfo
        {
            STableInfo info;
            info._strName = "runtimeinfo";

            info._mapFiled["solution_id"] = {"solution_id", "int(11) NOT NULL DEFAULT 0", "int(11)"};
            info._mapFiled["error"] = {"error", "text", "text"};

            info._strKeyString = "PRIMARY KEY (`solution_id`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);

        }

        // custominput 表
        {
            STableInfo info;
            info._strName = "custominput";

            info._mapFiled["solution_id"] ={"solution_id", "int(11) NOT NULL DEFAULT 0", "int(11)"};
            info._mapFiled["input_text"] = {"input_text", "text", "text"};

            info._strKeyString = "PRIMARY KEY (`solution_id`)";
            info._strEndineAndCharSet = "ENGINE=MyISAM DEFAULT CHARSET=utf8";

            _vecTableInfo.push_back(info);

        }


    }

    virtual ~mysqlManager()
    {}
public:
    bool init(const char* host, const char* user, const char* password, const char* dbName)
    {

        _strHost = host;
        _strUser = user;
        // 数据库密码可能为空
        if(password != nullptr)
            _strPassword = password;
        _strDBName = dbName;

        _pConnect.reset(new dataBaseMysql::CDatabaseMysql());
        if(!_pConnect->initialize(_strHost, _strUser, _strPassword, _strDBName))
        {
            LOG(util::FATAL) << " init error" << std::endl;
            return false;
        }

        // 检查库是否存在
        if(!isDBExist())
        {
            if(!createDB())
            {
                return false;
            }
        }

        // 再次确定是否可以链接上数据库
        _pConnect.reset(new dataBaseMysql::CDatabaseMysql());
        if(!_pConnect->initialize(_strHost, _strUser, _strPassword, _strDBName) != 0)
        {
            LOG(util::FATAL) << "CDatabaseMysql init error" << std::endl;
            return false;
        }

        // 2. 检查库中表是否正确
        for(size_t i = 0; i < _vecTableInfo.size(); ++i)
        {
            STableInfo& table = _vecTableInfo[i];
            if(!checkTable(table))
            {
                LOG(util::FATAL) << "table check faild" << std::endl;
                return false;
            }
        }   

        _pConnect.reset();
        return true;
    }

    std::string getHost() const
    {
        return _strHost;
    }

    std::string getUser() const 
    {
        return _strUser;
    }

    std::string getPassword() const 
    {
        return _strPassword;
    }

    std::string getDBName() const 
    {
        return _strDBName;
    }

    std::string getCharSet() const 
    {
        return _strCharactSet;
    }

private:
    bool isDBExist()
    {
        if(nullptr == _pConnect)
        {
            return false;
        }

        dataBaseMysql::QueryResult* pResult = _pConnect->query("show databases;");
        if(nullptr == pResult)
        {
            LOG(util::INFO) << "isDBExist(), no database(" << _strDBName << ")" << std::endl;
            return false;
        }

        dataBaseMysql::Field* pRow = pResult->fetch();
        while(pRow != nullptr)
        {
            std::string name = pRow[0].getString();
            if(name == _strDBName)
            {
                LOG(util::INFO) << "isDBExist(), find database(" << _strDBName << ")";
                pResult->endQuery();
                return true;
            }

            if(pResult->nextRow() == false)
            {
                break;
            }
            pRow = pResult->fetch();
        }

        LOG(util::INFO) << "isDBExist(), no database(" << _strDBName << ")" << std::endl;
        pResult->endQuery();
        return false;
    }

    bool createDB()
    {
        if(nullptr == _pConnect)
        {
            return false;
        }

        uint32_t uAffectedCount = 0;
        int nErrno = 0;

        std::stringstream ss;
        ss << "create database " << _strDBName << ";";
        if(_pConnect->execute(ss.str().c_str(), uAffectedCount, nErrno))
        {
            if(uAffectedCount == 1)
            {
                LOG(util::INFO) << "createDB(), create database "
                    << _strDBName << " success" << std::endl;
                return true;
            }
        }
        else
        {
            LOG(util::ERROR) << "createDB(), create database " 
                << _strDBName << " failed, errno = " << nErrno << std::endl;
            return false;
        }

        return false;
    }

    bool checkTable(const STableInfo& tableInfo)
    {
        if(nullptr == _pConnect)
        {
            return false;
        }

        if(tableInfo._strName.find_first_not_of("\t\r\n ") == std::string::npos)
        {
            LOG(util::WARNING) << "checkTable(), table info invalid";
            return false;
        }

        std::stringstream ss;
        ss << "desc " << tableInfo._strName << ";";
        dataBaseMysql::QueryResult* pResult = _pConnect->query(ss.str());
        if(nullptr == pResult)
        {
            LOG(util::INFO) << "checkTable(), no table " << tableInfo._strName << " , begin create..." << std::endl;;
            if(createTable(tableInfo))
            {
                LOG(util::INFO) << "checkTable(), " << tableInfo._strName << " , end create..." << std::endl;
                return true;
            }
            return false;
        }
        else
        {
            std::map<std::string, std::string> mapOldTable;
            dataBaseMysql::Field* pRow = pResult->fetch();
            while(pRow != nullptr)
            {
                std::string name = pRow[0].getString();
                std::string type = pRow[1].getString();
                mapOldTable[name] = type;

                if(pResult->nextRow() == false)
                {
                    break;
                }

                pRow = pResult->fetch();
            }

            pResult->endQuery();
            for(std::map<std::string, STableField>::const_iterator it = tableInfo._mapFiled.begin();
                it != tableInfo._mapFiled.end(); ++it)
            {
                STableField field = it->second;
                if(mapOldTable.find(field._strName) == mapOldTable.end())
                {
                    std::stringstream ss;
                    ss << "alter table " << tableInfo._strName << " add column "
                        << field._strName << " " << field._strType << ";";

                    const std::string& sql = ss.str();
                    if(_pConnect->execute(sql.c_str()))
                    {
                        LOG(util::INFO) << sql << std::endl;
                        continue;
                    }
                    else 
                    {
                        LOG(util::ERROR) << "checkTable() faild : " << sql << std::endl;
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool createTable(const STableInfo& tableInfo)
    {
        if(tableInfo._mapFiled.size() == 0)
        {
            LOG(util::ERROR) << "createTable(), table info invalid, " << tableInfo._strName << std::endl;
            return false;
        }

        std::stringstream ss;
        ss << "CREATE TABLE IF NOT EXISTS " << tableInfo._strName << " (";

        for(std::map<std::string, STableField>::const_iterator it = tableInfo._mapFiled.begin();
            it != tableInfo._mapFiled.end(); ++it)
        {
            if(it != tableInfo._mapFiled.begin())
            {
                ss << ", ";
            }

            STableField field = it->second;
            ss << field._strName << " " << field._strType;
        }

        if(tableInfo._strKeyString != "")
        {
            ss << ", " << tableInfo._strKeyString ;
        }

        if(tableInfo._strEndineAndCharSet != "")
        {
            ss << ") " << tableInfo._strEndineAndCharSet << ";";
        }
        else
        {
            ss << ")default charset = utf8, ENGINE = InnoDB;";
        }

#if __DEBUG_ON__ 
        std::cout << "@@@ sql: " << ss.str() << std::endl;
#endif

        if(_pConnect->execute(ss.str().c_str()))
        {
            return true;
        }

        return false;
    }

    bool updateTable(const STableInfo& tableInfo);

protected:
    std::shared_ptr<dataBaseMysql::CDatabaseMysql> _pConnect;

    std::string _strHost;
    std::string _strUser;
    std::string _strPassword;
    std::string _strDBName;
    std::string _strCharactSet;

    std::vector<STableInfo> _vecTableInfo;
};
