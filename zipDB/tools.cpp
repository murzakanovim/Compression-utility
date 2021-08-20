#include "tools.h"

#include <string>
#include <pqxx/pqxx>

#include "compress.h"
#include "CConnection.h"

void executeOneNote(const pqxx::row& row, std::unique_ptr< pqxx::work > zipWorker)
{
    auto zipString = getZipString(row.at("event").as< std::string >());
    zipWorker->exec_prepared("insert", row.at("type").as< std::string >(), row.at("subjects").as< std::string >(), row.at("timestamp").as< std::string >(), zipString, row.at("ts_vector").as< std::string >());
}

std::basic_string_view< std::byte > getZipString(const std::string& event)
{
    return pqxx::binary_cast(NConsulUtils::zip(event.data(), event.size()));
}
