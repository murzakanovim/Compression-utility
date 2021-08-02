#include "tools.h"
#include <string>
#include "column_info_t.h"
#include "compress.h"

std::string toBinary(int dec)
{
    std::string res;
    res.resize(8);
    for (int i = 0; i < 8; i++)
    {
        res[7 - i] = (dec % 2 + 48);
        dec /= 2;
    }
    return res;
}

int calculateSizeOfString(const char* ptr)
{
    int i = 0;
    while (ptr[i] != '\0')
    {
        i++;
    }
    return i;//?
}

std::pair<std::vector<uint8_t>, std::vector<uint8_t>> getZipVecs(const pqxx::row& row)
{
    auto vecEvent = NConsulUtils::zip(row[3].c_str(), calculateSizeOfString(row[3].c_str()));
    auto vecTsVector = NConsulUtils::zip(row[4].c_str(), calculateSizeOfString(row[4].c_str()));

    return std::pair<std::vector<uint8_t>, std::vector<uint8_t>>(vecEvent, vecTsVector);
}

//std::string getResultQuery(column_info_t ci)
//{
//    std::string base = "INSERT INTO t_event VALUES";
//    std::string resultString = base + '(' + ci.type + ',' + ' '
//         + '\'' + ci.subjects + '\'' + ',' + ' '
//         + '\'' + ci.timestamp + '\'' + ',' + ' ' 
//         + '\'' + ci.zip_event + '\'' + ',' + ' '
//        +  '\'' + ci.zip_ts_vector + '\'' + ')' + ';';
//    return resultString;
//}
