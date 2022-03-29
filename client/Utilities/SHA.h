#ifndef SHA_H
#define SHA_H

#include <string>
#include <cstdint>
#include <QString>

#include "debug.h"

#define DIGEST_INTS 5 //number of 32bit integers per SHA1 digest
#define BLOCK_INTS 16 // number of 32bit integers per SHA1 block

class SHA
{

private:
    static const uint32_t BLOCK_BYTES = BLOCK_INTS * 4;

    uint32_t digest[DIGEST_INTS];
    std::string buffer;
    uint64_t transforms;

    void Reset();
    void transform(uint32_t block[BLOCK_BYTES]);

    static void buffer_to_block(const std::string& buffer, uint32_t block[BLOCK_BYTES]);
    static void read(std::istream& is, std::string& s, int max);
    void Update(std::istream& is);
    std::string final();
public:
    SHA();
    ~SHA();
    std::string FileHash(QString path);
};

#endif // SHA_H
