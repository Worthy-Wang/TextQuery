class WordQuery:public Query_base
{
    string query_word;

private:
    WordQuery(const string &str) : query_word(str) {}
    QueryResult eval(const TextQuery& tq) { return tq.query(query_word); }

};