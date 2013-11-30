
def roll():
	from random import random
	score = int(round(random() * 5)) + 1
	print("Rolled %d." % (score,))
	return score


class Player:
	
	def __init__(self, name):
		self.name = name
		self.score = 0

	def turn(self):
		print(self.name + " turn now.")
		self.score += roll()
		self.score += roll()


def play(n):
	player1 = Player("Popek")
	player2 = Player("Walter White")

	for _ in range(n):
		player1.turn()
		player2.turn()
	
	while player1.score == player2.score:
		score1.turn()
		score2.turn()

	better = player1 if player1.score > player2.score else player2
	print("Player %s has won with score %d!" % (better.name, better.score))



from sys import argv

for arg in argv[1:]:
	play(int(arg))
