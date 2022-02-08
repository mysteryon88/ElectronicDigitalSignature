#include "RSA.h"

RSA::RSA() {
	privat_key = 0ull;
	public_key = 0ull;
}

RSA::~RSA() {}

void RSA::GenerateKey() {
	uint64_t p = 0ull, q = 0ull;
	uint64_t modulus = 0ull, euler = 0ull, open_exp = 0ull, secret_exp = 0ull;
	std::random_device rd;
	std::mt19937 mersenne(rd()); // initialize the Mersenne Twister with a random starting number
	//generate p
	do {
		p = 2000ull + mersenne() % 40000;
		if (IsPrime(p)) break;
	} while (true);
	//generate q
	do {
		q = 2000ull + mersenne() % 40000;
		if (IsPrime(q)) break;
	} while (true);
	//p = 3557;
	//q = 2579;
	//Their product n = p*q is calculated, which is called the modulus.
	modulus = p * q;

	//The value of the Euler function from the number n is calculated: φ(n) = (p−1)⋅(q−1)
	euler = (p - 1) * (q - 1);

	//An integer e ( 1 < e < φ(n) ) is chosen, coprime with the value of the Euler function (t)
	//open exhibitor
	open_exp = CalculateE(euler);			  //e

	secret_exp = CalculateD(open_exp, euler); //d

	//Пара {e, n} публикуется в качестве открытого ключа RSA
	std::cout << "\nRSA public key is (n = " << modulus << ", e = " << open_exp << ")" << std::endl;

	//Пара {d, n} играет роль закрытого ключа RSA и держится в секрете
	std::cout << "RSA private key is (n = " << modulus << ", d = " << secret_exp << ")" << std::endl;
}

//miller-rabin test (start)

bool RSA::IsPrime(uint64_t n, unsigned short iter) {
	if (n < 4) return n == 2 || n == 3;

	unsigned short s = 0;
	uint64_t d = n - 1;
	while ((d & 1) == 0) {
		d >>= 1;
		s++;
	}
	for (unsigned short i = 0; i < iter; i++) {
		int a = 2 + rand() % (n - 3);
		if (check_composite(n, a, d, s))
			return false;
	}
	return true;
}

bool RSA::check_composite(uint64_t n, uint64_t a, uint64_t d, int s) {
	uint64_t x = binpower(a, d, n);
	if (x == 1 || x == n - 1)
		return false;
	for (int r = 1; r < s; r++) {
		x = (uint64_t)x * x % n;
		if (x == n - 1)
			return false;
	}
	return true;
}

uint64_t RSA::binpower(uint64_t base, uint64_t e, uint64_t mod) {
	uint64_t result = 1;
	base %= mod;
	while (e) {
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

uint64_t RSA::GreatestCommonDivisor(uint64_t e, uint64_t t) {
	uint64_t temp;

	while (e > 0){
		temp = e;
		e = t % e;
		t = temp;
	}
	return t;
}
//open exponent calculation (end)

//Reverse modulo
//Calculate secret exponent (start)
int64_t RSA::CalculateD(int64_t a, int64_t m) {
	int64_t x, y;
	gcdex(a, m, x, y);
	x = (x % m + m) % m;
	return x;
}

// Extended Euclid algorithm
int64_t RSA::gcdex(int64_t a, int64_t b, int64_t& x, int64_t& y) {
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
