#ifndef _QUERYRESULT_H_
#define _QUERYRESULT_H_
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <memory>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;
using std::ostream;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

namespace wd_query
{
using line_no = std::size_t;

class QueryResult
{
    string _sought;                   //需要查询的单词
    shared_ptr<set<line_no>> _lines;  //单词出现的行号集合
    shared_ptr<vector<string>> _file; //整个文件
public:
    QueryResult(const string &, shared_ptr<set<line_no>>, shared_ptr<vector<string>>);
    friend ostream &print(ostream &, const QueryResult&);
};

ostream &print(ostream &os, const QueryResult& qr)
{
    os << qr._sought << " occurs " << qr._lines->size() << " times " << endl;
    for (auto& e: *qr._lines)
        cout << "(line " << e+1 << ") " << qr._file->at(e) << endl;
    cout << endl;
    return os;
}

QueryResult::QueryResult(const string &sought, shared_ptr<set<line_no>> lines, shared_ptr<vector<string>> file)
    : _sought(sought), _lines(lines), _file(file)
{
}

} //end of namespace wd_query

#endif