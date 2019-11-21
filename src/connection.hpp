#pragma once

#include <string>
#include <postgresql/libpq-fe.h>
#include "result.hpp"
#include <iostream>
#include <vector>
#include <memory>

namespace pqpool
{
using ResultPointer = std::unique_ptr<Result>;
using ListOfParams = std::initializer_list<const std::string>;

class Connection
{

public:
    Connection();
    Connection(std::string const& connString );
    Connection(std::string const &host, std::string const &dbname, std::string const &user, std::string const &password = "", int port = -1);
    Connection(Connection &&other);
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;
    ~Connection();

    int close();
    void reset();
    bool ok() const ;
    const char* message() const ;
    std::string escapeString( const std::string str ) const ;

    ResultPointer query(std::string const &query);
    ResultPointer query(std::string const &query, ListOfParams const &params );
    ResultPointer prepare(std::string const &name, std::string const& query );
    ResultPointer execPrepared(std::string const &name, ListOfParams const& params  = {} );

private:
    PGconn *m_pgconn;
    bool closed = false;

    int _countParams( std::string const& query );
};

} // namespace pqpool