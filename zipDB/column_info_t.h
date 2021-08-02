#ifndef COLUMN_INFO_T_H
#define COLUMN_INFO_T_H

#include <string>
#include <vector>
#include <pqxx/row>
#include <pqxx/blob>

struct column_info_t
{
    column_info_t(const pqxx::row& row, const std::vector< uint8_t >& zip_event_info,
        const std::vector< uint8_t >& zip_ts_vector_info);
    std::string type;
    std::string subjects;
    std::string timestamp;
    std::basic_string< std::byte > zip_event;
    std::basic_string< std::byte > zip_ts_vector;
};

#endif
