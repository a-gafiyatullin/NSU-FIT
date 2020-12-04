#include "AES_hash.h"

std::vector<unsigned char> AES_hash::hash(const std::string &file)
{
	std::vector<unsigned char> in_buffer(AES::BUFF_SIZE);

	// open file
	std::ifstream is(file, std::ios::binary);
	if (!is.is_open()) {
		return {};
	}

	std::vector<unsigned char> key(AES128_Nk * 4);
	std::fill(key.begin(), key.end(), 0);

	size_t size;
	while (!is.eof()) {
		// use result of he previous iteration as new key
		AES aes(AES128, key);
		is.read(reinterpret_cast<char *>(in_buffer.data()),
			AES::BUFF_SIZE);
		size = is.gcount();
		if (size != 0) {
			key = aes.code(in_buffer, size);
		}
		if (size != AES::BUFF_SIZE) {
			break;
		}
	}

	is.close();
	return key;
}
