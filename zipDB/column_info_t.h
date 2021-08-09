#ifndef COLUMN_INFO_T_H
#define COLUMN_INFO_T_H

#include <string>
#include <vector>
#include <pqxx/row>
#include <pqxx/blob>

struct column_info_t
{
    column_info_t(const pqxx::row& row, const std::basic_string_view< std::byte >& zip_event_info);
    std::string type;
    std::string subjects;
    std::string timestamp;
    std::basic_string_view< std::byte > zip_event;
    std::string ts_vector;
};

#endif
