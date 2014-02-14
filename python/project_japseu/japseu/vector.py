from math import sqrt, sin, cos


class Vec(object):

    def __init__(self, x, y):
        super(Vec, self).__init__()
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vec(self.x + other.x, self.y + other.y)

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self

    def __sub__(self, other):
        return Vec(self.x - other.x, self.y - other.y)

    def __isub__(self, other):
        self.x -= other.x
        self.y -= other.y
        return self

    def __mul__(self, scalar):
        return Vec(self.x * scalar, self.y * scalar)

    def __imul__(self, scalar):
        self.x *= scalar
        self.y *= scalar
        return self

    def __truediv__(self, scalar):
        return Vec(self.x / scalar, self.y / scalar)

    def __itruediv__(self, scalar):
        self.x /= scalar
        self.y /= scalar
        return self


def length(v):
    return sqrt(v.x * v.x + v.y * v.y)


def normalize(v):
    return v / length(v)


def dot(v, u):
    return v.x * u.x + v.y * u.y


def rotate(v, angle):
    x = self.x * cos(angle) - self.y * sin(angle)
    y = self.x * sin(angle) - self.y * cos(angle)
    return Vec(x, y)
