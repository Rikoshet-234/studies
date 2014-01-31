#ifndef FILE_BOARD_H
#define FILE_BOARD_H


#include <array>
#include <vector>


constexpr size_t BOARD_WIDTH = 6;
constexpr size_t BOARD_HEIGHT = 8;


enum class CodePeg {
	red,
	azure, 
	green,
	violet,
	blue,
	yellow,
	none // Has to be the last one.
};


enum class KeyPeg {
	white,
	black,
	none // Has to be the last one.
};


typedef std::array<CodePeg, BOARD_WIDTH> Code;
typedef std::array<KeyPeg, BOARD_WIDTH> Key;


class Board {
public:
	enum class State {
		playing, victory, defeat
	};

private:
	Code pattern; // A pattern that has to be guessed.
	Code guess; // Currently guessed code.

	std::vector<Code> codes; // Previously guessed codes.
	std::vector<Key> keys; // A hints for previously guessed codes.

	State state;
	size_t highlight;

public:
	Board();

	// Hightlighting actions.
	void moveLeft();
	void moveRight();
	void setCodePeg(CodePeg peg);

	void check();

	size_t getHighlight() const {
		return highlight;
	}

	State getState() const {
		return state;
	}

	const Code &getGuess() const {
		return guess;
	}
	
	const std::vector<Code> &getCodes() const {
		return codes;
	}

	const std::vector<Key> &getKeys() const {
		return keys;
	}
};


#endif
