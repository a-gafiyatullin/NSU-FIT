#pragma once

#include "MTF_Stack.h"
#include <vector>
#include <algorithm>

template<class T>
class MTF_Message {
private:
	std::vector<size_t> message;
	std::vector<T> alphabet;	// for decoding

public:
	MTF_Message(const std::vector<size_t>& message, const std::vector<T>& alphabet);

	std::vector<size_t> get_message() const;

	std::vector<T> get_alphabet() const;
};

template<class T>
class MTF {
private:
	MTF() {}

	/* create MTF_Stack from alphaet */
	static MTF_Stack<T> create_mtf_stack(const std::vector<T>& alphabet);

public:

	/* code a user message to sequence of digits */
	static MTF_Message<T> code_message(const std::vector<T>& message, const std::vector<T>& alphabet);

	/* decode to user message */
	static std::vector<T> decode_message(const MTF_Message<T>& message);
};

template<class T>
inline MTF_Stack<T> MTF<T>::create_mtf_stack(const std::vector<T>& alphabet) {
	MTF_Stack<T> stack;

	for (auto element : alphabet) {
		stack.add_to_tail(element);
	}

	return stack;
}

template<class T>
inline MTF_Message<T> MTF<T>::code_message(const std::vector<T>& message, const std::vector<T>& alphabet) {
	MTF_Stack<T> stack = create_mtf_stack(alphabet);
	std::vector<size_t> coded_message;

	for (auto& element : message) {
		coded_message.push_back(stack.get_and_move_to_head(element));
	}

	return MTF_Message<T>(coded_message, alphabet);
}

template<class T>
inline std::vector<T> MTF<T>::decode_message(const MTF_Message<T>& message) {
	MTF_Stack<T> stack = create_mtf_stack(message.get_alphabet());
	std::vector<T> decoded_message;

	for (auto& element : message.get_message()) {
		 decoded_message.push_back(stack.get_and_move_to_head(element));
	}

	return decoded_message;
}

template<class T>
inline MTF_Message<T>::MTF_Message(const std::vector<size_t>& message, const std::vector<T>& alphabet)
	: message(message)
	, alphabet(alphabet) {
}

template<class T>
inline std::vector<size_t> MTF_Message<T>::get_message() const {
	return message;
}

template<class T>
inline std::vector<T> MTF_Message<T>::get_alphabet() const {
	return alphabet;
}
