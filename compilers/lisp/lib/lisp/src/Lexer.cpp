#include <lisp/Lexer.hpp>


std::ostream &operator <<(std::ostream &out, const Token &token)
{
	switch (token.type) {
	case Token::Type::lpar:
		out << "tok_(";
		break;
	case Token::Type::rpar:
		out << "tok_)";
		break;
	case Token::Type::identifier:
		out << "tok_id(" << token.string << ")";
		break;
	case Token::Type::number:
		out << "tok_num(" << token.number << ")";
		break;
	case Token::Type::eof:
		out << "tok_eof";
		break;
	case Token::Type::unknown:
		out << "tok_unknown";
		break;
	}

	return out;
}


void Lexer::pullIdentifier(std::istream &in)
{
	std::string identifier;

	char curr;
	while (in && (curr = in.get()) && !std::isspace(curr)) {
		lookahead.push(curr);
	}

	while (!lookahead.empty() && lookahead.front() != '(' && lookahead.front() != ')') {
		identifier += lookahead.front();
		lookahead.pop();
	}

	tokens.push_back({ Token::Type::identifier, identifier, 0 });
}


void Lexer::pullNumber(std::istream &in)
{
	int sign = 1;
	if (lookahead.front() == '-') {
		sign = -1;
		lookahead.pop();
	}

	char curr;
	while (in && (curr = in.get())) {
		if (!std::isspace(curr)) {
			lookahead.push(curr);
		}

		if (!std::isdigit(curr)) {
			break;
		}
	}

	int number = 0;
	while (!lookahead.empty() && std::isdigit(lookahead.front())) {
		number = number * 10 + lookahead.front() - '0';
		lookahead.pop();
	}

	tokens.push_back({ Token::Type::number, "", sign * number });
}


void Lexer::pullToken(std::istream &in)
{
	char curr = lookahead.front(), next;
	if (curr == '(') {
		tokens.push_back({ Token::Type::lpar, "", 0 });
		lookahead.pop();
	} else if (curr == ')') {
		tokens.push_back({ Token::Type::rpar, "", 0 });
		lookahead.pop();
	} else if (std::isdigit(curr)) {
		pullNumber(in);
	} else {
		pullIdentifier(in);
	}	
}


Lexer::Lexer(std::istream &in)
{
	while (in) {
		char curr;
		if (lookahead.empty() && (in >> curr)) {
			lookahead.push(curr);
		} else if (!in) {
			break;
		}

		pullToken(in);
	}

	tokens.push_back({ Token::Type::eof, "", 0 });
}
