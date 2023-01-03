#include "MTF.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include "../tinyxml/tinyxml.h"

std::vector<std::string> split(std::string strToSplit, char delimeter) {
	std::stringstream ss(strToSplit);
	std::string item;
	std::vector<std::string> splittedStrings;

	while (std::getline(ss, item, delimeter)) {
		splittedStrings.push_back(item);
	}

	return splittedStrings;
}

int main(int argc, char *argv[]) {

	if (argc < 3) {
		std::cerr << "usage: mtf [-c (code) /-d (decode)] [path_to_file_with_message]" << std::endl;
		return -1;
	}

	TiXmlDocument message_file(argv[2]);
	message_file.LoadFile();

	auto root_attr = message_file.RootElement();
	if (root_attr) {
		switch (argv[1][1]) {
		case 'c': {
				std::string message, alphabet;

				auto message_attr = root_attr->FirstChildElement("message");
				if (message_attr) {
					message = std::string(message_attr->GetText());

				}
				else {
					std::cerr << "Error while getting message!" << std::endl;
					return -1;
				}
				auto alphabet_attr = root_attr->FirstChildElement("alphabet");
				if (alphabet_attr) {
					alphabet = std::string(alphabet_attr->GetText());
				}
				else {
					std::cerr << "Error while getting alphabet!" << std::endl;
					return -1;
				}

				auto coded_message = MTF<char>::code_message(std::vector<char>(message.begin(), message.end()), 
					std::vector<char>(alphabet.begin(), alphabet.end()));
				auto message_for_output = coded_message.get_message();
				auto alphabet_for_output = coded_message.get_alphabet();

				std::cout << "Coded message: ";
				std::copy(message_for_output.begin(), message_for_output.end(), std::ostream_iterator<size_t>(std::cout, " "));
				std::cout << std::endl << "Alphabet: ";
				std::copy(alphabet_for_output.begin(), alphabet_for_output.end(), std::ostream_iterator<char>(std::cout));
			}
			break;
		case 'd': {
				std::vector<std::string> coded_message;
				std::string alphabet;

				auto message_attr = root_attr->FirstChildElement("message");
				if (message_attr) {
					coded_message = split(std::string(message_attr->GetText()), ' ');
				}
				else {
					std::cerr << "Error while getting message!" << std::endl;
					return -1;
				}
				auto alphabet_attr = root_attr->FirstChildElement("alphabet");
				if (alphabet_attr) {
					alphabet = std::string(alphabet_attr->GetText());
				}
				else {
					std::cerr << "Error while getting alphabet!" << std::endl;
					return -1;
				}

				std::vector<size_t> coded_message_for_mtf;
				std::for_each(coded_message.begin(), coded_message.end(),
					[&coded_message_for_mtf](std::string number) { coded_message_for_mtf.push_back(std::stoi(number)); });

				MTF_Message<char> message(coded_message_for_mtf, std::vector<char>(alphabet.begin(), alphabet.end()));
				auto decoded_message = MTF<char>::decode_message(message);

				std::cout << "Decoded message: ";
				std::copy(decoded_message.begin(), decoded_message.end(), std::ostream_iterator<char>(std::cout));
			}
			break;
		default:
			std::cerr << "No such option!" << std::endl;
		}
	}
	else {
		std::cerr << "Can't open file!" << std::endl;
		return -1;
	}

	return 0;
}