#include "tools.h"

#include <string>
#include <pqxx/pqxx>

#include "column_info_t.h"
#include "compress.h"
#include "PConnection.h"

void executeOneNote(const pqxx::row& row, pqxx::work& zipWorker)
{
    auto zipString = getZipString(row.at("event").as< std::string >());
    column_info_t column(row, zipString);//ненужно
    zipWorker.exec_prepared("insert", column.type, column.subjects, column.timestamp, column.zip_event, column.ts_vector);
}

std::basic_string_view< std::byte > getZipString(const std::string& event)
{
    return pqxx::binary_cast(NConsulUtils::zip(event.data(), event.size()));
}
