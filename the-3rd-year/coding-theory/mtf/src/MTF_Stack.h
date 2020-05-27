#pragma once

#include <list>
#include <stdexcept>
template<class T>
class MTF_Stack {
private:
	std::list<T> mtf_stack;

public:
	/* add new element to tail of the special stack */
	void add_to_tail(const T &element);

	/* get ordering number of the element in the stack
	   and move it to the head of the stack */
	size_t get_and_move_to_head(const T& element);

	/* get element by ordering number of the element in the stack
		and move element to the head of the stack */
	T get_and_move_to_head(const size_t& order);
};

template<class T>
inline void MTF_Stack<T>::add_to_tail(const T& element) {
	mtf_stack.push_back(element);
}

template<class T>
inline size_t MTF_Stack<T>::get_and_move_to_head(const T& element) {
	auto iter = mtf_stack.begin();
	size_t order = 0;

	for (; iter != mtf_stack.end(); iter++) {
		if (*iter == element) {
			break;
		}
		order++;
	}

	if (iter == mtf_stack.end()) {
		throw std::out_of_range("No such element in MTF_Stack!");
	}

	mtf_stack.erase(iter);
	mtf_stack.push_front(element);

	return order;
}

template<class T>
inline T MTF_Stack<T>::get_and_move_to_head(const size_t& order) {
	if (order > mtf_stack.size()) {
		throw std::out_of_range("No such element in MTF_Stack!");
	}

	auto iter = mtf_stack.begin();
	std::advance(iter, order);
	auto element = *iter;

	mtf_stack.erase(iter);
	mtf_stack.push_front(element);

	return element;
}