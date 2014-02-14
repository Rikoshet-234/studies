import pytest
from japseu.ecs import World, Entity, Component, System


class Position(Component):

    def __init__(self, value):
        self.value = value


class Velocity(Component):

    def __init__(self, value):
        self.value = value


class Printable(Component):

    def __init__(self, name):
        self.name = name


class Movement(System):

    def process(self, world, delta):
        for entity in world.query(Position, Velocity):
            entity.position.value += entity.velocity.value * delta


def test_entities():
    e = Entity()
    e.position = Position(10)
    e.velocity = Velocity(20)
    e.not_component = 30

    assert e.has_component("position")
    assert e.has_component("velocity")
    assert not e.has_component("not_component")


def test_queries():
    world = World()

    e1 = Entity()
    e1.position = Position(10)
    e1.velocity = Velocity(1)
    e1.printable = Printable("e1")

    e2 = Entity()
    e2.position = Position(30)

    world.add(e1)
    world.add(e2)

    assert e1 in world.query(Position)
    assert e2 in world.query(Position)
    assert e1 in world.query(Velocity, Printable)
    assert e2 not in world.query(Velocity, Printable)

    world.remove(e1)

    assert e1 not in world.query(Position)
    assert e1 not in world.query(Velocity)
    assert e2 in world.query(Position)

    world.remove(e2)

    assert e2 not in world.query(Position)


def test_systems():
    world = World()
    world.add(Movement())

    e = Entity()
    e.position = Position(10)
    e.velocity = Velocity(1)

    world.add(e)

    assert e.position.value == 10
    assert e.velocity.value == 1

    world.process(1)

    assert e.position.value == 11
    assert e.velocity.value == 1

    world.process(4)

    assert e.position.value == 15
    assert e.velocity.value == 1


def test_incorrect_adding():
    world = World()

    with pytest.raises(TypeError):
        world.add(1)
