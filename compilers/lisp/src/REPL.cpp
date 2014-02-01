#include <iostream>
#include <string>
#include <sstream>

#include <lisp/Parser.hpp>
#include <lisp/Interpreter.hpp>


int main()
{
	using namespace lisp;

	Parser parser;
	Interpreter interpreter;

	while (std::cin) {
		std::string expr;
		std::getline(std::cin, expr, ';');
		if (expr.empty()) {
			continue;
		}

		try {
			std::istringstream is(expr);
			List parsed = parser.parse(is);
			std::cout << interpreter.eval(parsed) << "\n";
		} catch (parse_error error) {
			std::cerr << "parse error: " << error.what() << "\n";
		} catch (eval_error error) {
			std::cerr << "evaluation error: " << error.what() << "\n";
		}
	}
	
	return 0;
}
