#include <string>
#include <iostream>
#include <pqxx/pqxx>

int main()
{
    setlocale(LC_ALL, "rus");
    std::string connectionString = "host=localhost port=5432 dbname=postgres user=postgres password =123456";

    try
    {
        pqxx::connection connectionObject(connectionString.c_str());
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    system("pause");
}