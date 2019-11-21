#include "pooler.hpp"

namespace pqpool
{

Pooler::Pooler( std::string conninfo, int maxConn )
         : m_pool{std::make_shared<Pool>( conninfo, maxConn) }
{ }

Pooler::Pooler( Settings const& settings  )
    : m_pool{std::make_shared<Pool>( settings ) }
{ }
std::unique_ptr<Pooler> Pooler::data_base_instance ;

Pooler* instance(std::string const &conninfo, int maxConn) 
{
    if( Pooler::data_base_instance == nullptr )
    {
        Pooler::data_base_instance.reset(  new Pooler( conninfo, maxConn ) ) ; 
    }
    return Pooler::data_base_instance.get();
}

Pooler* instance(Settings const& settings )
{
    if( Pooler::data_base_instance == nullptr )
    {
        Pooler::data_base_instance.reset(  new Pooler( settings) ) ; 
    }
    return Pooler::data_base_instance.get();
}

std::shared_ptr<Transaction>  Pooler::transaction(AutoCommit autoCommit )
{
    return std::make_shared<Transaction>( m_pool, autoCommit );
}




} // namespace pqpool
