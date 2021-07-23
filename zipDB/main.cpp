#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <pqxx/pqxx>
#include <zlib.h>
#include "compress.h"

int main()
{
    std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
    pqxx::connection oldConnection(oldConnectionSetting);
    pqxx::work oldWorker(oldConnection);
    pqxx::result res = oldWorker.exec("SELECT event FROM t_event LIMIT 100");
    auto it = res.begin();
    auto end = res.end();
    //std::cout << res[0][0];
    auto vec = NConsulUtils::zip(&it, 100, NConsulUtils::ECompressionLevel::BEST);
    auto vecUn = NConsulUtils::unzip(vec.data(), 100);
    auto vecIt = vecUn.begin();
    auto vecEnd = vecUn.end();
    while (vecIt != vecEnd)
    {
        std::cout << *vecIt << " ";
        vecIt++;
    }
    std::cout << "\n";
    return 0;
}