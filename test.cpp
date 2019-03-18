#include <iostream>
#include <string>
#include <unordered_map>
#include <jsoncpp/json/json.h>
#include "httplib.h"
#include "compiler.hpp"

void test1()
{
    using namespace httplib;
    Server server;
    server.Post("/compile", [](const Request& req, Response& resp)
               {
               // (void) req;
               // resp.set_content("<html>hello<html>", "text/html");
                    // (void)req;
               // 
                    std::unordered_map<std::string, std::string> bodyKV;
                    util::urlUtil::parseBody(req.body, &bodyKV);
                    Json::Value req_json;
                    for(const auto & e: bodyKV)
                    {
                        req_json[e.first] = e.second;
                    }
                    Json::Value resp_json;
                    compiler::compilerAndRun(req_json, &resp_json);
                    Json::FastWriter wrt;
                    resp.set_content(wrt.write(resp_json), "text/plain");
               });
    server.set_base_dir("./wwroot");
    server.listen("0.0.0.0", 9092);
}

void test2()
{
    std::string code;
    util::fileUtil::read("./text", &code);
    std::cout << "text:[ " << code << " ]" << std::endl;
    std::string str = "#include <stdio.h> \n int main(void){printf(\"hehe\"); return 0}";
    util::fileUtil::write("./text", str);
    util::fileUtil::read("./text", &code);
    std::cout << "text:[ " << code << " ]" << std::endl;
}

int main(void)
{
    test1();
    return 0;
}
