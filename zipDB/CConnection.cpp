#include "CConnection.h"

#include <memory>

CConnection::CConnection(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password):
	m_connection("host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password)
{
}

void CConnection::make_prepared_query(const std::string& queryName, const std::string& query)
{
	m_connection.prepare(queryName, query);
}

std::unique_ptr< pqxx::work > CConnection::getWorker()
{
	return std::make_unique< pqxx::work >(m_connection);
}

std::shared_ptr< pqxx::work > getSharedWorker()
{
	return std::make_shared< pqxx::work >(m_connection);
}

