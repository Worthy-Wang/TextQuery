#include "TextQuery.h"
#include "Query.h"
using namespace wd_query;

void runQueries(ifstream &ifs)
{
    TextQuery tq(ifs);
    while (1)
    {
        cout << "input the word you want to seek, or 'q' to quit:" << endl;
        string word;
        std::cin >> word;
        if (word == "q")
            break;
        else
        {
            QueryResult queryRet = tq.query(word);
            print(cout, queryRet);
        }
    }
}

int main()
{
    ifstream ifs("data.txt", std::ifstream::in);
    if (!ifs.good())
        cout << "ifstream open file ERROR" << endl;
    TextQuery tq(ifs);
    Query q("Daddy");
    QueryResult qr = q.eval(tq);
    print(cout, qr);
    // Query q = ~Query("Daddy");
    // QueryResult qr = q.eval(tq);
    // print(cout, qr);
    ifs.close();
    return 0;
}