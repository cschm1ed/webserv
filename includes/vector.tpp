

#ifndef VECTOR_TPP
#define VECTOR_TPP

#include <webserv.hpp>

template<typename T>
void printVector(std::vector<T> & vec) {
	typename std::vector<T>::const_iterator it;

	for (it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << "\n";
	}
}

#endif
