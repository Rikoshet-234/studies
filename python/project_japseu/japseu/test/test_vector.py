from math import sqrt
from japseu.vector import Vec, length, dot, normalize


class TestOperators:

    def test_addition(self):
        v = Vec(1, 3)
        u = Vec(4, 7)
        w = v + u
        assert w.x == 5
        assert w.y == 10

    def test_subtraction(self):
        v = Vec(4, 7)
        u = Vec(1, 3)
        w = v - u
        assert w.x == 3
        assert w.y == 4

    def test_scaling(self):
        v = Vec(3, 4)
        u = v * 5
        assert u.x == 15
        assert u.y == 20

        v = Vec(12, 4)
        u = v / 2
        assert u.x == 6
        assert u.y == 2


def test_length():
    v = Vec(1, 1)
    assert abs(length(v) - sqrt(2)) < 0.0001


def test_dot():
    v = Vec(0, 1)
    u = Vec(1, 0)
    assert dot(v, u) == 0
    assert dot(u, v) == 0
    assert dot(v, v) == 1


def test_normalize():
    v = Vec(5, 16)
    n = normalize(v)
    assert length(n) == 1
