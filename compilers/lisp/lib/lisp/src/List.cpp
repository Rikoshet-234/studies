#include <lisp/List.hpp>

#include <vector>


namespace lisp {


List::List()
: node(std::make_shared<Identifier>("nil"))
{
}


List::List(int num)
: node(std::make_shared<Number>(num))
{
}


List::List(const std::string &str)
: node(std::make_shared<Identifier>(str))
{
}


List::List(const List &head, const List &tail)
: node(std::make_shared<Cons>(head, tail))
{
}


List::List(std::initializer_list<List> lists)
{
	// As of C++11 initializer list has no reverse iterator :(
	std::vector<List> elements(lists);
	List list("nil");

	for (size_t i = elements.size() - 1; i > 0; i--) {
		list = List(elements[i], list);
	}

	// Looks hacky but required (no virtual list constructor).
	if (elements.empty()) {
		node = std::make_shared<Identifier>("nil");
	} else {
		node = std::make_shared<Cons>(elements[0], list);
	}
}


List::Number::Number(int num)
: num(num)
{
	type = List::Type::number;
}


List::Identifier::Identifier(const std::string &id)
: id(id)
{
	type = List::Type::identifier;
}


List::Cons::Cons(const List &head, const List &tail)
: head(head), tail(tail)
{
	type = List::Type::cons;
}


std::ostream &operator <<(std::ostream &os, const List &list)
{
	switch (list.node->type) {
	case List::Type::number:
		os << list.getNumber();
		break;
	case List::Type::identifier:
		os << list.getIdentifier();
		break;
	case List::Type::cons:
		os << "(";
		auto it = list;
		bool space = false;
		while (it.getType() == List::Type::cons) {
			os << (space ? " " : "") << it.getHead();
			it = it.getTail();
			space = true;
		}

		if (it.getType() == List::Type::identifier &&
		    it.getIdentifier() == "nil")
		{
			os << ")";
		} else {
			os << (space ? " " : "") << it << ")";
		}
	}

	return os;
}


}
