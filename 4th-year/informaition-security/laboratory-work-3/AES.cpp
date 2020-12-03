#include "AES.h"

unsigned char AES::galois_mul(unsigned char a, unsigned char b)
{
	unsigned char p = 0;

	for (int counter = 0; counter < 8; counter++) {
		if ((b & 1) != 0) {
			p ^= a;
		}
		bool hi_bit_set = (a & 0x80) != 0;
		a <<= 1;
		if (hi_bit_set) {
			a ^= 0x1B;
		}
		b >>= 1;
	}

	return p;
}

void AES::shift_rows(std::vector<unsigned char> &state)
{
	unsigned char tmp[Nb] = { 0 };
	for (int r = 1; r < Nb; r++) {
		int i;
		for (i = 0; i < r; i++) {
			tmp[i] = state[r * Nb + i];
		}
		for (i = 0; i < Nb - r; i++) {
			state[r * Nb + i] = state[r * Nb + i + r];
		}
		for (int j = 0; j < r; j++) {
			state[r * Nb + i + j] = tmp[j];
		}
	}
}

void AES::inv_shift_rows(std::vector<unsigned char> &state)
{
	unsigned char tmp[Nb] = { 0 };
	for (int r = 1; r < Nb; r++) {
		int i;
		for (i = 0; i < r; i++) {
			tmp[r - i - 1] = state[(r + 1) * Nb - i - 1];
		}
		for (i = Nb - r - 1; i >= 0; i--) {
			state[r * Nb + i + r] = state[r * Nb + i];
		}
		for (int j = 0; j < r; j++) {
			state[r * Nb + j] = tmp[j];
		}
	}
}

void AES::mix_columns(std::vector<unsigned char> &state,
		      const unsigned char polynomial[Nb])
{
	for (int i = 0; i < Nb; i++) {
		unsigned char d0 =
			galois_mul(polynomial[3], state[i]) ^
			galois_mul(polynomial[2], state[3 * Nb + i]) ^
			galois_mul(polynomial[1], state[2 * Nb + i]) ^
			galois_mul(polynomial[0], state[Nb + i]);

		unsigned char d1 =
			galois_mul(polynomial[3], state[Nb + i]) ^
			galois_mul(polynomial[2], state[i]) ^
			galois_mul(polynomial[1], state[3 * Nb + i]) ^
			galois_mul(polynomial[0], state[2 * Nb + i]);

		unsigned char d2 =
			galois_mul(polynomial[3], state[2 * Nb + i]) ^
			galois_mul(polynomial[2], state[Nb + i]) ^
			galois_mul(polynomial[1], state[i]) ^
			galois_mul(polynomial[0], state[3 * Nb + i]);

		unsigned char d3 =
			galois_mul(polynomial[3], state[3 * Nb + i]) ^
			galois_mul(polynomial[2], state[2 * Nb + i]) ^
			galois_mul(polynomial[1], state[Nb + i]) ^
			galois_mul(polynomial[0], state[i]);

		state[i] = d0;
		state[Nb + i] = d1;
		state[2 * Nb + i] = d2;
		state[3 * Nb + i] = d3;
	}
}

void AES::add_round_key(
	std::vector<unsigned char> &state,
	std::vector<unsigned char>::const_iterator round_key_iter)
{
	for (int i = 0; i < Nb; i++) {
		for (int j = 0; j < Nb; j++) {
			state[j * Nb + i] ^= *(round_key_iter + (i * Nb + j));
		}
	}
}

void AES::gen_round_keys(const std::vector<unsigned char> &cipher_key)
{
	// copy user key
	round_keys.assign(cipher_key.begin(), cipher_key.end());

	int i = Nk;
	while (i < Nb * (Nr + 1)) {
		round_keys.insert(round_keys.end(), round_keys.end() - Nb,
				  round_keys.end());
		if (i % Nk == 0) {
			// shift the word, apply Sbox and add Round constant
			unsigned char temp_byte = round_keys[i * Nb];
			for (int j = 0; j < Nb - 1; j++) {
				round_keys[i * Nb + j] =
					sub_byte(round_keys[i * Nb + j + 1],
						 Sbox) ^
					Rcon[i / Nk][j];
			}
			round_keys[(i + 1) * Nb - 1] =
				sub_byte(temp_byte, Sbox) ^
				Rcon[i / Nk][Nb - 1];
		} else if (Nk > AES192_Nk && (i - Nb) % Nk == 0) {
			// apply Sbox
			std::for_each(round_keys.end() - Nb, round_keys.end(),
				      [](unsigned char &ch) {
					      ch = sub_byte(ch, Sbox);
				      });
		}

		for (int j = 0; j < Nb; j++) {
			round_keys[i * Nb + j] = round_keys[i * Nb + j] ^
						 round_keys[(i - Nk) * Nb + j];
		}
		i++;
	}
}

void AES::sub_bytes(
	std::vector<unsigned char> &state,
	const unsigned char box[SBOX_ROW_COL_SIZE * SBOX_ROW_COL_SIZE])
{
	std::for_each(state.begin(), state.begin() + Nb * Nb,
		      [&](unsigned char &byte) { byte = sub_byte(byte, box); });
}

AES::AES(const AES_ALG &alg_type, const std::vector<unsigned char> &cipher_key)
{
	switch (alg_type) {
	case AES128:
		Nk = AES128_Nk;
		Nr = AES128_Nr;
		break;
	case AES192:
		Nk = AES192_Nk;
		Nr = AES192_Nr;
		break;
	case AES256:
		Nk = AES256_Nk;
		Nr = AES256_Nr;
		break;
	default:
		throw std::logic_error("No such AES type!");
	}

	gen_round_keys(cipher_key);
}

std::vector<unsigned char> AES::code(std::vector<unsigned char> state,
				     const size_t &size)
{
	if (size != Nb * Nb) {
		std::fill(state.begin() + size, state.end(), 0);
	}

	add_round_key(state, round_keys.begin());

	for (int i = 1; i < Nr; i++) {
		sub_bytes(state, Sbox);
		shift_rows(state);
		mix_columns(state, cx);
		add_round_key(state, round_keys.begin() + i * Nb * Nb);
	}

	sub_bytes(state, Sbox);
	shift_rows(state);
	add_round_key(state, round_keys.begin() + Nb * Nb * Nr);

	return state;
}

std::vector<unsigned char> AES::decode(std::vector<unsigned char> state,
				       const size_t &size)
{
	add_round_key(state, round_keys.begin() + Nb * Nb * Nr);

	for (int i = Nr - 1; i > 0; i--) {
		inv_shift_rows(state);
		sub_bytes(state, InvSbox);
		add_round_key(state, round_keys.begin() + i * Nb * Nb);
		mix_columns(state, inv_cx);
	}

	inv_shift_rows(state);
	sub_bytes(state, InvSbox);
	add_round_key(state, round_keys.begin());

	return state;
}
