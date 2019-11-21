#pragma once
#include <memory>
#include "pool.hpp"
#include "cursor.hpp"


namespace pqpool
{

class Transaction
{

public:
    Transaction( std::shared_ptr<Pool> pool, AutoCommit autoCommit );
    ~Transaction();

    bool ok() const ;
    std::string escapeString( std::string const& str ) const ;
    ResultPointer query(std::string const &query);
    ResultPointer query(std::string const &query, ListOfParams const &params );
    ResultPointer prepare(std::string const &name, std::string const& query );
    ResultPointer execPrepared(std::string const &name, ListOfParams const& params  = {} );
    ResultPointer commit();
    ResultPointer rollback();

    template<int fetchCount = 1000>
    std::shared_ptr<Cursor> cursor( std::string const& query)
    {
        return std::make_shared<Cursor>( m_conn, query, fetchCount  );
    }

    template<int fetchCount = 1000>
    std::shared_ptr<Cursor> cursor( std::string const& query, ListOfParams const& params  )
    {
        return std::make_shared<Cursor>( m_conn, query, params,  fetchCount  );
    }

private:
    std::shared_ptr<Pool> m_pool;
    std::shared_ptr<Connection> m_conn;
    AutoCommit m_auto_commit ;
};

} // namespace pqpool