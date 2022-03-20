#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>


    enum class ECompressionMethod
    {
        ZIP,
        GZIP,
    };

    enum class ECompressionLevel
    {
        FASTEST,
        NORMAL,
        BEST,
    };

    std::vector<uint8_t> compress(void const* data, size_t size, ECompressionMethod method, ECompressionLevel level);
    std::vector<uint8_t> decompress(void const* data, size_t size, ECompressionMethod method);

    std::vector<uint8_t> zip(void const* data, size_t size, ECompressionLevel level = ECompressionLevel::NORMAL);
    std::vector<uint8_t> unzip(void const* data, size_t size);

    std::vector<uint8_t> gzip(void const* data, size_t size, ECompressionLevel level = ECompressionLevel::NORMAL);
    std::vector<uint8_t> gunzip(void const* data, size_t size);

