#include <iostream>
#include "compiler.hpp"

void test()
{
    Json::Value req;
    req["code"] = "#include <iostream> \n int main(void){std::cout << \"hehe\" << std::endl; return 0;}";
    req["stdin"] = "";
    Json::Value resp;
    Json::FastWriter reader ;
    std::cout << "req[ "<< reader.write(req) << " ]" << std::endl;
    compiler::compilerAndRun(req, &resp);
    Json::FastWriter wrt;
    std::cout << wrt.write(resp) << std::endl;
}

int main(void)
{
    test();

    return 0;
}
