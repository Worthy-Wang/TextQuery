#ifndef _QUERY_H_
#define _QUERY_H_
#include "TextQuery.h"

namespace wd_query
{

/*******************Query_base基类****************/
class Query_base
{
    friend class Query;

public:
    virtual ~Query_base() = default;

private:
    virtual QueryResult eval(const TextQuery &) const = 0;
    virtual string rep() const = 0;
};

/****************接口类Query 用于隐藏Query_base等基类以及派生类****************/
class Query
{
    shared_ptr<Query_base> _q;

public:
    //赋值_q
    Query(const string &);
    QueryResult eval(const TextQuery &tq) const { return _q->eval(tq); }
    string rep() const { return _q->rep(); }

private:
    Query(shared_ptr<Query_base> q)
        : _q(q)
    {
    }
};

/*************************Query_base派生类************************/

class WordQuery : public Query_base
{
    string query_word;
public:
    WordQuery(const string &str) : query_word(str) {}
private:
    QueryResult eval(const TextQuery &tq) const { return tq.query(query_word); }
    string rep() const { return query_word; }
};

inline Query::Query(const string &str)
    : _q(new WordQuery(str))
{
}


class NotQuery : public Query_base
{
};

} //end of namespace wd_query

#endif