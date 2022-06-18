#include "RSA.h"
#include "SHA.h"

#include <QDir>
#include <QFile>
#include <fstream>
#include <iterator>
#include <iostream>
#include <QRandomGenerator>

RSA::RSA()
{
	open_exp = 0ull;
	secret_exp = 0ull;
	modulus = 0ull;
}

RSA::~RSA() {}

void RSA::GenerateKey()
{
	uint64_t p = 0ull, q = 0ull, euler = 0ull;

    QRandomGenerator rand(time(0));
    do
    {
        p = 5000ull + rand.generate64() % 20000ull;
        if (IsPrime(p)) break;
    } while (true);

    //generate q
    do
    {
        q = 5000ull + rand.generate64() % 20000ull;
        if (q != p && IsPrime(q)) break;
    } while (true);

	//Their product n = p*q is calculated, which is called the modulus.
	modulus = p * q;

	//The value of the Euler function from the number n is calculated: φ(n) = (p−1)⋅(q−1)
	euler = (p - 1) * (q - 1);

	//An integer e ( 1 < e < φ(n) ) is chosen, coprime with the value of the Euler function (t)
	//open exhibitor
	open_exp = CalculateE(euler);			  //e

	secret_exp = CalculateD(open_exp, euler); //d
}

//miller-rabin test (start)

bool RSA::IsPrime(uint64_t n, uint16_t iter)
{
	if (n < 4) return n == 2 || n == 3;

	uint16_t s = 0;
	uint64_t d = n - 1;
    while ((d & 1) == 0)
    {
		d >>= 1;
		s++;
	}
    for (uint16_t i = 0; i < iter; i++)
    {
		uint32_t a = 2 + rand() % (n - 3);
		if (CheckComposite(n, a, d, s))
			return false;
	}
	return true;
}

bool RSA::CheckComposite(uint64_t n, uint64_t a, uint64_t d, int32_t s) {
	uint64_t x = Binpower(a, d, n);
	if (x == 1 || x == n - 1)
		return false;
    for (int r = 1; r < s; r++)
    {
		x = (uint64_t)x * x % n;
		if (x == n - 1)
			return false;
	}
	return true;
}

//fast exponentiation
uint64_t RSA::Binpower(uint64_t base, uint64_t e, uint64_t mod) {
	uint64_t result = 1;
	base %= mod;
    while (e)
    {
		if (e & 1)
			result = (uint64_t)result * base % mod;
		base = (uint64_t)base * base % mod;
		e >>= 1;
	}
	return result;
}

//miller-rabin test (end)

//open exponent calculation (start)
// Select an integer e ( 1 < e < t ) coprime with the value of the Euler function (t)
uint64_t RSA::CalculateE(uint64_t t) {
	uint64_t e;
	for (e = 2; e < t; e++)
		if (GreatestCommonDivisor(e, t) == 1) 
			return e;

	return -1;
}

uint64_t RSA::GreatestCommonDivisor(uint64_t e, uint64_t t)
{
	uint64_t temp;

    while (e > 0)
    {
		temp = e;
		e = t % e;
		t = temp;
	}
	return t;
}
//open exponent calculation (end)

//Reverse modulo
//Calculate secret exponent (start)
int64_t RSA::CalculateD(int64_t a, int64_t m)
{
	int64_t x, y;
	gcdex(a, m, x, y);
	x = (x % m + m) % m;
	return x;
}

// Extended Euclid algorithm
int64_t RSA::gcdex(int64_t a, int64_t b, int64_t& x, int64_t& y)
{
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	int64_t x1, y1;
	int64_t d = gcdex(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}
//Calculate secret exponent (end)

uint8_t RSA::Encrypt(std::string data)
{
	std::vector<uint64_t> encrypt_hash(SHA_LEN);

	for(size_t i = 0; i < SHA_LEN; ++i)
        encrypt_hash[i] = Binpower(data[i], secret_exp, modulus);

    QString sign = "signed.enc";
	std::ofstream encrypted;
    encrypted.open(sign.toLocal8Bit());

    if (!encrypted.is_open()) return FILE_NOT_OPEN;

    std::copy(encrypt_hash.begin(), encrypt_hash.end(), std::ostream_iterator<uint64_t>(encrypted, " "));
	encrypted.close();
    return OK;
}

std::string RSA::Decipher(QString path)
{
    std::ifstream encrypted(path.toLocal8Bit()); // open file with encrypted hash
	std::vector<uint64_t> encrypted_hash(SHA_LEN); 

	if (encrypted.is_open()) 
		for (size_t i = 0; i < SHA_LEN; ++i)
			encrypted >> encrypted_hash[i];
	encrypted.close();

	//hash decryption
	std::vector<uint8_t> decrypted_hash(SHA_LEN);

    for (size_t i = 0; i < SHA_LEN; ++i)
		decrypted_hash[i] = Binpower(encrypted_hash[i], open_exp, modulus);//----------------------------------

    std::string tmp(decrypted_hash.begin(), decrypted_hash.end());

	return tmp;
}

int8_t RSA::LoadMyKey(uint64_t d, uint64_t n)
{
    modulus = n;
    secret_exp = d;
    return OK;
}

bool RSA::Verification(QString hash_enc, QString file)
{

	uint64_t modulus_tmp = modulus;
	uint64_t open_exp_tmp = open_exp;

    open_exp = pub_key.exp;
    modulus = pub_key.modulus;

    std::string decrypted_hash = Decipher(hash_enc);

    SHA orig;

    std::string hash_= orig.FileHash(file);

	modulus = modulus_tmp;
	open_exp = open_exp_tmp;

	return decrypted_hash == hash_ ? true : false;
}

void RSA::GetPubKey(struct Key* key)
{
    key->modulus = modulus;
    key->exp = open_exp;
}

void RSA::GetPriKey(struct Key* key)
{
    key->modulus = modulus;
    key->exp = secret_exp;
}

void RSA::PrintKeys()
{
    std::cout << "\nRSA public key is (n = " << modulus << ", e = " << open_exp << ")" << std::endl;
    std::cout << "RSA private key is (n = " << modulus << ", d = " << secret_exp << ")" << std::endl;
}

