#pragma once

#include <string>
#include <vector>

#include <pqxx/row>
#include <pqxx/transaction>

#include "column_info_t.h"

std::basic_string_view< std::byte > getZipString(const std::string& event);
void executeOneNote(const pqxx::row& row, pqxx::work& newWorker);
