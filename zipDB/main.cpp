#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <optional>

#include <pqxx/pqxx>
#include <zlib.h>

#include "PConnection.h"
#include "column_info_t.h"
#include "tools.h"
#include "compress.h"
//что-то что возвращает worker и сохраняет connection // почти сделано
//исключение не должно завершать программу
//вынести цикл
//сделать что-то, что обрабатывает одну запись
//обрабатывать по 1000

std::optional< int > execute(pqxx::work& worker, pqxx::work& zipWorker, int id = 0)
{
    int current = id;
    while (true)
    {
        std::string query = "SELECT * FROM t_event ORDER BY timestamp DESC LIMIT 10000 OFFSET " + std::to_string(id);
        pqxx::result res = worker.exec(query);
        
        if (res.empty())
        {
            break;
        }
        for (const auto& row : res)
        {
            try
            {
                executeOneNote(row, zipWorker);
                ++current;
                if (id > 5)
                    throw std::runtime_error("");
            }
            catch (const std::exception&)
            {
                return {current};
            }
        }
        id += 10000;
    }
    return std::nullopt;
}

int main()
{
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "ngp";
    std::string user = "ngp";
    std::string newDbName = "ngpNew";
    std::string password = "123456";
    try
    {
        PConnection conn(host, port, dbname, user, password);
        pqxx::work worker = conn.getWorker();

        PConnection zipConn(host, port, newDbName, user, password);
        pqxx::work zipWorker = zipConn.getWorker();
        zipWorker.conn().prepare("insert", "INSERT INTO t_event (type, subjects, timestamp, zip_event, ts_vector) VALUES($1, $2, $3, $4, $5);");
        auto fail = execute(worker, zipWorker);
        if (fail)
        {
            execute(worker, zipWorker, fail.value());
        }
    }
    catch (const std::exception& exc)
    {
        std::cerr << exc.what();
    }
    return 0;
}