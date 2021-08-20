#include "CConnection.h"

#include <memory>

CConnection::CConnection(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password):
	m_connection("host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password)
{
}

std::unique_ptr< pqxx::work > CConnection::getWorker()
{
	return std::make_unique< pqxx::work >(m_connection);
}
