#pragma once
#include <google/template.h>
#include "oj_model.hpp"

class ojView
{
public:
    static void renderAllQuestions(const std::vector<Question> allQuestions, std::string* html)
    {
        // 1. 先创建一个ctemplate的对象，这是一个总的组织数据的对象
        // 2. 循环的往这个对象中添加一些子对象
        // 3. 每一个子对象再设置一些键值对（和模板里留下的{{}}是要对应的）
        // 4. 进行数据的替换，生成最终的html
        google::TemplateDictionary dict("all_questions");
        for(const auto& question : allQuestions)
        {
            google::TemplateDictionary* tableDict = 
                dict.AddSectionDictionary("question");
            tableDict->SetValue("id", question.id);
            tableDict->SetValue("name", question.name);
            tableDict->SetValue("star", question.star);
        }
        google::Template* tpl;
        tpl = google::Template::GetTemplate("./template/all_questions.html", google::DO_NOT_STRIP);
        tpl->Expand(html,&dict);

    }

    static void renderQuestion(const Question& question, std::string* html)
    {
        google::TemplateDictionary dict("question");
        
        dict.SetValue("id", question.id);
        dict.SetValue("name", question.name);
        dict.SetValue("star", question.star);
        dict.SetValue("desc", question.desc);
        dict.SetValue("header", question.header_cpp);
        
        google::Template* tpl;
        tpl = google::Template::GetTemplate("./template/question.html", google::DO_NOT_STRIP);
        tpl->Expand(html,&dict);
    }

    static void renderResult(const std::string& strStdout, const std::string& reason, 
                             std::string * html)
    {
        google::TemplateDictionary dict("result");
        
        dict.SetValue("stdout", strStdout);
        dict.SetValue("header", reason);
        
        google::Template* tpl;
        tpl = google::Template::GetTemplate("./template/result.html", google::DO_NOT_STRIP);
        tpl->Expand(html,&dict);
    }
};