#include <fstream>
#include "RC4.h"
#include "AES.h"
#include "iostream"

constexpr size_t BUFF_SIZE = 16;

int main(int argc, char *argv[])
{
	std::string error = "coding-tool [-c (code, default) / -d (decode)]"
			    "-a [rc4(default) / aes128 / aes192 / aes256]"
			    "(-f [file_name] (if aes is chosen))";
	if (argc < 2) {
		std::cerr << error << std::endl;
	}

	bool rc4_chosen = true;
	bool code_chosen = true;
	AES_ALG aes_type = AES128;
	std::string file_name_for_aes;

	int i = 0;
	while (++i < argc) {
		switch (argv[i][1]) {
		case 'd':
			code_chosen = false;
			break;
		case 'a': {
			std::string alg = std::string(argv[++i]);
			rc4_chosen = (alg == "rc4");
			if (!rc4_chosen) {
				if (alg == "aes128") {
					aes_type = AES128;
				} else if (alg == "aes192") {
					aes_type = AES192;
				} else if (alg == "aes256") {
					aes_type = AES256;
				} else {
					std::cerr << error << std::endl;
					return -1;
				}
			}
			break;
		}
		case 'f':
			file_name_for_aes = std::string(argv[++i]);
			break;
		}
	}

	int key_size = 0;
	if (rc4_chosen) {
		std::cout << "Enter a key length(in bytes):" << std::endl;
		std::cin >> key_size;
	} else {
		switch (aes_type) {
		case AES128:
			key_size = 4 * AES128_Nk;
			break;
		case AES192:
			key_size = 4 * AES192_Nk;
			break;
		case AES256:
			key_size = 4 * AES256_Nk;
			break;
		}
	}

	std::cin.unsetf(std::ios::dec);
	std::cin.unsetf(std::ios::oct);
	std::cin.setf(std::ios::hex);
	std::vector<rc4_key_t> key(key_size);
	unsigned tmp = 0;
	std::cout << "Enter key values in bytes. Every value is a digit "
		     "between 0x00 and 0xFF:"
		  << std::endl;
	for (i = 0; i < key_size; i++) {
		std::cin >> std::hex >> tmp;
		key[i] = tmp;
	}

	if (rc4_chosen) {
		RC4 rc4(key);
		if (code_chosen) {
			std::cout
				<< "Enter a text string. Enter Ctrl+D to stop. "
				   "The coded text will be displayed as the digits "
				   "between 0(0x00) and 255(0xFF) for every string:"
				<< std::endl;

			int ch;
			std::vector<unsigned int> coded_text;
			while ((ch = getchar()) != EOF) {
				if (ch != '\n') {
					coded_text.push_back(
						(unsigned int)ch ^
						rc4.get_next_byte());
				} else {
					for (i = 0; i < coded_text.size();
					     i++) {
						std::cout << coded_text[i]
							  << " ";
					}
					std::cout << std::endl;
					coded_text.clear();
				}
			}
			for (i = 0; i < coded_text.size(); i++) {
				std::cout << coded_text[i] << " ";
			}
		} else {
			std::cout
				<< "Enter a coded text string as a sequence "
				   "of the digits between 0(0x00) and 255(0xFF). "
				   "Enter -1 to get current result. Enter -2 to stop:"
				<< std::endl;

			int code = 0;
			std::string text;
			while (true) {
				std::cin >> code;
				if (code >= 0) {
					text += ((char)((unsigned char)code ^
							rc4.get_next_byte()));
				} else {
					std::cout << text << std::endl;
					if (code != -2) {
						text.clear();
					} else {
						break;
					}
				}
			}
		}
	} else {
		AES aes(aes_type, key);

		std::vector<unsigned char> in_buffer(BUFF_SIZE);
		std::vector<unsigned char> out_buffer;

		std::ifstream is(file_name_for_aes, std::ios::binary);
		std::ofstream os(file_name_for_aes + ".output",
				 std::ios::binary);
		if (!is.is_open() || !os.is_open()) {
			std::cerr << "Can't open input or output file!"
				  << std::endl;
			return -1;
		}
		size_t size = 0;
		while (!is.eof()) {
			is.read(reinterpret_cast<char *>(in_buffer.data()),
				BUFF_SIZE);
			size = is.gcount();
			if (size != 0) {
				if (code_chosen) {
					out_buffer = aes.code(in_buffer, size);
				} else {
					out_buffer =
						aes.decode(in_buffer, size);
				}
				os.write(reinterpret_cast<const char *>(
						 out_buffer.data()),
					 BUFF_SIZE);
			}
			if (size != BUFF_SIZE) {
				break;
			}
		}

		is.close();
		os.close();
	}

	return 0;
}
