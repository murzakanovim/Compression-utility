//#include <string>
//#include <iostream>
//#include <algorithm>
//#include <iterator>
//#include <vector>
//#include <fstream>
//
//#include <pqxx/pqxx>
//#include <zlib.h>
//#include "convert.h"
//#include "compress.h"
//
//int calculateSizeOfString(const char* ptr)
//{
//    int i = 0;
//    while (ptr[i] != '\0')
//    {
//        i++;
//    }
//    return i - 1;
//}
//
//struct column_info_t
//{
//    /*column_info_t(const std::vector< uint8_t >& zip_event_info, const std::vector< uint8_t >& zip_ts_vector_info) :
//        zip_event(zip_event_info.data(), zip_event_info.size()),
//        zip_ts_vector(zip_ts_vector_info.data(), zip_ts_vector_info.size())
//    {}*/
//    std::string type;
//    std::string subjects;
//    std::string timestamp;
//    std::string zip_event;
//    std::string zip_ts_vector;
//};
//
//
//column_info_t makeColumnInfo(const pqxx::row& row, const std::vector< uint8_t >& zip_event_info,
//                                const std::vector< uint8_t >& zip_ts_vector_info)
//{
//    column_info_t column_info{};
//    column_info.type = row[0].c_str();
//    column_info.subjects = row[1].c_str();
//    column_info.timestamp = row[2].c_str();
//    auto itEvent = zip_event_info.begin();
//    auto endEvent = zip_event_info.end();
//    
//    while (itEvent != endEvent)
//    {
//        column_info.zip_event.push_back(*itEvent);
//        itEvent++;
//    }
//    auto itTsVector = zip_ts_vector_info.begin();
//    auto endTsVector = zip_ts_vector_info.end();
//    while (itTsVector != endTsVector)//Техника повторного использования кода
//    {
//        column_info.zip_ts_vector.push_back(*itTsVector);
//        itTsVector++;
//    }
//    return column_info;
//}
//
//std::string getResultQuery(column_info_t ci)
//{
//    std::string base = "INSERT INTO t_event (type, subjects, timestamp, zip_event, zip_ts_vector) VALUES";
//    std::string resultString = base + '(' + ci.type + ',' +
//        '\'' + ci.subjects + '\'' + ',' +
//        '\'' + ci.timestamp + '\'' + ',' +
//        '\'' + ci.zip_event + '\'' + ',' +
//        '\'' + ci.zip_ts_vector + '\'' + ')' + ';';
//    return resultString;
//}
//
//
//int main()
//{
//    std::fstream out("out.txt");
//    std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
//    pqxx::connection oldConnection(oldConnectionSetting);
//    pqxx::work oldWorker(oldConnection);
//    pqxx::result res = oldWorker.exec("SELECT * FROM t_event LIMIT 1");
//
//    int i = 0;
//    auto vecEvent = NConsulUtils::zip(res[i][3].c_str(), calculateSizeOfString(res[i][3].c_str()));
//    auto vecTsVector = NConsulUtils::zip(res[i][4].c_str(), calculateSizeOfString(res[i][4].c_str()));
//    column_info_t column = makeColumnInfo(res[0], vecEvent, vecTsVector);
//    std::string query = getResultQuery(column);
//    //std::cout << query;
//    //
//    // надо разобраться, как теперь последоватаельность байтов в ставить в БД
//    // Скорее всего, нужно подключать воркер
//    std::string newConnectionSetting("host=localhost port=5432 user=ngp dbname=ngpNew password=123456");
//    pqxx::connection newConnection(newConnectionSetting);
//    //newConnection.prepare("test", "")
//    pqxx::work newWorker(newConnection);
//    try
//    {
//        newWorker.exec(query);
//    }
//    catch (const std::exception& exc)
//    {
//        out << exc.what();
//    }
//    //auto vecUn = NConsulUtils::unzip(vec.data(), vec.size());
//}