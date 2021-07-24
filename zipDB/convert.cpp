#include "convert.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

std::string convert(const std::vector< uint8_t > vec)
{
    std::stringstream ss;
    for (size_t i = 0; i < vec.size(); i++)
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
        << (unsigned)vec[i];
    return ss.str();
}