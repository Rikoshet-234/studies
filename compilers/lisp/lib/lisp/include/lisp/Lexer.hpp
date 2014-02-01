#ifndef FILE_LEXER_HPP
#define FILE_LEXER_HPP


#include <iostream>
#include <string>
#include <vector>
#include <queue>


struct Token {
	enum class Type {
		lpar, rpar, identifier, number, eof, unknown
	} type;

	std::string string;
	int number;
};

std::ostream &operator <<(std::ostream &out, const Token &token);


class Lexer {
private:
	typedef std::vector<Token> Tokens;
	Tokens tokens;

	std::queue<char> lookahead;

	void pullIdentifier(std::istream &in);
	void pullNumber(std::istream &in);
	void pullToken(std::istream &in);

public:
	Lexer(std::istream &in);

	typedef Tokens::iterator iterator;
	typedef Tokens::const_iterator const_iterator;

	iterator begin() {
		return tokens.begin();
	}

	iterator end() {
		return tokens.end();
	}

	const_iterator cbegin() const {
		return tokens.cbegin();
	}

	const_iterator cend() const {
		return tokens.cend();
	}
};


#endif
