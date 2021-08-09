#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>

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

bool execute(pqxx::work& oldWorker, pqxx::work& newWorker)
{
    int id = 0;
    while (true)
    {
        std::string query = "SELECT * FROM t_event ORDER BY timestamp DESC LIMIT 1000 OFFSET " + std::to_string(id);
        pqxx::result res = oldWorker.exec(query);
        
        if (res.empty())
        {
            break;
        }

        for (const auto& row : res)
        {
            executeOneNote(row, newWorker);
        }
        id += 1000;
    }
    return true;
}

int main()
{
    std::fstream out("out1.txt");
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "ngp";
    std::string user = "ngp";
    std::string newDbName = "ngpNew";
    std::string password = "123456";
    try
    {
        PConnection conn(host, port, dbname, user, password);
        pqxx::work oldWorker = conn.getWorker();

        PConnection connNew(host, port, newDbName, user, password);
        pqxx::work newWorker = connNew.getWorker();
        newWorker.conn().prepare("insert", "INSERT INTO t_event (type, subjects, timestamp, zip_event, ts_vector) VALUES($1, $2, $3, $4, $5);");
        if (execute(oldWorker, newWorker))
        {
            newWorker.commit();
        }
    }
    catch (const std::exception& exc)
    {
        out << exc.what();
    }
   
}