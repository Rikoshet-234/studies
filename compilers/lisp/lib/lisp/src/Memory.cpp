#include <lisp/Memory.hpp>


namespace lisp {


const List *Memory::lookup(const std::string &id) const
{
	auto it = values.find(id);
	return it != values.end() ? &(it->second.top()) : nullptr;
}


void Memory::set(const std::string &id, const List &list)
{
	values[id].push(list);
	closure.push(id);
}


void Memory::save()
{
	restpoints.push(closure.size());
}


void Memory::restore()
{
	while (closure.size() > restpoints.top()) {
		auto it = values.find(closure.top());
		auto &stack = it->second;
		closure.pop();
		stack.pop();

		if (stack.empty()) {
			values.erase(it);
		}
	}
	restpoints.pop();
}


}
