#ifndef FILE_LISP_INTERPRETER_HPP
#define FILE_LISP_INTERPRETER_HPP


#include <iostream>
#include <string>
#include <stdexcept>

#include <lisp/List.hpp>
#include <lisp/Memory.hpp>


namespace lisp {


class eval_error : public std::runtime_error {
public:
	eval_error(const std::string &what)
	: std::runtime_error(what)
	{
	}
};


class Interpreter {
public:
	List eval(const List &list, Memory &local);
	List eval(const List &list);

private:
	Memory global;

	List evalIdentifier(const std::string &id, Memory &local);
	List evalCons(const List &expr, Memory &local);
	List evalApply(const List &lambda, const List &args, Memory &local);

	// Language constructs.
	List evalIf(const List &expr, Memory &local);
	List evalSet(const List &expr, Memory &local);
	List evalQuote(const List &expr, Memory &local);
	List evalDefine(const List &expr, Memory &local);

	// List primitives.
	List evalCar(const List &expr, Memory &local);
	List evalCdr(const List &expr, Memory &local);
	List evalConstruct(const List &expr, Memory &local);
	List evalIsNil(const List &expr, Memory &local);

	template <typename BinaryOperator>
	List evalNumeric(const List &expr, Memory &local) {
		if (!expr.isCons()) {
			throw eval_error("invalid syntax for numeric expression");
		}

		auto it = eval(expr.getHead(), local);
		if (!it.isNumber()) {
			throw eval_error("non-numeric operands for binary operator");
		}

		auto result = it.getNumber();		
		for (it = expr.getTail(); !it.isNil(); it = it.getTail()) {
			if (!it.isCons()) {
				throw eval_error("invalid arguments for numeric expression");
			}

			auto value = eval(it.getHead(), local);
			if (!value.isNumber()) {
				throw eval_error("non-numeric operands for binary oberator");
			}

			result = BinaryOperator()(result, value.getNumber());
		}

		return result;
	}

	template <typename BinaryOperator>
	List evalRelation(const List &expr, Memory &local) {
		if (!expr.isCons()) {
			throw eval_error("invalid syntax for relational expression");
		}

		auto prev = eval(expr.getHead(), local);
		if (!prev.isNumber()) {
			throw eval_error("non-numeric operands for binnary operator");
		}

		for (auto it = expr.getTail(); !it.isNil(); it = it.getTail()) {
			if (!it.isCons()) {
				throw eval_error("invalid syntax for relational expression");
			}

			auto curr = eval(it.getHead(), local);
			if (!curr.isNumber()) {
				throw eval_error("non-numeric operands for binary operator");
			}

			if (!(BinaryOperator()(prev.getNumber(), curr.getNumber()))) {
				return List("false");
			}

			prev = curr;
		}

		return List("true");
	}
};


}


#endif
