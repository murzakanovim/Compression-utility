#include "PConnection.h"

PConnection::PConnection(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password):
	m_connection("host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password)
{
}

pqxx::work PConnection::getWorker()
{
	return pqxx::work(m_connection);
}
