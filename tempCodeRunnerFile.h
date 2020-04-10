inline Query operator&(const Query &lhs, const Query &rhs)
{
    return Query(shared_ptr<Query_base>(new AndQuery(lhs, rhs)));
}