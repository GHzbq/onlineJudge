#include "httplib.h"

void test()
{
    httplib::Server server;
    server.Get("/", [](const httplib::Request& req, httplib::Response& resp){
                (void)req;
                std::string str = "<html>hello world</html>";
                resp.set_content(str, "text/html");
               });
    server.listen("0.0.0.0", 9092);
}



int main(void)
{
    return 0;
}
