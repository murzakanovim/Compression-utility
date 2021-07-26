#define NOMINMAX
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <codecvt>
#include <Windows.h>

#include <pqxx/pqxx>
#include <zlib.h>
#include "convert.h"
#include "compress.h"

int calculateSizeOfString(const char* ptr)
{
    int i = 0;
    while (ptr[i] != '\0')
    {
        i++;
    }
    return i - 1;
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
    std::wstring wstr;
    for (auto a : vec)
    {
        wstr.push_back(a);
    }
    auto utf8 = ConvertWideToUtf8(wstr);
    
    std::string query = "INSERT INTO test(test) VALUES('" + utf8 + '\'' + ')' + ';';
    out << query;
    try
    {
        oldWorker.exec(query);
    }
    catch (const std::exception& exc)
    {
        out << exc.what();
    }
}