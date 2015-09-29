#include <sstream>
#include <stdexcept>
#include <string>

#include "App.hpp"


int main(int argc, char **argv)
{
	float lambda = 0.0f;
	Algorithm algorithm = Algorithm::EDMONDS;
	std::string filepath;

	for (size_t i = 1; i < static_cast<size_t>(argc);) {
		std::string cmd(argv[i]);
		if (cmd == "--algorithm" || cmd == "-a") {
			std::string alg(argv[i + 1]);
			if (alg == "edmonds") {
				algorithm = Algorithm::EDMONDS;
			} else if (alg == "boykov") {
				algorithm = Algorithm::BOYKOV;
			} else if (alg == "boykov2") {
				algorithm = Algorithm::BOYKOV2;
			} else {
				throw std::runtime_error("unknown algorithm");
			}
			i += 2;
		} else if (cmd == "--lambda" || cmd == "-l") {
			std::istringstream is(argv[i + 1]);
			is >> lambda;
			i += 2;
		} else {
			filepath = std::move(cmd);
			i++;
		}
	}

	App app(filepath, algorithm, lambda);
	app.run();

	return 0;
}
