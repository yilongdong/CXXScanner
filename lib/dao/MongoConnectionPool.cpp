#include <mongocxx/instance.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include "dao/MongoConnectionPool.h"
#include "utility/log.h"

CXXScanner::dao::ConnectionPool &CXXScanner::dao::ConnectionPool::instance() {
    static ConnectionPool connectionPool;
    return connectionPool;
}

CXXScanner::dao::ConnectionPool &CXXScanner::dao::ConnectionPool::username(std::string const& username) {
    m_username = username;
    return *this;
}

CXXScanner::dao::ConnectionPool &CXXScanner::dao::ConnectionPool::password(std::string const& password) {
    m_password = password;
    return *this;
}

CXXScanner::dao::ConnectionPool &CXXScanner::dao::ConnectionPool::host(std::array<uint8_t, 4> host) {
    std::copy(host.begin(), host.end(),m_host.begin());
    return *this;
}

CXXScanner::dao::ConnectionPool &CXXScanner::dao::ConnectionPool::port(uint16_t port) {
    m_port = port;
    return *this;
}

CXXScanner::dao::ConnectionPool &
CXXScanner::dao::ConnectionPool::auth_database(std::string const& auth_database) {
    m_auth_database = auth_database;
    return *this;
}

void CXXScanner::dao::ConnectionPool::connect() {
    if(hasConnected()) {
        return;
    };
    auto uri_str = fmt::format("mongodb://{username}:{password}@{host}:{port}/?authSource={auth_database}&minPoolSize={min}&maxPoolSize={max}",
                        fmt::arg("username", m_username),
                        fmt::arg("password", m_password),
                        fmt::arg("host", fmt::join(m_host, ".")),
                        fmt::arg("port", m_port),
                        fmt::arg("auth_database", m_auth_database),
                        fmt::arg("min", 3), fmt::arg("max",5));
    LOG_INFO("uri str = {}", uri_str);
//    mongocxx::uri uri{"mongodb://124.222.77.135:27017/?minPoolSize=3&maxPoolSize=3"};
    m_pPool = std::make_unique<mongocxx::pool>(mongocxx::uri{uri_str});

}

bool CXXScanner::dao::ConnectionPool::hasConnected() const {
    return m_pPool != nullptr;
}

mongocxx::pool::entry CXXScanner::dao::ConnectionPool::acquire() {
    if (!hasConnected()) {
        connect();
    }
    return m_pPool->acquire();
}

CXXScanner::dao::ConnectionPool::ConnectionPool() :
    m_username("mongodb"), m_host{127,0,0,1},
    m_port(27017),m_auth_database("admin") {
    static mongocxx::instance inst{};
}

CXXScanner::dao::ConnectionPool::~ConnectionPool() = default;

