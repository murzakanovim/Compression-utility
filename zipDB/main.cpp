#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>

#include <pqxx/pqxx>
#include <zlib.h>
#include "convert.h"
#include "compress.h"

int calculateSizeOfString(const char* ptr)
{
    int i = 0;
    while (ptr[i] != '\0')
    {
        i++;
    }
    return i - 1;
}

int main()
{
    std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
    pqxx::connection oldConnection(oldConnectionSetting);
    pqxx::work oldWorker(oldConnection);
    pqxx::result res = oldWorker.exec("SELECT event FROM t_event LIMIT 1");

    auto vec = NConsulUtils::zip(res[0][0].c_str(), calculateSizeOfString(res[0][0].c_str()));
    // надо разобраться, как теперь последоватаельность байтов в ставить в БД
    // Скорее всего, нужно подключать воркер
    std::string addQuery = "UPDATE t_event SET zip_event = '" + vec + "' WHERE id =" + id;
        
    newWorker.exec(addQuery.c_str());
    auto vecUn = NConsulUtils::unzip(vec.data(), vec.size());
}