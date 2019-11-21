#include "cursor.hpp"
#include <iostream>

using std::cout;

namespace pqpool
{

Cursor::Cursor(std::shared_ptr<Connection> conn, std::string const &query, int fetchCount )
    : Cursor( conn, query, {} , fetchCount )
{ }

Cursor::Cursor(std::shared_ptr<Connection> conn, std::string const &query, ListOfParams const &params, int fetchCount) 
                    : m_conn{conn},m_fetchCount{std::to_string(fetchCount)}
{
    open(query, params);
} 

Cursor::Cursor(Cursor &&  other )
    :m_conn {std::move(other.m_conn) }, m_result{  other.m_result.release() }, m_rowCount{other.m_rowCount}
{}


Cursor::Cursor(Cursor &  other )
        : m_conn {std::move(other.m_conn) },
          m_result{  other.m_result.release() },
          m_rowCount{other.m_rowCount}
{}


Cursor::~Cursor()
{
    close();
} 

bool Cursor::ok() const
{
    return  m_result->ok() ;
}

std::string Cursor::status() const 
{
    return m_result->status();
}

std::string Cursor::message() const
{
    return m_result->message();
}


bool Cursor::next()
{
    if( m_result->next() )
        return true;

    m_result = m_conn->query( "FETCH " + m_fetchCount +" in " + m_name  );
    return m_result->next();
}

std::string Cursor::get(std::string const &name)
{
    return m_result->get( name );
}

void Cursor::open( std::string const& query, ListOfParams const& params )
{
    m_name = CURSOR_NAME + std::to_string( ++m_count);
    m_result = m_conn->query("BEGIN");

    m_result = m_conn->query("DECLARE "+ m_name  + " CURSOR FOR " + query, params ) ;
    m_rowCount = 0;
    if( m_result->ok() )
    {
        auto res = m_conn->query( "MOVE FORWARD ALL IN "+ m_name  );
        m_rowCount = res->affectedRowCount();
        m_conn->query( "MOVE ABSOLUTE 0 IN " + m_name );
    }
}

void Cursor::close()
{
    if( closed == false )
    {
        m_conn->query( "CLOSE " + m_name +"; END" );
        closed = true;
    }
}

int Cursor::rowCount()
{
    return m_rowCount;
}

void  Cursor::moveFirst()
{
    m_conn->query( "MOVE FIRST IN " + m_name );
}

void  Cursor::moveLast()
{
    m_conn->query( "MOVE LAST IN " + m_name );
}

void  Cursor::moveTo( int row )
{
    m_conn->query( "MOVE ABSOLUTE "+ std::to_string( row ) +" IN " + m_name );
}


std::atomic<int> Cursor::m_count{0} ;

} // namespace pqpool