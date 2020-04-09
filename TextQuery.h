#ifndef _TEXTQUERY_H_
#define _TEXTQUERY_H_
#include "QueryResult.h"
#include <regex>

namespace wd_query
{

class TextQuery
{
    shared_ptr<vector<string>> _file;          //用于保存整个输入文件的拷贝，下标代表行号，一行一行进行存储
    map<string, shared_ptr<set<line_no>>> _wm; //用于保存每个单词出现的行号， set是升序排序

public:
    TextQuery(ifstream &ifs);
    QueryResult query(const string&) const ;
    // shared_ptr<vector<string>> getFile() { return _file;}
    // shared_ptr<set<line_no>> getLineSet(const string& word) { return _wm.at(word); }
};

QueryResult TextQuery::query(const string& sought) const 
{
    static shared_ptr<set<line_no>> noLine(new set<line_no>);
    auto it = _wm.find(sought);
    if (it != _wm.end())
        return QueryResult(sought, it->second, _file);
    return QueryResult(sought, noLine, _file);
}

TextQuery::TextQuery(ifstream &ifs) : _file(new vector<string>)
{
    string line;
    //分解每一行
    while (getline(ifs, line))
    {
        _file->push_back(line);
        string replacedLine = std::regex_replace(line, std::regex("[^a-zA-Z0-9]"), " ");
        istringstream iss(replacedLine);
        string word;
        line_no lineNumber = _file->size() - 1; //行号从0开始
        //分解每一行中的每一个单词
        while (iss >> word)
        {
            shared_ptr<set<line_no>>& it = _wm[word];
            if (!it.get())
                it.reset(new set<line_no>);
            it->insert(lineNumber);
        }
    }
}

} //end of namespace wd_query

#endif