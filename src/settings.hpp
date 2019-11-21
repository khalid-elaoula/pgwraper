#pragma once
#include <string>

namespace pqpool
{
struct Settings
{
    std::string host;
    std::string dbname;
    std::string user;
    std::string password;
    int port = -1;
    int maxConn;
};
enum AutoCommit {YES, NON };
} // namespace pqpool
