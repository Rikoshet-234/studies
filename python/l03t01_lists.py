from functools import *


def primesf(n):
    return list(filter(lambda p: reduce(lambda prime, k: prime and (p % k != 0), range(2, p - 1), True), range(2, n)))


def primesc(n):
    return [p for p in range(2, n) if not [k for k in range(2, p - 1) if p % k == 0]]


def time(func, arg):
    return timeit(func + "(" + arg + ")",
        setup="from __main__ import " + func,
        number=10000)


if __name__ == "__main__":
    from timeit import timeit
    from sys import argv

    for arg in argv[1:]:
        n = int(arg)
        fmt = "(%d) = %s (%0.4fs)"
        print(("primesf" + fmt) % (n, primesf(n), time("primesf", arg)))
        print(("primesc" + fmt) % (n, primesc(n), time("primesc", arg)))
