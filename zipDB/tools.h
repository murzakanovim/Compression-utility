#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

#include <pqxx/row>
#include <pqxx/transaction>
#include <pqxx/blob>

std::basic_string< std::byte > getZipString(const std::string& event);
void executeOneNote(const pqxx::row& row, pqxx::work& newWorker);
//pqxx::blob toBinary(int dec);

#endif
