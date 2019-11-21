#include "connection.hpp"
#include <regex>

namespace pqpool
{

Connection::Connection(std::string const& connString )
{
    m_pgconn = PQconnectdb( connString.c_str() );
}

Connection::Connection( std::string const &host, std::string const &dbname, std::string const &user, std::string const &password, int port)
{
    m_pgconn = PQsetdbLogin(host.c_str(),
                            port > 0 ? std::to_string(port ).c_str() : nullptr ,
                            nullptr, 
                            nullptr, 
                            dbname.c_str(), 
                            user.c_str(), 
                            password.c_str() );
}

Connection::Connection(Connection &&o)
{
    m_pgconn = o.m_pgconn;
    o.m_pgconn = nullptr;
}

Connection::~Connection()
{
    close();
}


bool Connection::ok() const 
{
    return PQstatus(m_pgconn) == CONNECTION_OK;
}


const char* Connection::message() const 
{
    return PQerrorMessage( m_pgconn );
}



int Connection::close()
{
    if (!closed)
    {
        PQfinish(m_pgconn);
        closed = true;
    }
}

void Connection::reset()
{
    PQreset(m_pgconn);
}

ResultPointer Connection::query(std::string const &query)
{
    auto res = PQexec(m_pgconn, query.c_str());
    return std::make_unique<Result>( res );

}

ResultPointer Connection::query(std::string const &query, ListOfParams const &params)
{
    int nbParams = params.size();
    const char *paramValues[nbParams];
    int i = 0;
    for (auto &&param : params)
        paramValues[i++] = param.c_str();

    auto res = PQexecParams(m_pgconn,
                            query.c_str(),
                            nbParams,
                            nullptr /*paramsTypes array of 0: text, 1 : binary */,
                            paramValues,
                            nullptr /*paramsLength*/,
                            nullptr /*paramsFormats 0: text, 1 : binary*/,
                            0 /*resultFormat*/
    );
    return std::make_unique<Result>(res );
}


ResultPointer Connection::prepare(std::string const &name, std::string const& query  )
{
    auto res  = PQprepare( m_pgconn, name.c_str(), query.c_str(), _countParams( query ) , nullptr );
    return std::make_unique<Result>( res );
}


ResultPointer Connection::execPrepared(std::string const &name, ListOfParams const& params )
{
   const char* paramValues[ params.size() ];
   int i = 0;
   for( auto const& p : params )
        paramValues[i++] = p.c_str();

    auto res =  PQexecPrepared(m_pgconn, 
                    name.c_str(), 
                    params.size(),
                    paramValues,
                    nullptr, //const int *paramLengths,
                    nullptr, // int *paramFormats,
                    0 ); //int resultFormat) ;
    return std::make_unique<Result>( res );
}




std::string Connection::escapeString( const std::string str ) const 
{
    auto escaped = PQescapeLiteral(m_pgconn,  str.c_str(), str.size() );
    std::string ret { escaped };
    PQfreemem( escaped );
    return ret ;
}


int Connection::_countParams( std::string const& query )
{
    const std::regex reg("\\$[1-9]+");
    return std::distance( std::sregex_iterator(query.begin(), query.end(), reg), std::sregex_iterator() );
}

// std::string Connection::_getConnectionString()
// {
//     std::string conninfo = "host=" + (m_host.size() ? m_host : "''") +
//                            " dbname=" + (m_dbname.size() ? m_dbname : "''") +
//                            " user=" + (m_user.size() ? m_user : "''") +
//                            " password=" + (m_password.size() ? m_password : "''");
//     if (m_port > 0)
//         conninfo += " port=" + std::to_string(m_port);
//     return conninfo;
// }

// std::string Connection::_getConnectionUri()
// {
//     std::string connUri = "postgresql://" + m_user + (m_password.size() ? ":" + m_password : "") +
//                           "@" + m_host + (m_port > 0 ? ":" + std::to_string(m_port) : "") +
//                           "/" + m_dbname;
//     return connUri;
// }

// void Connection::startAsyn(std::function<void(Connection& conn )> onConnect)
// {
//     m_pgconn = PQconnectStart( _getConnectionString().c_str() );

//     while( true )
//     {
//         auto status = PQconnectPoll( m_pgconn );
//         if( status == PGRES_POLLING_FAILED || status == PGRES_POLLING_OK )
//         {
//             onConnect( *this );
//             break;
//         }
//     }

// }

} // namespace pqpool