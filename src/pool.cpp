#include "pool.hpp"


namespace pqpool
{

Pool::Pool(Settings const &conf)
{
    for( int i=0; i < conf.maxConn; ++i )
    {
        m_pool.emplace( std::make_shared<Connection> (conf.host, conf.dbname,conf.user, conf.password, conf.port ) );
    }
}

Pool::Pool(std::string const &conninfo, int max)
{
    for( int i=0; i < max ; ++i )
    {
        m_pool.emplace( std::make_shared<Connection>( conninfo) );
    }
}


std::shared_ptr<Connection> Pool::connection()
{
    std::unique_lock<std::mutex> lock_( m_mutex );
    while( m_pool.empty() )
        m_condition.wait(lock_);

    auto conn  = m_pool.front();
    m_pool.pop();
    return conn;
}

void Pool::freeConnection( std::shared_ptr<Connection> conn )
{
    std::unique_lock<std::mutex> lock_(m_mutex);
    m_pool.push( conn );
    lock_.unlock();
    m_condition.notify_one();
}



} // namespace pqpool
