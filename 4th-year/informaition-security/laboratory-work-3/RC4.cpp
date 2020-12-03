#include "RC4.h"

RC4::RC4(const std::vector<rc4_key_t> &key) : x(0), y(0), key(key)
{
	// initialize S block 0x00 to 0xFF
	for (int i = 0; i < S_BLOCK_LEN; i++) {
		S[i] = i;
	}

	// KSA
	int j = 0;
	for (int i = 0; i < S_BLOCK_LEN; i++) {
		j = (j + S[i] + this->key[i % this->key.size()]) % S_BLOCK_LEN;
		std::swap(S[i], S[j]);
	}
}

rc4_key_t RC4::get_next_byte()
{
	// Pseudo-Random Generation Algorithm
	x = (x + 1) % S_BLOCK_LEN;
	y = (y + S[x]) % S_BLOCK_LEN;

	std::swap(S[x], S[y]);

	return S[(S[x] + S[y]) % S_BLOCK_LEN];
}
