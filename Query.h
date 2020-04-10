#ifndef _QUERY_H_
#define _QUERY_H_
#include "TextQuery.h"
#include <iterator>
#include <algorithm>

namespace wd_query
{

/*******************************Query_base基类************************************/
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
    friend Query operator~(const Query &q);
    friend Query operator&(const Query &lhs, const Query &rhs);
    friend Query operator|(const Query &lhs, const Query &rhs);

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

/************************************WordQuery***********************************/

class WordQuery : public Query_base
{
    string query_word;

public:
    WordQuery(const string &str) : query_word(str) {}

private:
    QueryResult eval(const TextQuery &tq) const { return tq.query(query_word); }
    string rep() const { return query_word; }
};

inline Query::Query(const string &str) : _q(new WordQuery(str)) {}

/********************************NotQuery******************************************/
class NotQuery : public Query_base
{
    Query _query;
    friend Query operator~(const Query &);

private:
    NotQuery(const Query &q) : _query(q) {}
    QueryResult eval(const TextQuery &tq) const;
    string rep() const { return "~(" + _query.rep() + ")"; }
};

inline Query operator~(const Query &q)
{
    return Query(shared_ptr<Query_base>(new NotQuery(q)));
}

QueryResult NotQuery::eval(const TextQuery &tq) const
{
    QueryResult qr = _query.eval(tq);
    shared_ptr<set<line_no>> lines = qr.getLines();
    shared_ptr<vector<string>> file = qr.getFile();
    shared_ptr<set<line_no>> newLine(new set<line_no>);

    for (line_no i = 0; i < file->size(); i++)
    {
        auto it = lines->find(i);
        if (it == lines->end())
            newLine->insert(i);
    }

    return QueryResult(this->rep(), newLine, file);
}

/********************************BinaryQuery抽象类******************************************/
class BinaryQuery : public Query_base
{
protected:
    Query _lhs;
    Query _rhs;
    string _opSym; //符号，& 或者 |
protected:
    BinaryQuery(const Query &lhs, const Query &rhs, string opSym)
        : _lhs(lhs), _rhs(rhs), _opSym(opSym)
    {
    }

    string rep() const { return "(" + _lhs.rep() + " " + _opSym + " " + _rhs.rep() + ") "; }
};

/********************************AndQuery******************************************/
class AndQuery : public BinaryQuery
{
    friend Query operator&(const Query &lhs, const Query &rhs);

private:
    AndQuery(const Query &lhs, const Query &rhs)
        : BinaryQuery(lhs, rhs, "&")
    {
    }

    QueryResult eval(const TextQuery &tq) const;
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
    return Query(shared_ptr<Query_base>(new AndQuery(lhs, rhs)));
}

QueryResult AndQuery::eval(const TextQuery &tq) const
{
    QueryResult left_qr = _lhs.eval(tq);
    shared_ptr<set<line_no>> left_lines = left_qr.getLines();
    QueryResult right_qr = _rhs.eval(tq);
    shared_ptr<set<line_no>> right_lines = right_qr.getLines();

    shared_ptr<set<line_no>> newLine(new set<line_no>);
    std::set_intersection(left_lines->begin(), left_lines->end(), right_lines->begin(), right_lines->end(), std::inserter(*newLine, newLine->end()));
    return QueryResult(this->rep(), newLine, left_qr.getFile());
}

/********************************OrQuery******************************************/
class OrQuery : public BinaryQuery
{
    friend Query operator|(const Query &lhs, const Query &rhs);

private:
    OrQuery(const Query &lhs, const Query &rhs)
        : BinaryQuery(lhs, rhs, "|")
    {
    }

    QueryResult eval(const TextQuery &tq) const;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
    return Query(shared_ptr<Query_base>(new OrQuery(lhs, rhs)));
}

QueryResult OrQuery::eval(const TextQuery &tq) const
{
    QueryResult left_qr = _lhs.eval(tq);
    shared_ptr<set<line_no>> left_lines = left_qr.getLines();
    QueryResult right_qr = _rhs.eval(tq);
    shared_ptr<set<line_no>> right_lines = right_qr.getLines();

    shared_ptr<set<line_no>> newLine(new set<line_no>);
    std::set_union(left_lines->begin(), left_lines->end(), right_lines->begin(), right_lines->end(), std::inserter(*newLine, newLine->end()));
    return QueryResult(this->rep(), newLine, left_qr.getFile());
}

} //end of namespace wd_query

#endif