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

//WordQuery
/*
在程序中Query_base类只是作为抽象类，作用只是定义智能指针的类型。
不论是哪一种查询方式，都要先通过生成WordQuery类并返回Query_base类的指针，
相当于此时此刻，Queyr类中指针指向的是WordQuery类
*/
void test1()
{
    ifstream ifs("data.txt", std::ifstream::in);
    if (!ifs.good())
        cout << "ifstream open file ERROR" << endl;
    TextQuery tq(ifs);
    Query q("Daddy");
    QueryResult qr = q.eval(tq);
    print(cout, qr);
    ifs.close();
}

/*
NotQuery：
NotQuery的创建必须在WordQuery之前，因为NotQuery的Query类（Query类的指针指向WordQuery类）
通过~运算符，返回一个NotQuery的类型给新的Query内部的指针
*/
void test2()
{
    ifstream ifs("data.txt", std::ifstream::in);
    if (!ifs.good())
        cout << "ifstream open file ERROR" << endl;
    TextQuery tq(ifs);
    Query q = ~Query("Daddy");
    QueryResult qr = q.eval(tq);
    print(cout, qr);
    ifs.close();
}

/*
AndQuery:
返回的Query类型，内部的_q指向的是AndQuery的类型，实质上根本区别这些类型的还是函数eval返回的QueryResult
对于 ~(Query("has")&Query("red"))) 语句做一下分析：
1.Query("has") 与 Query("red") 内部指针都绑定的是 WordQuery类型， 通过运算符& 生成 AndQuery类型(内部两个指针均绑定WordQuery类型)；
2.AndQuery类型再通过~运算符生成NotQuery类型（内部指针绑定AndQuery类型）
3.q内部绑定的也就是NotQuery类型，调用NotQuery的eval(),递归调用AndQuery的eval(),再分别递归调用WordQuery的eval()
总的来说，难点在于树结构的递归调用, 最为基础的WordQuery写出来之后，不断递归返回新的QueryResult类型
*/
void test3()
{
    ifstream ifs("data.txt", std::ifstream::in);
    if (!ifs.good())
        cout << "ifstream open file ERROR" << endl;
    TextQuery tq(ifs);
    Query q = ~(Query("has")& Query("red"));
    // Query q = Query("has")& Query("red");
    QueryResult qr = q.eval(tq);
    print(cout, qr);
    ifs.close();
}

/*
OrQuery
*/
void test4()
{
    ifstream ifs("data.txt", std::ifstream::in);
    if (!ifs.good())
        cout << "ifstream open file ERROR" << endl;
    TextQuery tq(ifs);
    Query q = Query("hair") | Query("Alice");
    QueryResult qr = q.eval(tq);
    print(cout, qr);
    ifs.close();
}


int main()
{
    test4();
    return 0;
}