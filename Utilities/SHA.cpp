#include "SHA.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>

/* Help macros */
#define SHA_ROL(value, bits) (((value) << (bits)) | (((value) & 0xffffffff) >> (32 - (bits))))
#define SHA_BLK(i) (block[i&15] = SHA_ROL(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA */
#define SHA_R0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i]    + 0x5a827999 + SHA_ROL(v,5); w=SHA_ROL(w,30);
#define SHA_R1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + SHA_BLK(i) + 0x5a827999 + SHA_ROL(v,5); w=SHA_ROL(w,30);
#define SHA_R2(v,w,x,y,z,i) z += (w^x^y)           + SHA_BLK(i) + 0x6ed9eba1 + SHA_ROL(v,5); w=SHA_ROL(w,30);
#define SHA_R3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + SHA_BLK(i) + 0x8f1bbcdc + SHA_ROL(v,5); w=SHA_ROL(w,30);
#define SHA_R4(v,w,x,y,z,i) z += (w^x^y)           + SHA_BLK(i) + 0xca62c1d6 + SHA_ROL(v,5); w=SHA_ROL(w,30);

SHA::SHA() {
    Reset();
}

SHA::~SHA() {}

void SHA::Reset() {
    /* SHA initialization constants */
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;
    digest[4] = 0xc3d2e1f0;

    /* Reset counters */
    transforms = 0;
    buffer = "";
}

std::string SHA::FileHash(QString path, QString namedir)
{
    std::ifstream stream(path.toLocal8Bit(), std::ios::binary);

    Update(stream);

    std::string hash = final();
    stream.close();

    std::ofstream hash_file;
    QString hash_path = namedir + "/file.hash";

    hash_file.open(hash_path.toLocal8Bit());
    if (hash_file.is_open())
        std::copy(hash.begin(), hash.end(), std::ostream_iterator<uint8_t>(hash_file));
    hash_file.close();

    std::cout << hash << " SHA" << std::endl;
    return hash;
}

void SHA::Update(std::istream& is) {
    std::string rest_of_buffer;
    read(is, rest_of_buffer, BLOCK_BYTES - buffer.size());
    buffer += rest_of_buffer;

    while (is)
    {
        uint32_t block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(block);
        read(is, buffer, BLOCK_BYTES);
    }
}


/*
 * Add padding and return the message digest.
 */

std::string SHA::final()
{
    /* Total number of hashed bits */
    uint64_t total_bits = (transforms * BLOCK_BYTES + buffer.size()) * 8;

    /* Padding */
    buffer += 0x80;
    uint32_t orig_size = buffer.size();

    while (buffer.size() < BLOCK_BYTES)
        buffer += (char)0x00;

    uint32_t block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(block);
        for (size_t i = 0; i < BLOCK_INTS - 2; i++) block[i] = 0;
    }

    /* Append total_bits, split this uint64_t into two uint32_t */
    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32);
    transform(block);

    /* Hex std::string */
    std::ostringstream result;
    for (size_t i = 0; i < DIGEST_INTS; i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << (digest[i] & 0xffffffff);
    }

    /* Reset for next run */
    Reset();

    return result.str();
}

/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

void SHA::transform(uint32_t block[BLOCK_BYTES])
{
    /* Copy digest[] to working vars */
    uint32_t a = digest[0];
    uint32_t b = digest[1];
    uint32_t c = digest[2];
    uint32_t d = digest[3];
    uint32_t e = digest[4];


    /* 4 rounds of 20 operations each. Loop unrolled. */
    SHA_R0(a, b, c, d, e, 0);
    SHA_R0(e, a, b, c, d, 1);
    SHA_R0(d, e, a, b, c, 2);
    SHA_R0(c, d, e, a, b, 3);
    SHA_R0(b, c, d, e, a, 4);
    SHA_R0(a, b, c, d, e, 5);
    SHA_R0(e, a, b, c, d, 6);
    SHA_R0(d, e, a, b, c, 7);
    SHA_R0(c, d, e, a, b, 8);
    SHA_R0(b, c, d, e, a, 9);
    SHA_R0(a, b, c, d, e, 10);
    SHA_R0(e, a, b, c, d, 11);
    SHA_R0(d, e, a, b, c, 12);
    SHA_R0(c, d, e, a, b, 13);
    SHA_R0(b, c, d, e, a, 14);
    SHA_R0(a, b, c, d, e, 15);
    SHA_R1(e, a, b, c, d, 16);
    SHA_R1(d, e, a, b, c, 17);
    SHA_R1(c, d, e, a, b, 18);
    SHA_R1(b, c, d, e, a, 19);
    SHA_R2(a, b, c, d, e, 20);
    SHA_R2(e, a, b, c, d, 21);
    SHA_R2(d, e, a, b, c, 22);
    SHA_R2(c, d, e, a, b, 23);
    SHA_R2(b, c, d, e, a, 24);
    SHA_R2(a, b, c, d, e, 25);
    SHA_R2(e, a, b, c, d, 26);
    SHA_R2(d, e, a, b, c, 27);
    SHA_R2(c, d, e, a, b, 28);
    SHA_R2(b, c, d, e, a, 29);
    SHA_R2(a, b, c, d, e, 30);
    SHA_R2(e, a, b, c, d, 31);
    SHA_R2(d, e, a, b, c, 32);
    SHA_R2(c, d, e, a, b, 33);
    SHA_R2(b, c, d, e, a, 34);
    SHA_R2(a, b, c, d, e, 35);
    SHA_R2(e, a, b, c, d, 36);
    SHA_R2(d, e, a, b, c, 37);
    SHA_R2(c, d, e, a, b, 38);
    SHA_R2(b, c, d, e, a, 39);
    SHA_R3(a, b, c, d, e, 40);
    SHA_R3(e, a, b, c, d, 41);
    SHA_R3(d, e, a, b, c, 42);
    SHA_R3(c, d, e, a, b, 43);
    SHA_R3(b, c, d, e, a, 44);
    SHA_R3(a, b, c, d, e, 45);
    SHA_R3(e, a, b, c, d, 46);
    SHA_R3(d, e, a, b, c, 47);
    SHA_R3(c, d, e, a, b, 48);
    SHA_R3(b, c, d, e, a, 49);
    SHA_R3(a, b, c, d, e, 50);
    SHA_R3(e, a, b, c, d, 51);
    SHA_R3(d, e, a, b, c, 52);
    SHA_R3(c, d, e, a, b, 53);
    SHA_R3(b, c, d, e, a, 54);
    SHA_R3(a, b, c, d, e, 55);
    SHA_R3(e, a, b, c, d, 56);
    SHA_R3(d, e, a, b, c, 57);
    SHA_R3(c, d, e, a, b, 58);
    SHA_R3(b, c, d, e, a, 59);
    SHA_R4(a, b, c, d, e, 60);
    SHA_R4(e, a, b, c, d, 61);
    SHA_R4(d, e, a, b, c, 62);
    SHA_R4(c, d, e, a, b, 63);
    SHA_R4(b, c, d, e, a, 64);
    SHA_R4(a, b, c, d, e, 65);
    SHA_R4(e, a, b, c, d, 66);
    SHA_R4(d, e, a, b, c, 67);
    SHA_R4(c, d, e, a, b, 68);
    SHA_R4(b, c, d, e, a, 69);
    SHA_R4(a, b, c, d, e, 70);
    SHA_R4(e, a, b, c, d, 71);
    SHA_R4(d, e, a, b, c, 72);
    SHA_R4(c, d, e, a, b, 73);
    SHA_R4(b, c, d, e, a, 74);
    SHA_R4(a, b, c, d, e, 75);
    SHA_R4(e, a, b, c, d, 76);
    SHA_R4(d, e, a, b, c, 77);
    SHA_R4(c, d, e, a, b, 78);
    SHA_R4(b, c, d, e, a, 79);

    /* Add the working vars back into digest[] */
    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;

    /* Count the number of transformations */
    transforms++;
}


void SHA::buffer_to_block(const std::string& buffer, uint32_t block[BLOCK_BYTES])
{
    /* Convert the std::string (byte buffer) to a uint32_t array (MSB) */
    for (size_t i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4 * i + 3] & 0xff)
            | (buffer[4 * i + 2] & 0xff) << 8
            | (buffer[4 * i + 1] & 0xff) << 16
            | (buffer[4 * i + 0] & 0xff) << 24;
    }
}


void SHA::read(std::istream& is, std::string& s, int max)
{
    char* sbuf = new char[max];
    is.read(sbuf, max);
    s.assign(sbuf, is.gcount());
}
