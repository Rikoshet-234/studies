

def factoriali(n):
	accum = 1
	while n > 1:
		n, accum = n - 1, accum * n

	return accum 


def factorialr(n):
	def aux(n, accum):
		if n > 1:
			return aux(n - 1, accum * n)
		return accum

	return aux(n, 1)


def factoriale(n):
	def aux(n, accum):
		if n > 1:
			aux(n - 1, accum * n)
		raise Exception(accum)

	try:
		aux(n, 1)
	except Exception as accum:
		return accum


def fibi(n):
	a, b = 1, 1
	while n > 1:
		n, a, b = n - 1, a + b, a

	return a


def fibr(n):
	def aux(n, a, b):
		if n > 1:
			return aux(n - 1, a + b, a)
		return a

	return aux(n, 1, 1)


def fibe(n):
	def aux(n, a, b):
		if n > 1:
			aux(n - 1, a + b, a)
		raise Exception(a)

	try:
		aux(n, 1, 1)
	except Exception as accum:
		return accum


def test(func, arg):
	print(func + ":")
	print(timeit(func + "(" + arg + ")",
		setup="from __main__ import " + func, number=10))


if __name__ == "__main__":
	from timeit import timeit
	from sys import argv, setrecursionlimit

	setrecursionlimit(100000)

	for arg in argv[1:]:
		test("factoriali", arg)
		test("factorialr", arg)
		test("factoriale", arg)

		test("fibi", arg)
		test("fibr", arg)
		test("fibe", arg)
