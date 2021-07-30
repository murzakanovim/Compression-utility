#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

#include <pqxx/pqxx>
#include <zlib.h>
#include "compress.h"
#include "tools.h"

int main()
{
    std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
    pqxx::connection oldConnection(oldConnectionSetting);
    pqxx::work oldWorker(oldConnection);
    pqxx::result res = oldWorker.exec("SELECT event FROM t_event LIMIT 1");
    auto vec = NConsulUtils::zip(res[0][0].c_str(), calculateSizeOfString(res[0][0].c_str()));
    //std::string utf8;
    //utf8.clear();
    //for (auto a : vec)
    //{
    //    utf8 += toBinary(a);
    //    utf8.push_back(' ');
    //}

    //std::string query = "INSERT INTO test(test) VALUES('" + utf8 + '\'' + ')' + ';';
    try
    {
        oldWorker.exec(query);
        oldWorker.commit();
    }
    catch (const std::exception& exc)
    {
        std::cerr << exc.what();
    }

}
