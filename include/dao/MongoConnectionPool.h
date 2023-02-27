#pragma once
#include <string>
#include <array>
#include <memory>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>

namespace CXXScanner::dao {
    class ConnectionPool final {
    public:
        static ConnectionPool& instance();
        ConnectionPool& username(std::string const & username);
        ConnectionPool& password(std::string const & password);
        ConnectionPool& host(std::array<uint8_t, 4> host);
        ConnectionPool& port(uint16_t port);
        ConnectionPool& auth_database(std::string const & auth_database);
        void connect();
        [[nodiscard]] bool hasConnected() const;
        mongocxx::pool::entry acquire();

    private:
        ConnectionPool();
        ~ConnectionPool();
    public:
        ConnectionPool(ConnectionPool const&) = delete;
        ConnectionPool(ConnectionPool const&&) = delete;
        ConnectionPool const& operator=(ConnectionPool const&) = delete;
        ConnectionPool const& operator=(ConnectionPool &&) = delete;
    private:
        std::string m_username;
        std::string m_password;
        std::array<uint8_t, 4> m_host;
        uint16_t m_port;
        std::string m_auth_database;
        std::unique_ptr<mongocxx::pool> m_pPool;
    };
}
