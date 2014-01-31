#include "Board.hpp"


Board::Board() : state(State::playing), highlight(0)
{
	guess.fill(CodePeg::none);
	// Randomly generate the pattern.
	for (auto &peg : pattern) {
		auto count = static_cast<int>(CodePeg::none);
		peg = static_cast<CodePeg>(std::rand() % count);
	}
}


void Board::moveLeft()
{
	if (highlight == 0) {
		highlight = BOARD_WIDTH;
	}
	highlight -= 1;
}


void Board::moveRight()
{
	highlight += 1;
	if (highlight == BOARD_WIDTH) {
		highlight = 0;
	}
}


void Board::setCodePeg(CodePeg peg)
{
	guess[highlight] = peg;
}


void Board::check()
{
	// First, check if all the code values are present;
	for (auto &peg : guess) {
		if (peg == CodePeg::none) {
			return;
		}
	}

	// Now we can proceed with computing current code for a player.
	Key hint;
	hint.fill(KeyPeg::none);
	Key::iterator curr;

	std::array<bool, BOARD_WIDTH> used;
	used.fill(false);

	curr = std::begin(hint);
	for (auto &peg : pattern) {
		for (size_t i = 0; i < BOARD_WIDTH; i++) {
			if (used[i]) {
				continue;
			}

			if (peg == guess[i]) {
				*curr = KeyPeg::white;
				curr++;

				used[i] = true;
				break;
			}
		}
	}

	curr = std::begin(hint);
	for (size_t i = 0; i < BOARD_WIDTH; i++) {
		if (pattern[i] == guess[i]) {
			*curr = KeyPeg::black;
			curr++;
		}
	}

	// Now, we add code and key to previous guesses and create a new ones.
	codes.push_back(guess);
	keys.push_back(hint);

	highlight = 0;
	guess.fill(CodePeg::none);

	// Checking for eventual victory of defeat.
	if (codes.size() >= BOARD_HEIGHT) {
		// Guessed more times than board rows, defeat.
		state = State::defeat;
	} else if (curr == std::end(hint)) {
		// All black, victory!
		state = State::victory;
	}
}
