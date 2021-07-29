#define NOMINMAX
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>


#include <pqxx/pqxx>
#include <zlib.h>
#include "compress.h"

std::string toBinary(int dec)
{
    std::string res;
    res.resize(8);
    for (int i = 0; i < 8; i++)
    {
        res[7 - i] = (dec % 2 + 48);
        dec /= 2;
    }
    return res;
}

int calculateSizeOfString(const char* ptr)
{
    int i = 0;
    while (ptr[i] != '\0')
    {
        i++;
    }
    return i;
}

std::string ConvertWideToUtf8(const std::wstring& wstr)
{
    int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;
}

int main()
{
    std::ofstream out("out.txt");
	std::string oldConnectionSetting("host=localhost port=5432 user=ngp dbname=ngp password=123456");
    pqxx::connection oldConnection(oldConnectionSetting);
    pqxx::work oldWorker(oldConnection);
    pqxx::result res = oldWorker.exec("SELECT event FROM t_event LIMIT 1");
    auto vec = NConsulUtils::zip(res[0][0].c_str(), calculateSizeOfString(res[0][0].c_str()));
    std::string utf8;
    utf8.clear();
    for (auto a : vec)
    {
        utf8 += toBinary(a);
        utf8.push_back(' ');
    }

    
    std::string query = "INSERT INTO test(test) VALUES('" + utf8 + '\'' + ')' + ';';
    try
    {
        oldWorker.exec(query);
        oldWorker.commit();
    }
    catch (const std::exception& exc)
    {
        out << exc.what();
    }

}