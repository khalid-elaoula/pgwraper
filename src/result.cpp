#include "result.hpp"
#include <iostream>
using std::cout;

namespace pqpool
{

Result::Result(PGresult *pg_result) : m_pgresult{pg_result}
{
}

Result::Result(Result &&other) : m_pgresult{other.m_pgresult}
{
    other.m_pgresult = nullptr;
}
Result::~Result()
{
    PQclear(m_pgresult);
}

bool Result::ok () const
{
    switch (PQresultStatus(m_pgresult) )
    {
        case PGRES_COMMAND_OK: 
        case PGRES_TUPLES_OK :
        case PGRES_SINGLE_TUPLE: 
        case PGRES_NONFATAL_ERROR:  return true;
    }
    return false;
}

const char* Result::status () const
{
    return PQresStatus(PQresultStatus( m_pgresult ));
}

const char* Result::message () const
{
    return  PQresultErrorMessage( m_pgresult);
}

int Result::rowCount() const {
    return PQntuples( m_pgresult );
}

int Result::fieldCount() const {
    return PQnfields( m_pgresult );
}

std::string Result::fieldName( int columnNumber ) const 
{
    return std::string( PQfname( m_pgresult, columnNumber ) );
}

int Result::fieldNumber( std::string const& columnName ) const 
{
    return PQfnumber( m_pgresult, columnName.c_str() );
}

bool Result::next()
{
    return ( ++m_currentRowIndex ) <  rowCount();
}

void Result::reset()
{
    m_currentRowIndex = -1;
}

bool Result::moveToRow( int rowIndex )
{
    if( rowIndex < 0 || rowIndex >= rowCount() )
        return false;
    m_currentRowIndex = rowIndex;
    return true;
}

std::string Result::get(std::string fieldName )
{
    auto num  = fieldNumber( fieldName );
    return num >=0 ? get( num  ) : "" ;
}

std::string Result::get(int fieldNumber )
{
    int rownIndex = m_currentRowIndex >= 0  ?  m_currentRowIndex : 0;
    return _get( rownIndex , fieldNumber );
}


int  Result::affectedRowCount()
{
    auto count_str = PQcmdTuples(m_pgresult );
    if( count_str == nullptr )
        return 0 ;
    return std::atoi( count_str );
}

std::string Result::_get( int rowNumber, int fieldNumber)
{
    return std::string( PQgetvalue( m_pgresult, rowNumber, fieldNumber ) );
}



} // namespace pqpool
