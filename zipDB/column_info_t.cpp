#include "column_info_t.h"

#include <string>
#include <vector>
#include <pqxx/row>

#include "tools.h"

column_info_t::column_info_t(const pqxx::row& row, const std::basic_string_view< std::byte >& zip_event_info):
    type(row.at("type").as< std::string >()),
    subjects(row.at("subjects").as< std::string >()),
    timestamp(row.at("timestamp").as< std::string >()),
    zip_event(zip_event_info),
    ts_vector(row.at("ts_vector").as< std::string >())
{
}
