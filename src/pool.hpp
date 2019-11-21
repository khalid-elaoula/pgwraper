#pragma once
#include <queue>
#include "connection.hpp"
#include <memory>
#include <mutex>
#include <condition_variable>
#include "settings.hpp"

namespace pqpool
{
class Pool
{
public:

    Pool(Settings const &conf);
    Pool(std::string const &conninfo, int max);
    std::shared_ptr<Connection> connection();
    void freeConnection( std::shared_ptr<Connection> conn );

private:
    std::queue<std::shared_ptr<Connection>> m_pool;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};

} // namespace pqpool