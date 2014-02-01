#include <lisp/Parser.hpp>

#include <stdexcept>


namespace lisp {


List Parser::parseArgs(Lexer::const_iterator &it)
{
	if (it->type == Token::Type::rpar) {
		return List("nil");
	}

	List head = parseList(it);
	List tail = parseArgs(it);
	return List(head, tail);
}


List Parser::parseList(Lexer::const_iterator &it)
{
	switch (it->type) {
	case Token::Type::identifier:
		return List((it++)->string);
	case Token::Type::number:
		return List((it++)->number);
	case Token::Type::lpar: {
		it++;
		List result = parseArgs(it);
		it++;
		return result;
	}
	default:
		throw parse_error("expected an atom or a list");
	}
}


List Parser::parse(Lexer::const_iterator &it)
{
	return parseList(it);
}


List Parser::parse(std::istream &is)
{
	Lexer lexer(is);
	Lexer::const_iterator it = lexer.cbegin();
	return parse(it);
}


}
