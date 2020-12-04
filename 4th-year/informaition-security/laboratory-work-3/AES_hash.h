#pragma once

#include "AES.h"

class AES_hash {
    public:
	std::vector<unsigned char> hash(const std::string &file);
};
