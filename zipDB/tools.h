#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>
#include <pqxx/row>
#include "column_info_t.h"

//std::string toBinary(int dec);
//std::string getResultQuery(column_info_t ci);
int calculateSizeOfString(const char* ptr);
std::pair < std::vector< uint8_t >, std::vector< uint8_t > > getZipVecs(const pqxx::row& row);

#endif
