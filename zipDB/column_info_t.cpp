#include "column_info_t.h"

#include <string>
#include <vector>
#include <pqxx/row>

#include "tools.h"

column_info_t::column_info_t(const pqxx::row& row, const std::vector<uint8_t>& zip_event_info, const std::vector<uint8_t>& zip_ts_vector_info)
{
    type = row[0].c_str();
    subjects = row[1].c_str();
    timestamp = row[2].c_str();
    for (auto byte: zip_event_info)
    {
        zip_event += toBinary(byte);
    }

    for (auto byte : zip_ts_vector_info)
    {
        zip_ts_vector += toBinary(byte);
    }
}
