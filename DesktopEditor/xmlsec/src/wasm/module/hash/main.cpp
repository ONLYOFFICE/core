#include <memory>
#include "../openssl/include/openssl/sha.h"
#include "../openssl/include/openssl/md2.h"
#include "../openssl/include/openssl/md4.h"
#include "../openssl/include/openssl/md5.h"
#include "../openssl/include/openssl/whrlpool.h"
#include "../openssl/include/openssl/ripemd.h"


#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OOXML_HASH_ALG_MD2          0
#define OOXML_HASH_ALG_MD4          1
#define OOXML_HASH_ALG_MD5          2
#define OOXML_HASH_ALG_RIPEMD160    3
#define OOXML_HASH_ALG_SHA1         4
#define OOXML_HASH_ALG_SHA256       5
#define OOXML_HASH_ALG_SHA384       6
#define OOXML_HASH_ALG_SHA512       7
#define OOXML_HASH_ALG_WHIRLPOOL    8

WASM_EXPORT unsigned char* hash(unsigned char* data, int size, int alg)
{
    int nBufLen = 0;
    unsigned char* pBufData = NULL;
    size_t d = (size_t)size;

    switch (alg)
    {
        case OOXML_HASH_ALG_MD2:
        {
            nBufLen = 16;
            pBufData = (unsigned char*)malloc(nBufLen);
            MD2(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_MD4:
        {
            nBufLen = 16;
            pBufData = (unsigned char*)malloc(nBufLen);
            MD4(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_MD5:
        {
            nBufLen = 16;
            pBufData = (unsigned char*)malloc(nBufLen);
            MD5(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_RIPEMD160:
        {
            nBufLen = 20;
            pBufData = (unsigned char*)malloc(nBufLen);
            RIPEMD160(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_SHA1:
        {
            nBufLen = 20;
            pBufData = (unsigned char*)malloc(nBufLen);
            SHA1(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_SHA256:
        {
            nBufLen = 32;
            pBufData = (unsigned char*)malloc(nBufLen);
            SHA256(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_SHA384:
        {
            nBufLen = 48;
            pBufData = (unsigned char*)malloc(nBufLen);
            SHA384(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_SHA512:
        {
            nBufLen = 64;
            pBufData = (unsigned char*)malloc(nBufLen);
            SHA512(data, d, pBufData);
            break;
        }
        case OOXML_HASH_ALG_WHIRLPOOL:
        {
            nBufLen = 64;
            pBufData = (unsigned char*)malloc(nBufLen);
            WHIRLPOOL(data, d, pBufData);
            break;
        }
    default:
        break;
    }

    return pBufData;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
#include <string>
int main()
{
    std::string data = "test";
    unsigned char* result = hash((const unsigned char*)data.c_str(), (unsigned int)data.length(), OOXML_HASH_ALG_SHA256);

    return 0;
}
#endif
