#ifndef FILE_LISP_LIST_HPP
#define FILE_LISP_LIST_HPP


#include <ostream>
#include <memory>
#include <string>
#include <stdexcept>
using std::shared_ptr;


namespace lisp {


class List {
public:
	enum class Type { cons, identifier, number };

private:	
	class Node {
	public:
		List::Type type;

		virtual const List &getHead() const {
			throw std::runtime_error("head not implemented");
		}
		
		virtual const List &getTail() const {
			throw std::runtime_error("tail not implemented");
		}

		virtual const std::string &getIdentifier() const {
			throw std::runtime_error("id not implemented");
		}
		
		virtual const int &getNumber() const {
			throw std::runtime_error("num not implemented");
		}
	};

	class Number;
	class Identifier;
	class Cons;

	shared_ptr<Node> node;

public:
	List();
	List(int num);
	List(const std::string &id);
	List(const List &head, const List &tail);
	List(std::initializer_list<List> lists);

	const Type &getType() const {
		return node->type;
	}

	const bool isIdentifier() const {
		return getType() == List::Type::identifier;
	}

	const bool isNumber() const {
		return getType() == List::Type::number;
	}

	const bool isCons() const {
		return getType() == List::Type::cons;
	}

	const bool isNil() const {
		return isIdentifier() && getIdentifier() == "nil";
	}

	const List &getHead() const {
		return node->getHead();
	}

	const List &getTail() const {
		return node->getTail();
	}

	const int &getNumber() const {
		return node->getNumber();
	}

	const std::string &getIdentifier() const {
		return node->getIdentifier();
	}

	friend std::ostream &operator <<(std::ostream &os, const List &list);
};


class List::Number : public List::Node {
private:
	int num;
public:
	Number(int number);

	virtual const int &getNumber() const override {
		return num;
	}
};


class List::Identifier : public List::Node {
private:
	std::string id;
public:
	Identifier(const std::string &id);

	virtual const std::string &getIdentifier() const override {
		return id;
	}
};


class List::Cons : public List::Node {
private:
	List head, tail;
public:
	Cons(const List &head, const List &tail);

	virtual const List &getHead() const override {
		return head;
	}

	virtual const List &getTail() const override {
		return tail;
	}
};


}


#endif
