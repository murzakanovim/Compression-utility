#include "tools.h"

#include <string>
#include <pqxx/pqxx>
#include <iostream>
#include <iomanip>

#include "compress.h"
#include "CConnection.h"


void executeOneNote(const pqxx::row& row, pqxx::work& zipWorker, unsigned int& before, unsigned int& after)
{
    std::string event = row.at("event").as< std::string >();
    std::basic_string< std::byte > zipString = getZipString(event);

    before += event.size();
    after += zipString.size();

    zipWorker.exec_prepared("insert", row.at("type").as< std::string >(), row.at("subjects").as< std::string >(), row.at("timestamp").as< std::string >(), zipString, row.at("ts_vector").as< std::string >());
}

void printInfoAboutMemory(std::ostream& out, unsigned int total, unsigned int before, unsigned int after)
{
	double percentage = ((total * 1.0) / before) * 100;
	
    out << "Total saved memory: " << total << " bytes\n";
    out << "Memory of event before zip: " << before << " bytes\n";
    out << "Memory of event after zip: " << after << " bytes\n";
	out << "How much memory has decreased in percentage: " << std::setprecision(1) << percentage << " %\n"; 
}

std::basic_string< std::byte > getZipString(const std::string& event)
{
    auto zipVec = NConsulUtils::zip(event.data(), event.size(), NConsulUtils::ECompressionLevel::BEST);
    std::basic_string< std::byte > zipString{};
    for (const auto& byte : zipVec)
    {
        zipString.push_back(std::byte(byte));
    }
    return zipString;
}
