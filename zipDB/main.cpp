#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>

#include <pqxx/pqxx>
#include <zlib.h>
#include "column_info_t.h"
#include "tools.h"
#include "compress.h"

int main()
{
    std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
    pqxx::connection oldConnection(oldConnectionSetting);
    pqxx::work oldWorker(oldConnection);
    pqxx::result res = oldWorker.exec("SELECT * FROM t_event");
    
    std::string newConnectionSetting("host=localhost port=5432 user=ngp dbname=ngpNew password=123456");
    pqxx::connection newConnection(newConnectionSetting);
    pqxx::work newWorker(newConnection);
    try
    {
        for (auto row : res)
        {
            auto pair = getZipVecs(row);
            column_info_t column(row, pair.first, pair.second);
            std::string query = getResultQuery(column);
            newWorker.exec(query);
            newWorker.commit();
        }
    }
    catch (const std::exception& exc)
    {
        std::cerr << exc.what();
    }
}