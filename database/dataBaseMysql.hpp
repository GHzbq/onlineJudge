#pragma once 
#include <vector>
#include <string>
#include <map>
#include <stdint.h>
#include <cassert>
#include <algorithm>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>
#include "../util.hpp"

namespace dataBaseMysql 
{
#define MAX_QUERY_LEN 1024

    inline void toLowerString(std::string& str)
    {
        for(size_t i = 0; i < str.size(); ++i)
        {
            if(str[i] >= 'A' && str[i] <= 'Z')
            {
                str[i] = str[i] + ('a' - 'A');
            }
        }
    }

    class Field
    {
    public: 
        enum DataTypes
        {
            DB_TYPE_UNKNOWN = 0x00,
            DB_TYPE_STRING  = 0x01,
            DB_TYPE_INTEGER = 0x02,
            DB_TYPE_FLOAT   = 0x03,
            DB_TYPE_BOOL    = 0x04
        };

        Field()
            : mType(DB_TYPE_UNKNOWN)
        {
            m_bNULL = false;
        }

        Field(Field& f)
        {
            m_strValue = f.m_strValue;
        }

        Field(const char* value, enum DataTypes type)
            : mType(type)
        {
            m_strValue = value;
        }

        ~Field()
        {}

        enum DataTypes getType() const
        {
            return mType;
        }

        const std::string getString() const
        {
            return m_strValue;
        }

        std::string getCppString() const
        {
            return m_strValue;
        }

        float getFloat() const
        {
            return static_cast<float>(atof(m_strValue.c_str()));
        }

        bool getBool() const
        {
            return atoi(m_strValue.c_str()) > 0;
        }

        int32_t getInt32() const
        {
            return static_cast<int32_t>(atol(m_strValue.c_str()));
        }

        uint8_t getUInt8() const
        {
            return static_cast<uint8_t>(atol(m_strValue.c_str()));
        }

        uint16_t getUInt16() const
        {
            return static_cast<uint16_t>(atol(m_strValue.c_str()));
        }

        int16_t getInt16() const
        {
            return static_cast<int16_t>(atol(m_strValue.c_str()));
        }

        uint32_t getUInt32() const
        {
            return static_cast<uint32_t>(atol(m_strValue.c_str()));
        }

        uint64_t getUInt64() const
        {
            uint64_t value = 0;
            value = atoll(m_strValue.c_str());
            return value;
        }

        void setType(enum DataTypes type)
        {
            mType = type;
        }

        void setValue(const char* value, size_t len)
        {
            m_strValue.assign(value, len);
        }

        void setName(const std::string& strName)
        {
            m_strFieldName = strName;
            toLowerString(m_strFieldName);
        }

        const std::string& getName()
        {
            return m_strFieldName;
        }

        bool isNULL()
        {
            return m_bNULL;
        }

        template <typename T>
            void conertValue(T& value);
    private:
        std::string m_strValue;
        std::string m_strFieldName;
        enum DataTypes mType;
    public:
        bool m_bNULL;

    };

    class QueryResult
    {
    public:
        typedef std::map<uint32_t, std::string> fieldNames;
    public:
        QueryResult(MYSQL_RES* result, uint64_t rowCount, uint32_t fieldCount)
            : mFieldCount(fieldCount)
            , mRowCount(rowCount)
        {
            mResult = result;
            mCurrentRow = new Field[mFieldCount];
            assert(mCurrentRow);

            MYSQL_FIELD* fields = mysql_fetch_fields(mResult);
            
            for(uint32_t i = 0; i < mFieldCount; ++i)
            {
                if(fields[i].name != nullptr)
                {
                    mFieldNames[i] = fields[i].name;
                    m_vtFieldNames.push_back(fields[i].name);
                }
                else
                {
                    mFieldNames[i] = "";
                    m_vtFieldNames.push_back("");
                }

                mCurrentRow[i].setType(convertNativeType(fields[i].type));
            }
        }
        
        virtual ~QueryResult()
        {
            endQuery();
        }

        virtual bool nextRow()
        {
            MYSQL_ROW row;

            if(!mResult)
                return false;

            row = mysql_fetch_row(mResult);
            if(!row)
            {
                endQuery();
                return false;
            }

            unsigned long int * ulFieldLen = nullptr;
            ulFieldLen = mysql_fetch_lengths(mResult);
            for(uint32_t i = 0; i < mFieldCount; ++i)
            {
                if(row[i] == nullptr)
                {
                    mCurrentRow[i].m_bNULL = true;
                    mCurrentRow[i].setValue("", 0);
                }
                else
                {
                    mCurrentRow[i].m_bNULL = false;
                    mCurrentRow[i].setValue(row[i], ulFieldLen[i]);
                }

                mCurrentRow[i].setName(mFieldNames[i]);
            }

            return true;
        }

        uint32_t getFieldIndex(const std::string& name) const
        {
            for(fieldNames::const_iterator it = getFieldNames().begin(); it != getFieldNames().end(); ++it)
            {
                if(it->second == name)
                    return it->first;
            }

            assert(false && "unknow field name");
            return uint32_t(-1);
        }

        Field* fetch() const
        {
            return mCurrentRow;
        }

        const Field& operator[](int index) const
        {
            return mCurrentRow[index];
        }

        const Field& operator[](const std::string& name) const
        {
            return mCurrentRow[getFieldIndex(name)];
        }

        uint32_t getFieldCount() const
        {
            return mFieldCount;
        }

        uint32_t getRowCount() const
        {
            return mRowCount;
        }

        fieldNames const& getFieldNames() const
        {
            return mFieldNames;
        }

        std::vector<std::string> const& getNames() const
        {
            return m_vtFieldNames;
        }
    private:
        enum Field::DataTypes convertNativeType(enum_field_types mysqlType) const
        {
            switch(mysqlType)
            {
            case FIELD_TYPE_TIMESTAMP:
            case FIELD_TYPE_DATE:
            case FIELD_TYPE_TIME:
            case FIELD_TYPE_DATETIME:
            case FIELD_TYPE_YEAR:
            case FIELD_TYPE_STRING:
            case FIELD_TYPE_VAR_STRING:
            case FIELD_TYPE_BLOB:
            case FIELD_TYPE_SET:
            case FIELD_TYPE_NULL:
                return Field::DB_TYPE_STRING;
            case FIELD_TYPE_TINY:
            case FIELD_TYPE_SHORT:
            case FIELD_TYPE_LONG:
            case FIELD_TYPE_INT24:
            case FIELD_TYPE_LONGLONG:
            case FIELD_TYPE_ENUM:
                return Field::DB_TYPE_INTEGER;
            case FIELD_TYPE_DECIMAL:
            case FIELD_TYPE_FLOAT:
            case FIELD_TYPE_DOUBLE:
                return Field::DB_TYPE_FLOAT;
            default:
                return Field::DB_TYPE_UNKNOWN;
            }
        }
    public:
        void endQuery()
        {
            if(mCurrentRow)
            {
                delete[] mCurrentRow;
                mCurrentRow = nullptr;
            }

            if(mResult)
            {
                mysql_free_result(mResult);
                mResult = nullptr;
            }
        }
    protected:
        Field*                   mCurrentRow;
        uint32_t                 mFieldCount;
        uint64_t                 mRowCount;
        fieldNames               mFieldNames;
        std::vector<std::string> m_vtFieldNames;

        MYSQL_RES*               mResult;
    };

    class CDatabaseMysql
    {
    public:
        struct DatabaseInfo
        {
            std::string _strHost;
            std::string _strUser;
            std::string _strPwd;
            std::string _strDBName;
        };
    public:
        CDatabaseMysql()
            : m_Mysql(nullptr)
            , m_bInit(false)
        {}
    
        ~CDatabaseMysql()
        {
            if(m_Mysql != nullptr)
            {
                if(m_bInit)
                {
                    mysql_close(m_Mysql);
                }
            }
        }
    
    
        bool initialize(const std::string& host,
                         const std::string& user,
                         const std::string& pwd,
                         const std::string& dbname)
        {
            if(m_bInit)
            {
                mysql_close(m_Mysql);
            }

            m_Mysql = mysql_init(m_Mysql);
            m_Mysql = mysql_real_connect(m_Mysql, host.c_str(), user.c_str(), 
                                         pwd.c_str(), dbname.c_str(), 0, nullptr, 0);
            m_DBInfo._strHost = host;
            m_DBInfo._strUser = user;
            m_DBInfo._strPwd  = pwd;
            m_DBInfo._strDBName = dbname;

            if(m_Mysql)
            {
                mysql_query(m_Mysql, "set names utf8");
                m_bInit = true;
                return true;
            }
            else
            {
                LOG(util::ERROR) << "Could not connect to MYSQL database: " << mysql_error(m_Mysql) << std::endl;
                mysql_close(m_Mysql);
                return false;
            }
            LOG(util::ERROR) << "CDatabaseMysql::initialize, init false" << std::endl;
            return false;
        }

        QueryResult* query(const char* sql)
        {
            if(!m_Mysql)
            {
                LOG(util::INFO) << "mysql is disconnected!" << std::endl; 
                if(false == initialize(m_DBInfo._strHost, m_DBInfo._strUser, 
                                       m_DBInfo._strPwd, m_DBInfo._strDBName))
                {
                    return nullptr;
                }                   
            }

            if(!m_Mysql)
                return 0;

            MYSQL_RES* result = nullptr;
            uint64_t rowCount = 0;
            uint32_t fieldCount = 0;
            {
                LOG(util::INFO) << sql << std::endl;
                int iTempRet = mysql_real_query(m_Mysql, sql, strlen(sql));
                if(iTempRet)
                {
                    unsigned int uErrno = mysql_errno(m_Mysql);
                    LOG(util::ERROR) << "mysql is abnormal, errno : " << uErrno << std::endl;
                    if(CR_SERVER_GONE_ERROR == uErrno)
                    {
                        LOG(util::ERROR) << "mysql is disconnected!" << std::endl; 
                        if(false == initialize(m_DBInfo._strHost, m_DBInfo._strUser,
                                               m_DBInfo._strPwd, m_DBInfo._strDBName))
                        {
                            return nullptr;
                        }
                        LOG(util::INFO) << sql << std::endl;
                        iTempRet = mysql_real_query(m_Mysql, sql, strlen(sql));
                        if(iTempRet)
                        {
                            LOG(util::ERROR) << "SQL:" << sql << std::endl;
                            LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                        }
                    }
                    else
                    {
                        LOG(util::ERROR) << "SQL: " << sql << std::endl;
                        LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                        return nullptr;
                    }
                }

                result = mysql_store_result(m_Mysql);
                rowCount = mysql_affected_rows(m_Mysql);
                fieldCount = mysql_field_count(m_Mysql);              
            }
            if(!result)
            {
                return nullptr;
            }

            QueryResult* queryResult = new QueryResult(result, rowCount, fieldCount);
            queryResult->nextRow();
            return queryResult;
        }

        QueryResult* query(const std::string& sql)
        {
            return query(sql.c_str());
        }

        QueryResult* pQuery(const char* format, ...)
        {
            if(!format)
            {
                return nullptr;
            }

            va_list ap;
            char szQuery[MAX_QUERY_LEN];
            va_start(ap, format);
            int res = vsnprintf(szQuery, MAX_QUERY_LEN, format, ap);
            va_end(ap);

            if (res == -1)
            {
                LOG(util::ERROR) << "SQL Query truncated (and not execute) for format: " << format << std::endl;
                return nullptr;
            }

            return query(szQuery);
        }

        bool execute(const char* sql)
        {
            if(!m_Mysql)
                return false;

            {
                int iTempRet = mysql_query(m_Mysql, sql);
                if(iTempRet)
                {
                    unsigned int uErrno = mysql_errno(m_Mysql);
                    LOG(util::ERROR) << "mysql is abnormal, errno : " << uErrno << std::endl;
                    if(CR_SERVER_GONE_ERROR == uErrno)
                    {
                        LOG(util::ERROR) << "mysql is disconnected! << std::endl";
                        if(false == initialize(m_DBInfo._strHost, m_DBInfo._strUser,
                                               m_DBInfo._strPwd, m_DBInfo._strDBName))
                        {
                            return false;
                        }
                        LOG(util::ERROR) << sql  << std::endl;
                        iTempRet = mysql_real_query(m_Mysql, sql, strlen(sql));
                        if(iTempRet)
                        {
                            LOG(util::ERROR) << "SQL: "<< sql << std::endl;
                            LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                        }
                    }
                    else
                    {
                        LOG(util::ERROR) << "SQL: " << sql << std::endl;
                        LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                    }

                    return false;
                }
            }
            return true;
        }

        bool execute(const char* sql, uint32_t& uAffectedCount, int& nErrno)
        {
            if(!m_Mysql)
                return false;

            {
                int iTempRet = mysql_query(m_Mysql, sql);
                if(iTempRet)
                {
                    unsigned int uErrno = mysql_errno(m_Mysql);
                    LOG(util::ERROR) << "mysql is abnormal, errno : " << uErrno << std::endl;
                    if(uErrno == CR_SERVER_GONE_ERROR)
                    {
                        LOG(util::ERROR) << "mysql is disconnection!" << std::endl;
                        if(false == initialize(m_DBInfo._strHost, m_DBInfo._strUser, 
                                               m_DBInfo._strPwd, m_DBInfo._strDBName))
                        {
                            return false;
                        }
                        LOG(util::ERROR) << sql << std::endl;
                        iTempRet = mysql_query(m_Mysql, sql);
                        nErrno = iTempRet;
                        if(iTempRet)
                        {
                            LOG(util::ERROR) << "SQL: " << sql << std::endl;
                            LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                        }
                    }
                    else
                    {
                        LOG(util::ERROR) << "SQL: " << sql << std::endl;
                        LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                    }
                    return false;
                }
                uAffectedCount = static_cast<uint32_t>(mysql_affected_rows(m_Mysql));
            }
            return true;
        }

        bool pExecute(const char* format, ...)
        {
            if(!format)
            {
                return false;
            }

            va_list ap;
            char szQuery[MAX_QUERY_LEN] = {0};
            va_start(ap, format);
            int res = vsnprintf(szQuery, MAX_QUERY_LEN, format, ap);
            va_end(ap);

            if(res == -1)
            {
                LOG(util::ERROR) << "SQL query truncated (and not execute) for format: " << format << std::endl;
                return false;
            }

            if(!m_Mysql)
                return false;

            {
                int iTempRet = mysql_query(m_Mysql, szQuery);
                if(iTempRet)
                {
                    unsigned int uErrno = mysql_errno(m_Mysql);
                    LOG(util::ERROR) << "mysql is abnormal, errno : " << uErrno << std::endl;
                    if(CR_SERVER_GONE_ERROR == uErrno)
                    {
                        LOG(util::ERROR) << "mysql is disconnected" << std::endl;
                        if(false == initialize(m_DBInfo._strHost, m_DBInfo._strUser,
                                               m_DBInfo._strPwd, m_DBInfo._strDBName))
                        {
                            return false;
                        }
                        LOG(util::ERROR) << szQuery << std::endl;
                        iTempRet = mysql_query(m_Mysql, szQuery);
                        if(iTempRet)
                        {
                            LOG(util::ERROR) << "SQL: " << szQuery << std::endl;
                            LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                        }
                    }
                    else
                    {
                        LOG(util::ERROR) << "SQL: " << szQuery << std::endl;
                        LOG(util::ERROR) << "query ERROR: " << mysql_error(m_Mysql) << std::endl;
                    }
                    return false;
                }
            }

            return true;
        }

        uint32_t getInsertID()
        {
            return (uint32_t)mysql_insert_id(m_Mysql);
        }

        void clearStoreResults()
        {
            if(!m_Mysql)
            {
                return;
            }

            MYSQL_RES* result = nullptr;
            while(!mysql_next_result(m_Mysql))
            {
                if((result = mysql_store_result(m_Mysql)) != nullptr)
                {
                    mysql_free_result(result);
                }
            }
        }

        int32_t escapeString(char* szDst, const char* szSrc, uint32_t uSize)
        {
            if(m_Mysql == nullptr)
            {
                return 0;
            }

            if(szDst == nullptr || szSrc == nullptr)
            {
                return 0;
            }

            return mysql_real_escape_string(m_Mysql, szDst, szSrc, uSize);
        }
        
    private:
        DatabaseInfo m_DBInfo;
        MYSQL       *m_Mysql;   
        bool         m_bInit;
    };
}
