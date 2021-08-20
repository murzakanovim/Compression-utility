#include <string>
#include <iostream>

#include <pqxx/pqxx>
#include <pqxx/except.hxx>
#include <zlib.h>

#include "CConnection.h"
#include "tools.h"
#include "compress.h"

//что-то что возвращает worker и сохраняет connection // почти сделано
//исключение не должно завершать программу // нет
//вынести цикл // есть
//сделать что-то, что обрабатывает одну запись // есть
//обрабатывать по 10000 //есть

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
    std::unique_ptr< pqxx::work > worker = conn.getWorker();

    CConnection zipConn(host, port, zipDbName, user, password);
    zipConn.make_prepared_query("insert", "INSERT INTO t_event(type, subjects, timestamp, zip_event, ts_vector) VALUES($1, $2, $3, $4, $5);");
    //счетчик 
    
    int id = 0;
    while (true)
    {
        std::unique_ptr< pqxx::work > zipWorker = zipConn.getWorker();
        //сколько было
        //сколько стало
        
        pqxx::result res = worker->exec_prepared("select", PACK, id);

        if (res.empty())
        {
            break;
        }

        for (const auto& row : res)
        {
            try
            {
                executeOneNote(row, *zipWorker);
            }
            catch (const std::exception&)
            {
                std::cerr << "";//
            }
        }
        zipWorker->commit();
        id += PACK;
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
