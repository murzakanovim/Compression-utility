#pragma once

#include <string>
#include <vector>

#include <pqxx/row>
#include <pqxx/transaction>

std::basic_string< std::byte > getZipString(const std::string& event);
void executeOneNote(const pqxx::row& row, pqxx::work& zipWorker);
