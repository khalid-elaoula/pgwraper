#pragma once
#include <string>
#include <postgresql/libpq-fe.h>


namespace pqpool
{
class Result
{

public:

    Result() = delete;
    Result(Result const& ) = delete;
    Result& operator=(Result const&) = delete;
    Result(PGresult* pg_result);
    Result( Result && other);
    ~Result();

    //Result Status functions
    bool ok() const ;
    const char* message() const ;
    const char* status() const ;

    // Retriving data functions
    int rowCount() const ;
    int fieldCount() const ;
    int fieldNumber( std::string const& fieldName ) const ;
    std::string fieldName( int fieldNumber ) const ;
    bool next();
    void reset();
    bool moveToRow( int rowIndex );
    std::string get(std::string fieldName );
    std::string get(int fieldNumber );
    int affectedRowCount();
    

private:
    PGresult* m_pgresult;
    int m_currentRowIndex = -1;
    std::string _get( int rowNumber, int fieldNumber);
};
} // namespace pqpool
