#pragma once
#include <cstdint>
#include <random>
#include <iostream>

class RSA
{
private:
    uint64_t privat_key;
    uint64_t public_key;

    bool IsPrime(uint64_t n, unsigned short iter = 5);
    bool check_composite(uint64_t n, uint64_t a, uint64_t d, int s);
    uint64_t binpower(uint64_t base, uint64_t e, uint64_t mod);

    uint64_t CalculateE(uint64_t t);
    uint64_t GreatestCommonDivisor(uint64_t e, uint64_t t);

    int64_t CalculateD(int64_t a, int64_t m);
    int64_t gcdex(int64_t a, int64_t b, int64_t& x, int64_t& y);
public:
    RSA();
    ~RSA();
    void GenerateKey();
        
};

