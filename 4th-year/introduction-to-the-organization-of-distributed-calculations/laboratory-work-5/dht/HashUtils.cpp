#include "HashUtils.h"

bool operator<(const value_hash_or_node_id &l, const value_hash_or_node_id &r) {
  for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    if(l[i] != r[i]) {
      return l[i] < r[i];
    }
  }

  return false;
}

value_hash_or_node_id operator-(const value_hash_or_node_id &l, const value_hash_or_node_id &r) {
  value_hash_or_node_id res(SHA_DIGEST_LENGTH);

  int carry = 0;
  for(int i = SHA_DIGEST_LENGTH - 1; i >= 0; i--) {
    if(l[i] == 0 || (l[i] - carry) < r[i]) {
      res[i] = (unsigned char)(((unsigned int)(1 << 9) | l[i]) - carry - r[i]);
      carry = 1;
    } else {
      res[i] = l[i] - r[i];
      carry = 0;
    }
  }

  return res;
}

value_hash calc_hash(const std::string &seed) {
  node_id id(SHA_DIGEST_LENGTH);
  SHA1((const unsigned char *)seed.data(), seed.length(), id.data());

  return id;
}
