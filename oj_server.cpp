#include "httplib.h"
#include <jsoncpp/json/json.h>
#include "util.hpp"
#include "compiler.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"

void test()
{
    using namespace httplib;
    httplib::Server server;
    ojModel model;
    model.load();
    server.Get("/all_questions", [&model](const httplib::Request& req,
                                    httplib::Response& resp){
               (void)req;
               std::vector<Question> all_questions;
               model.getAllQuestion(&all_questions);
               std::string html;
               ojView::renderAllQuestions(all_questions, &html);
               resp.set_content(html, "text/html");
               });
    // R"()"  c++11 原始字符串 -- 忽略字符串的转移字符
    // \d+    正则表达式 用一些特殊符号来表示字符串满足啥样条件
    server.Get(R"(/question/(\d+))", [&model](const httplib::Request& req, 
                               httplib::Response& resp){
               Question question;
               model.getQuestion(req.matches[1].str(), &question);
               std::string html;
               ojView::renderQuestion(question, &html);
               resp.set_content(html, "text.html");
               });
    server.Post(R"(/compile/(\d+))", [&model](const httplib::Request& req, 
                                              httplib::Response& resp){
                // 1. 先根据 id 获取到题目信息
                Question question;
                model.getQuestion(req.matches[1].str(), &question);

                // 2. 解析 body ，获取到用户提交的代码
                std::unordered_map<std::string, std::string> bodyKV;
                util::urlUtil::parseBody(req.body, &bodyKV);
                const std::string& userCode = bodyKV["code"];

                // 3. 构造 JSON 结构的参数
                Json::Value req_json;
                // 真实需要编译的代码，是用户提交的代码+题目的测试用例代码
                req_json["code"] = userCode + question.tail_cpp;
                Json::Value resp_json;

                // 4. 调用编译模块进行编译
                compiler::compilerAndRun(req_json, &resp_json);

                // 5. 根据编译结果构造最终的网页
                std::string html;
                ojView::renderResult(resp_json["stdout"].asString(), 
                                     resp_json["reason"].asString(), &html);
                resp.set_content(html, "text/html");
                });

    server.set_base_dir("./wwroot");
    server.listen("0.0.0.0", 9092);
}

int main()
{
    test();

    return 0;
}
