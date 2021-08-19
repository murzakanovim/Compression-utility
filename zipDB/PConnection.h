#pragma once

#include <string>

#include <pqxx/connection>
#include <pqxx/transaction>

class PConnection
{
public:
	PConnection(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password);
	pqxx::work getWorker();
private:
	pqxx::connection m_connection;
};
