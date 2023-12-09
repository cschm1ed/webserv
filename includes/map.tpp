
#ifndef MAP_TPP
#define MAP_TPP

#include <map>

template<typename T, typename K>
void printMap(std::map<T, K> map) {
	if (map.empty())
		return;
	typename std::map<T, K>::const_iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		std::cout << "key: '" << it->first << "', value: '" << it->second << "'\n";
	}
}

#endif
