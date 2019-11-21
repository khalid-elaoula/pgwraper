#include <iostream>
#include "../../src/pqpool.hpp"



int main(int argc, char *argv[])
{
    std::string conninfo = "dbname=dbgwcl user=getigwcl";
    auto conn = std::make_shared<pqpool::Connection>( conninfo);
    
    auto result = conn->query("Select * from employee limit 10  ");
    if( result->ok() )
    {
        while( result->next())
        {
            std::cout << result->get("name") <<", " << result->get("salary") <<'\n';
        }
    }
    else
    {
        std::cout << result->message() <<'\n';
    }

    // query with argument
    result = conn->query("Select * from employee where dep = $1 and salary > $2 ", {"RH", "15000"});
    
    if( result->ok() )
    {
        std::cout <<"Row count : "<< result->rowCount() <<'\n';

        while( result->next())
        {
            std::cout << result->get("name") <<", " << result->get("address") <<'\n';
        }
    }
    else
    {
        std::cout << result->message() <<'\n';
    }

    return 0;
}