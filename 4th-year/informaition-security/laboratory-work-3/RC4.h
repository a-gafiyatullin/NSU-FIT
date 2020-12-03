#pragma once
#include <memory>
#include <exception>
#include <cstring>
#include <vector>

typedef unsigned char rc4_key_t;

class RC4 {
    private:
	std::vector<rc4_key_t> key;

	int x, y;

	static const int S_BLOCK_LEN = 256;
	rc4_key_t S[S_BLOCK_LEN];

    public:
	explicit RC4(const std::vector<rc4_key_t> &key);

	rc4_key_t get_next_byte();
};
