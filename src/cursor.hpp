#pragma once
#include "connection.hpp"
#include <atomic>

namespace pqpool
{
#define CURSOR_NAME "_MYCURSOR_"

class Cursor
{

public:

    Cursor( std::shared_ptr<Connection> conn, std::string const& query, int fetchCount = 1000 );
    Cursor( std::shared_ptr<Connection> conn, std::string const& query, ListOfParams const& params , int fetchCount = 1000  );
    Cursor(Cursor &&  other );
    Cursor(Cursor& other ); 
    ~Cursor();
    bool ok() const ;
    std::string status() const;
    std::string message() const;
    bool next();
    std::string get(std::string const& name );
    void close();
    int rowCount();
    void moveFirst();
    void moveLast();
    void moveTo( int row );


private:
    std::shared_ptr<Connection> m_conn;
    std::string  m_fetchCount;
    std::unique_ptr<Result> m_result;
    static std::atomic_int m_count;
    std::string m_name;
    int m_rowCount;
    
    void open( std::string const& query, ListOfParams const& params );
    bool closed = false;

};
} // namespace pqpool