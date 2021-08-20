#include <string>
#include <iostream>

#include <pqxx/pqxx>
#include <zlib.h>

#include "CConnection.h"
#include "tools.h"
#include "compress.h"

void execute()
{
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "ngp";
    std::string user = "ngp";
    std::string zipDbName = "ngpNew";
    std::string password = "123456";

    const unsigned int PACK = 500;

    CConnection conn(host, port, dbname, user, password);
    conn.make_prepared_query("select", "SELECT * FROM t_event ORDER BY timestamp DESC LIMIT $1 OFFSET $2");

    CConnection zipConn(host, port, zipDbName, user, password);
    zipConn.make_prepared_query("insert", "INSERT INTO t_event(type, subjects, timestamp, zip_event, ts_vector) VALUES($1, $2, $3, $4, $5);");
    unsigned int total = 0;//счетчик 
    
    int id = 0;
    while (true)
    {
        std::unique_ptr< pqxx::work > worker = conn.getWorker();
        std::unique_ptr< pqxx::work > zipWorker = zipConn.getWorker();
        unsigned int before = 0;//сколько было
        unsigned int after = 0;//сколько стало
        
        pqxx::result res = worker->exec_prepared("select", PACK, id);
        worker->commit();

        if (res.empty())
        {
            break;
        }

        for (const auto& row : res)
        {
            try
            {
                executeOneNote(row, *zipWorker, before, after);//не нравится
            }
            catch (const std::exception&)
            {
                std::cerr << "Something wrong in LIMIT" + std::to_string(PACK) + "and OFFSET " + std::to_string(id);
            }
        }
        zipWorker->commit();
        id += PACK;

        total += before - after;
    }
}

int main()
{
    try
    {
        execute();
    }
    catch (const std::exception& exc)
    {
        std::cerr << exc.what();
    }
    return 0;
}
