#pragma once

#define DEBUG

#include <vector>
#include <openssl/sha.h>
#include <string>

#ifdef DEBUG
#include <fstream>
#endif

using node_id               = std::vector<unsigned char>;
using value_hash            = std::vector<unsigned char>;
using value_hash_or_node_id = std::vector<unsigned char>;

bool operator<(const value_hash_or_node_id &l, const value_hash_or_node_id &r);

value_hash_or_node_id operator-(const value_hash_or_node_id &l, const value_hash_or_node_id &r);

value_hash calc_hash(const std::string &seed);

#ifdef DEBUG
void dump_hash(std::ostream &s, const std::string &str, const value_hash_or_node_id &data);
#endif
