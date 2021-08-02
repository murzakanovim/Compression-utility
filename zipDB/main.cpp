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
    std::fstream out("out.txt");
    std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
    pqxx::connection oldConnection(oldConnectionSetting);
    pqxx::work oldWorker(oldConnection); 
    pqxx::result res = oldWorker.exec("SELECT * FROM t_event LIMIT 100000");
   
    std::string newConnectionSetting("host=localhost port=5432 user=ngp dbname=ngpNew password=123456");
    pqxx::connection newConnection(newConnectionSetting);
    pqxx::work newWorker(newConnection);

    newConnection.prepare("insert", "INSERT INTO t_event (type, subjects, timestamp, zip_event, zip_ts_vector) VALUES($1, $2, $3, $4, $5);");
    try
    {
        for (const auto& row : res)
        {            
            auto pair = getZipVecs(row);
            column_info_t column(row, pair.first, pair.second);//умирает где-то здесь
            newWorker.exec_prepared("insert", column.type, column.subjects, column.timestamp, column.zip_event, column.zip_ts_vector);
        }
        newWorker.commit();
    }
    catch (const std::exception& exc)
    {        
        std::cerr << exc.what();
    }
}