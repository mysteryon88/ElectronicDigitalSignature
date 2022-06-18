#include "RC4.h"
#include <iostream>
#include <random>

RC4::RC4(std::string key)
{
	//KSA
	uint8_t j = 0;
    uint32_t len = key.size();
	for (auto i = 0; i < S_LEN; ++i) Sbox[i] = i;

    for (auto i = 0; i < S_LEN; ++i)
    {
       j = (j + Sbox[i] + (uint8_t)key[i % len]) % S_LEN;
		Swap(&Sbox[i], &Sbox[j]);
	}
}

RC4::~RC4() {}

void RC4::PrintASCII(char* input, uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
		std::cout << " " << (int)input[i];
	std::cout << std::endl;
}

void RC4::KeyGen(char* output, uint16_t k_size)
{
    uint8_t i = 0, j = 0, t;
    for (uint16_t k = 0; k < k_size; ++k)
    {
        i += 1;
        j += Sbox[i];
        Swap(&Sbox[i], &Sbox[j]);
        t = Sbox[i] + Sbox[j];
        output[k] = (char)Sbox[t];
	}
}

void RC4::EnDeCrypt(char* output, char* key, uint16_t k_size, char* input, uint32_t mes_len)
{
    for (uint32_t i = 0; i < mes_len; ++i)
        output[i] = input[i] ^ key[i % k_size];
}

void RC4::Swap(uint8_t* x, uint8_t* y)
{
	uint8_t t = *x;
	*x = *y;
	*y = t;
}
