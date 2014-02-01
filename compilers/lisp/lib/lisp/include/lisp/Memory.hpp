#ifndef FILE_LISP_MEMORY_HPP
#define FILE_LISP_MEMORY_HPP

#include <vector>
#include <stack>
#include <unordered_map>

#include <lisp/List.hpp>


namespace lisp {


class Memory {
private:
	std::unordered_map<std::string, std::stack<List>> values;
	std::stack<std::string> closure;
	std::stack<size_t> restpoints;

public:
	const List *lookup(const std::string &id) const;
	void set(const std::string &id, const List &list);

	void save();
	void restore();
};


}


#endif
