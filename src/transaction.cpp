#include "transaction.hpp"
#include <cstdio>

namespace pqpool
{

Transaction::Transaction(std::shared_ptr<Pool> pool, AutoCommit autoCommit  )
    : m_pool{pool}, m_conn{pool->connection()}, m_auto_commit{ autoCommit }
{
    if( autoCommit == AutoCommit::NON )
        m_conn->query("BEGIN" );
}

Transaction::~Transaction()
{
    m_conn->query("DEALLOCATE ALL"); // free all prepared stmt in this connxion
    m_pool->freeConnection(m_conn);
}

bool Transaction::ok() const 
{
    return m_conn->ok();
}

ResultPointer Transaction::query(std::string const &query)
{
    return m_conn->query(query);
}

ResultPointer Transaction::query(std::string const &query, ListOfParams const &params)
{
    return m_conn->query(query, params);
}

ResultPointer Transaction::prepare(std::string const &name, std::string const &query)
{
    return m_conn->prepare(name, query);
}

ResultPointer Transaction::execPrepared(std::string const &name, ListOfParams const &params  )
{
    return m_conn->execPrepared(name, params);
}

ResultPointer Transaction::commit()
{
    return m_conn->query("COMMIT");
}


ResultPointer Transaction::rollback()
{
    return m_conn->query("ROLLBACK");
}


std::string Transaction::escapeString( std::string const& str ) const 
{
    return m_conn->escapeString( str );
}


} // namespace pqpool