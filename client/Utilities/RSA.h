#ifndef RSA_H
#define RSA_H

#include <string>
#include <cstdint>
#include <QString>

#include "debug.h"

#define SHA_LEN 40

enum ERRORS
{
    OK,
    FILE_NOT_OPEN,
    FILE_MIXED_UP
};

struct Key
{
    uint64_t modulus;   //n
    uint64_t exp;       //e/d
};

class RSA
{
private:
    uint64_t modulus;   //n
    uint64_t secret_exp;//d
    uint64_t open_exp;  //e

    bool IsPrime(uint64_t n, uint16_t iter = 5);
    bool CheckComposite(uint64_t n, uint64_t a, uint64_t d, int32_t s);
    uint64_t Binpower(uint64_t base, uint64_t e, uint64_t mod);

    uint64_t CalculateE(uint64_t t);
    uint64_t GreatestCommonDivisor(uint64_t e, uint64_t t);

    int64_t CalculateD(int64_t a, int64_t m);
    int64_t gcdex(int64_t a, int64_t b, int64_t& x, int64_t& y);
    std::string Decipher(QString path);

public:
    RSA();
    ~RSA();
    void GenerateKey();
    void GetPubKey(Key* key);
    void GetPriKey(Key* key);
    void PrintKeys();
    uint8_t Encrypt(std::string data);
    int8_t LoadMyKey(uint64_t d, uint64_t n);
    bool Verification(QString hash_enc, QString file);
    Key pub_key;
};

#endif // RSA_H
