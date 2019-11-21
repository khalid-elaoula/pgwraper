#include <iostream>
#include "../../src/pqpool.hpp"



int main(int argc, char *argv[])
{
    std::string conninfo = "dbname=dbgwcl user=getigwcl";

    auto conn = std::make_shared<pqpool::Connection>( conninfo);
    
    auto result = conn->prepare("stmt1", "Select * from employee  where id = $1 ");

    if( !result->ok())
    {
        std::cout << result->message();
        return -1;
    }
    auto listUserId = {"1","2","3","4"};

    for( auto &id : listUserId )
    {
        result = conn->execPrepared("stmt1", {id } );
        if( result->ok() )
        {
            if( result->rowCount() > 0 )
            {
                // calling the result->get("") before calling result->next will always return the first element 
                std::cout << result->get("name") <<", " << result->get("salary") <<'\n';
            }
        }
        else
        {
            std::cout << result->message() <<'\n';
        }
    }

    return 0;
}