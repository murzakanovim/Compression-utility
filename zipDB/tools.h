#pragma once

#include <string>
#include <vector>
#include <memory>

#include <pqxx/row>
#include <pqxx/transaction>
#include <pqxx/blob>

std::basic_string< std::byte > getZipString(const std::string& event);
void executeOneNote(const pqxx::row& row, std::shared_ptr< pqxx::work > zipWorker, unsigned int& before, unsigned int& after);
void printInfoAboutMemory(std::ostream& out, unsigned int total, unsigned int before, unsigned int after);
