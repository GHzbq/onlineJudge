cmake_minimum_required(VERSION 2.8)

project(ONLINEJUDED)

set(lib_name "pthread" "mysqlclient" "jsoncpp" "ctemplate")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall -O0 -Wno-unused-variable -pthread")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall ")

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
    /usr/lib64/mysql/
    /usr/lib
    /usr/lib64
    )

# 查找mysqlclient动态库
find_library(MYSQL_LIB libmysqlclient.so /usr/lib64/mysql/)
IF (NOT MYSQL_LIB)
    MESSAGE(FATAL_ERROR "mysqlclient not found.")
ENDIF (NOT MYSQL_LIB)

# 查找

set(serverSrc 
    ../base/readConfig.hpp
    ../base/Singleton.h
    ../base/fileUtil.hpp
    ../base/logFile.hpp
    ../base/util.hpp 
    
    ../database/dataBaseMysql.hpp
    ../database/MySQLManger.hpp
    
    ../include/httplib.h
    ../serverSrc/compiler.hpp
    ../serverSrc/main.cpp)

add_executable(ojServer ${serverSrc})
TARGET_LINK_LIBRARIES(ojServer ${lib_name})
