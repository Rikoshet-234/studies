from functools import *


class PrimeIterator(object):

    def __init__(self, n):
        self.primes = [False, False] + [True] * (n - 2)

        for i in range(len(self.primes)):
            # If not prime, we don't care.
            if not self.primes[i]:
                continue

            # Otherwise, erase all divisible numbers.
            for j in range(2 * i, len(self.primes), i):
                self.primes[j] = False


    def __iter__(self):
        self.pointer = 2
        return self


    def __next__(self):
        p = self.pointer
        while p < len(self.primes) and not self.primes[p]:
            p += 1

        if p >= len(self.primes):
            raise StopIteration

        self.pointer = p + 1
        return p


def primesf(n):
    return list(filter(lambda p: reduce(lambda prime, k: prime and (p % k != 0), range(2, p - 1), True), range(2, n)))


def primesc(n):
    return [p for p in range(2, n) if not [k for k in range(2, p - 1) if p % k == 0]]


def primesi(n):
    return [p for p in PrimeIterator(n)]



def time(func, arg):
    return timeit(func + "(" + arg + ")",
        setup="from __main__ import " + func,
        number=1000)


if __name__ == "__main__":
    from sys import argv
    from timeit import timeit

    table = [
      ("functional", "primesf"),
      ("comprehension", "primesc"),
      ("iterator (sieve)", "primesi")
    ]

    print(' ' * 6, '|', ' | '.join(map(lambda p: "%14s" % p[0], table)))
    for arg in argv[1:]:
        row = [time(func, arg) for (_, func) in table]
        print(
            "% 6d |" % int(arg),
            ' | '.join(map(lambda x: "%14.4f" % (x,), row)))

