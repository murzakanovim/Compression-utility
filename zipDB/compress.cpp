#include "compress.h"

#include <cassert>
#include <zlib.h>
#include <stdexcept>

namespace
{
    constexpr size_t CHUNK = 16 * 1024;

    constexpr std::uint8_t WINDOW_BITS = 15;
    constexpr std::uint8_t GZIP_ENCODING = 16;

    using namespace NConsulUtils;

    int convert2zlib(ECompressionLevel level)
    {
        switch (level)
        {
        case ECompressionLevel::FASTEST: return Z_BEST_SPEED;
        case ECompressionLevel::NORMAL:  return Z_DEFAULT_COMPRESSION; // or the middle value between Z_BEST_COMPRESSION and Z_BEST_SPEED ?
        case ECompressionLevel::BEST:    return Z_BEST_COMPRESSION;
        default:
            assert(false);
            throw std::runtime_error("Unsupported compression level");
        }
    }
} // anonimous namespace

namespace NConsulUtils
{
    std::vector<uint8_t> compress(void const* data, size_t size, ECompressionMethod method, ECompressionLevel level)
    {
        if (data == nullptr || size == 0)
            return {};

        // zlib struct
        z_stream strm = {};
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        int ret = Z_OK;
        switch (method)
        {
        case ECompressionMethod::ZIP:
            ret = deflateInit(&strm, convert2zlib(level));
            break;
        case ECompressionMethod::GZIP:
            ret = deflateInit2(&strm, convert2zlib(level), Z_DEFLATED, WINDOW_BITS | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY);
            break;
        default:
            throw std::runtime_error("Unsupported compression method");
        }
        if (ret != Z_OK)
            throw std::runtime_error("Can't init zlib!");

        //BOOST_SCOPE_EXIT_ALL(&strm) {
         //   (void)deflateEnd(&strm);
        //};

        strm.avail_in = size;
        strm.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(data)); // input char array

        std::vector<uint8_t> outData;
        outData.resize(deflateBound(&strm, size));

        size_t nextOutIdx = 0;
        for (;;)
        {
            strm.avail_out = outData.size() - nextOutIdx;
            strm.next_out = outData.data() + nextOutIdx;

            ret = deflate(&strm, Z_FINISH);    /* no bad return value */
            if (ret == Z_STREAM_ERROR)
                throw std::runtime_error("Zip failed! Z_STREAM_ERROR");

            if (strm.avail_out != 0)
            {
                outData.resize(outData.size() - strm.avail_out);
                break;
            }

            nextOutIdx = outData.size();
            outData.resize(outData.size() + CHUNK);
        }
        return outData;
    }

    std::vector<uint8_t> decompress(void const* data, size_t size, ECompressionMethod method)
    {
        if (data == nullptr || size == 0)
            return {};

        /* allocate inflate state */
        z_stream strm = {};
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        int ret = Z_OK;
        switch (method)
        {
        case ECompressionMethod::ZIP:
            ret = inflateInit(&strm);
            break;
        case ECompressionMethod::GZIP:
            ret = inflateInit2(&strm, WINDOW_BITS | GZIP_ENCODING);
            break;
        default:
            throw std::runtime_error("Unsupported decompression!");
        }
        if (ret != Z_OK)
            throw std::runtime_error("Can't setup zlib for decompression!");

        //BOOST_SCOPE_EXIT_ALL(&strm) {
        //   inflateEnd(&strm);
        //};

        strm.avail_in = size;
        strm.next_in = const_cast<Bytef*>(reinterpret_cast<Bytef const*>(data));

        std::vector<uint8_t> outData;
        outData.resize(std::max(2 * size, static_cast<size_t>(256U)));

        size_t nextOutIdx = 0;
        for (;;)
        {
            strm.avail_out = outData.size() - nextOutIdx;
            strm.next_out = outData.data() + nextOutIdx;

            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR)  /* state not clobbered */
                throw std::runtime_error("Z_STREAM_ERROR in decompression");
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                throw std::runtime_error(std::string("zlib error: ") + strm.msg);
            }

            if (strm.avail_out != 0)
            {
                outData.resize(outData.size() - strm.avail_out);
                break;
            }

            nextOutIdx = outData.size();
            outData.resize(outData.size() + CHUNK);
        }
        return outData;
    }

    std::vector<uint8_t> zip(void const* data, size_t size, ECompressionLevel level /*= ECompressionLevel::NORMAL*/)
    {
        return compress(data, size, ECompressionMethod::ZIP, level);
    }

    std::vector<uint8_t> unzip(void const* data, size_t size)
    {
        return decompress(data, size, ECompressionMethod::ZIP);
    }

    std::vector<uint8_t> gzip(void const* data, size_t size, ECompressionLevel level /*= ECompressionLevel::NORMAL*/)
    {
        return compress(data, size, ECompressionMethod::GZIP, level);
    }

    std::vector<uint8_t> gunzip(void const* data, size_t size)
    {
        return decompress(data, size, ECompressionMethod::GZIP);
    }
}