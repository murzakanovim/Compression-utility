#include <string>
#include <memory>


#include <pqxx/connection>
#include <pqxx/transaction>

class CConnection
{
public:
	CConnection(const std::string& host, const std::string& port, const std::string& dbname, const std::string& user, const std::string& password);
	void make_prepared_query(const std::string& queryName, const std::string& query);
	std::unique_ptr< pqxx::work > getUniqueWorker();
	std::shared_ptr< pqxx::work > getSharedWorker();
private:
	pqxx::connection m_connection;
};

