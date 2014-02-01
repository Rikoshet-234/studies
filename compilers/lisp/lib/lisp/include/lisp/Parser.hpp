#ifndef FILE_LISP_PARSER_HPP
#define FILE_LISP_PARSER_HPP


#include <istream>
#include <stdexcept>

#include <lisp/Lexer.hpp>
#include <lisp/List.hpp>


namespace lisp {


class parse_error : public std::runtime_error {
public:
	parse_error(const std::string &what)
	: std::runtime_error(what)
	{
	}
};


class Parser {
private:
	List parseArgs(Lexer::const_iterator &it);
	List parseList(Lexer::const_iterator &it);

public:
	List parse(Lexer::const_iterator &it);
	List parse(std::istream &is);
};


}


#endif
