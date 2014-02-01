#include <lisp/Interpreter.hpp>

#include <functional>


namespace lisp {


List Interpreter::evalIdentifier(const std::string &id, Memory &local)
{
	if (id == "true" || id == "false") {
		return id;
	}

	const List *result;
	if ((result = local.lookup(id)) || (result = global.lookup(id))) {
		return *result;
	}

	throw eval_error("unbound value: " + id);
}


List Interpreter::evalCons(const List &expr, Memory &local)
{
	const List &head = expr.getHead(), &tail = expr.getTail();
	if (!head.isIdentifier()) {
		const List lambda = eval(head, local);
		return evalApply(lambda, tail, local);
	}

	if (head.getIdentifier() == "if") {
		return evalIf(tail, local);
	} else if (head.getIdentifier() == "set") {
		return evalSet(tail, local);
	} else if (head.getIdentifier() == "quote") {
		return evalQuote(tail, local);
	} else if (head.getIdentifier() == "define") {
		return evalDefine(tail, local);
	} else if (head.getIdentifier() == "car") {
		return evalCar(tail, local);
	} else if (head.getIdentifier() == "cdr") {
		return evalCdr(tail, local);
	} else if (head.getIdentifier() == "cons") {
		return evalConstruct(tail, local);
	} else if (head.getIdentifier() == "nil?") {
		return evalIsNil(tail, local);
	} else if (head.getIdentifier() == "+") {
		return evalNumeric<std::plus<int>>(tail, local);
	} else if (head.getIdentifier() == "-") {
		return evalNumeric<std::minus<int>>(tail, local);
	} else if (head.getIdentifier() == "*") {
		return evalNumeric<std::multiplies<int>>(tail, local);
	} else if (head.getIdentifier() == "=") {
		return evalRelation<std::equal_to<int>>(tail, local);
	} else if (head.getIdentifier() == "<") {
		return evalRelation<std::less<int>>(tail, local);
	} else if (head.getIdentifier() == "<=") {
		return evalRelation<std::less_equal<int>>(tail, local);
	} else if (head.getIdentifier() == ">") {
		return evalRelation<std::greater<int>>(tail, local);
	} else if (head.getIdentifier() == ">=") {
		return evalRelation<std::greater_equal<int>>(tail, local);
	} else {
		const List lambda = evalIdentifier(head.getIdentifier(), local);
		return evalApply(lambda, tail, local);
	}
}


List Interpreter::evalCar(const List &expr, Memory &local)
{
	if (!expr.isCons() ||
	    !expr.getTail().isNil() ||
	    !expr.getHead().isCons())
	{
		throw eval_error("incorrect car syntax or arguments");
	}

	List result = eval(expr.getHead(), local);
	return result.getHead();
}


List Interpreter::evalCdr(const List &expr, Memory &local)
{
	if (!expr.isCons() ||
	    !expr.getTail().isNil() ||
	    !expr.getHead().isCons())
	{
		throw eval_error("incorrect cdr syntax or arguments");
	}

	List result = eval(expr.getHead(), local);
	return result.getTail();
}


List Interpreter::evalConstruct(const List &expr, Memory &local)
{
	if (!expr.isCons() ||
	    !expr.getTail().isCons() ||
	    !expr.getTail().getTail().isNil())
	{
		throw eval_error("cons takes exactly 2 arguments");
	}

	List head = eval(expr.getHead(), local);
	List tail = eval(expr.getTail().getHead(), local);

	if (!tail.isCons()) {
		throw eval_error("cons second argument not a list");
	}

	return List(head, tail);
}


List Interpreter::evalIsNil(const List &expr, Memory &local)
{
	if (!expr.isCons() ||
	    !expr.getTail().isNil())
	{
		throw eval_error("nil takes exactly one argument");
	}

	List result = eval(expr.getHead(), local);
	return result.isNil() ? List("true") : List("false");
}


List Interpreter::evalIf(const List &expr, Memory &local)
{
	if (!expr.isCons() || !expr.getTail().isCons()) {
		throw eval_error("incorrect if syntax");
	}
	List cond = eval(expr.getHead(), local);

	if (cond.isIdentifier() && cond.getIdentifier() == "true") {
		return eval(expr.getTail().getHead(), local);
	} else {
		return eval(expr.getTail().getTail().getHead(), local);
	}
}


List Interpreter::evalSet(const List &expr, Memory &local)
{
	if (!expr.isCons() || !expr.getHead().isIdentifier()) {
		throw eval_error("incorrect set syntax");
	}

	List result = eval(expr.getTail().getHead(), local);
	global.set(expr.getHead().getIdentifier(), result);
	return List("nil");
}


List Interpreter::evalQuote(const List &expr, Memory &local)
{
	if (!expr.isCons() || !expr.getTail().isNil()) {
		throw eval_error("quote takes exactly one argument");
	}

	return expr.getHead();
}


List Interpreter::evalDefine(const List &expr, Memory &local)
{
	if (!expr.isCons() ||
	    !expr.getHead().isIdentifier() ||
	    !expr.getTail().isCons() ||
	    !expr.getTail().getTail().isCons() ||
	    !expr.getTail().getTail().getTail().isNil())
	{
		throw eval_error("incorrect define syntax");	
	}

	const auto &name = expr.getHead().getIdentifier();
	const auto &args = expr.getTail().getHead();
	const auto &body = expr.getTail().getTail().getHead();
	global.set(name, List({ List("lambda"), args, body }));

	return List("nil");
}


List Interpreter::evalApply(const List &lambda, const List &args, Memory &local)
{
	if (!lambda.isCons() ||
	    !lambda.getHead().isIdentifier() ||
	    !(lambda.getHead().getIdentifier() == "lambda") ||
	    !lambda.getTail().isCons() ||
	    !lambda.getTail().getTail().isCons() ||
	    !lambda.getTail().getTail().getTail().isNil())
	{
		throw eval_error("incorrect lambda syntax");
	}

	auto body = lambda.getTail().getTail().getHead();
	auto var = lambda.getTail().getHead();
	auto arg = args;

	local.save();
	while (!var.isNil() && !arg.isNil()) {
		if (!var.isCons() || !var.getHead().isIdentifier()) {
			throw eval_error("incorrect lambda arguments syntax");
		}
		if (!arg.isCons()) {
			throw eval_error("invalid lambda argument");
		}
		List earg = eval(arg.getHead(), local);
		local.set(var.getHead().getIdentifier(), earg);

		var = var.getTail();
		arg = arg.getTail();
	}
	auto result = eval(body, local);
	local.restore();

	return result;
}


List Interpreter::eval(const List &list, Memory &local)
{
	switch (list.getType()) {
	case List::Type::number:
		return list;
	case List::Type::identifier:
		return evalIdentifier(list.getIdentifier(), local);
	case List::Type::cons:
		return evalCons(list, local);
	}
}


List Interpreter::eval(
	const List &list
)
{
	Memory local;
	return eval(list, local);
}


}
