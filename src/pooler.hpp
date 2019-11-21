#pragma once
#include "pool.hpp"
#include "transaction.hpp"
#include "settings.hpp"


namespace pqpool
{

class Pooler
{

public:
    Pooler() = delete;
    Pooler(Pooler const& ) = delete;
    Pooler& operator=(Pooler const& ) = delete;

    friend Pooler* instance( std::string const& conninfo, int maxConn );
    friend Pooler* instance(Settings const& settings );
    std::shared_ptr<Transaction> transaction(AutoCommit autoCommit = YES );

private:
    Pooler( std::string conninfo, int maxConn );
    Pooler( Settings const& settings  );
    std::shared_ptr<Pool>  m_pool;
    static std::unique_ptr<Pooler> data_base_instance;
};

Pooler* instance( std::string const& conninfo, int maxConn );
Pooler* instance(Settings const& settings );


} // namespace pqpool
