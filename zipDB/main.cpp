#include <string>
#include <iostream>
#include <fstream>

#include <pqxx/except>
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

    unsigned int total = 0;
    unsigned int before = 0;
    unsigned int after = 0;

    CConnection conn(host, port, dbname, user, password);
    conn.make_prepared_query("select", "SELECT * FROM t_event ORDER BY timestamp DESC LIMIT $1 OFFSET $2");

    CConnection zipConn(host, port, zipDbName, user, password);
    zipConn.make_prepared_query("insert", "INSERT INTO t_event(type, subjects, timestamp, zip_event, ts_vector) VALUES($1, $2, $3, $4, $5);");
    
    unsigned int id = 0;

    while (true)
    {
        std::shared_ptr< pqxx::work > worker = conn.getSharedWorker();
        std::shared_ptr< pqxx::work > zipWorker = zipConn.getSharedWorker();

        pqxx::result res = worker->exec_prepared("select", PACK, id);

        if (res.empty())
        {
            break;
        }

        for (const auto& row : res)
        {
            try
            {
                executeOneNote(row, zipWorker, before, after);
            }
			catch (const pqxx::sql_error& exc)
			{
				std::cerr << "Probably problem is sql query\n";
				std::cerr << exc.what() << '\n';
				return;
			}
            catch (const std::exception&)
            {
                std::cerr << "Something wrong in LIMIT " + std::to_string(PACK) + " and OFFSET " + std::to_string(id) << '\n';
				std::cerr << exc.what() << '\n';
			}
        }

        zipWorker->commit();
        id += PACK;
    }
    total += before - after;

    std::fstream out;
    out.open("out.txt");
    if (out.is_open())
    {
        printInfoAboutMemory(out, total, before, after);
    }
    else
    {
        printInfoAboutMemory(std::cout, total, before, after);
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
