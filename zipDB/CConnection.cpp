#include "CConnection.h"

CConnection::CConnection(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password):
	m_connection("host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password)
{
}

pqxx::work CConnection::getWorker()
{
	return pqxx::work(m_connection);
}

void CConnection::make_prepared(const std::string& name, const std::string& query)
{
	m_connection.prepare(name, query);
}
