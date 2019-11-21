#include <iostream>
#include <thread>
#include "../../src/pqpool.hpp"


std::string conninfo = "dbname=rh user=rh";

auto pool = pqpool::instance( conninfo , 10 ); // 10 connexion max ;


void do_some_work_on_db()
{

    auto transaction = pool->transaction();

    auto result = transaction->query( "Select * from employee  where salary > $1 limit 10", {"200"});

    if( result->ok())
    {
        while (result->next() )     
        {
             std::cout << result->get("name") <<", " << result->get("address") <<'\n';
        }
    }
    else 
    {
        std::cout << result->message();
    }
}


int main(int argc, char *argv[])
{
    std::thread thlist[10];
    for( int  i=0;i < 10; ++i )
    {
        thlist[i] = std::thread( do_some_work_on_db );
    }

    for( int  i=0;i < 10; ++i )
    {
        thlist[i].join();
    }

    return 0;
}