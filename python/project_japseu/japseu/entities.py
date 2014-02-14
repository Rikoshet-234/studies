from random import random
import pygame
from pygame import Color
from pygame.font import Font

from .ecs import Entity
from .components import *


class Player(Entity):
    IMAGE = Image("gfx/player.png")

    def __init__(self, position=Position(0.0, 0.0)):
        super(Player, self).__init__()

        self.image = Player.IMAGE
        self.position = position
        self.velocity = Velocity(0.0, 0.0)
        self.aabb = AABB(Player.IMAGE.surface.get_size())
        self.mass = Mass()
        self.vision = Vision(Player.IMAGE.surface, Vision.RIGHT_DIRECTION)
        self.invicibility = Invicibility()

        self.lives = 3
        self.points = 0


class Enemy(Entity):
    IMAGE = Image("gfx/enemy.png")

    def __init__(self, position=Position(0.0, 0.0)):
        super(Enemy, self).__init__()

        self.image = Enemy.IMAGE
        self.position = position
        self.velocity = Velocity(0.0, 0.0)
        self.aabb = AABB(Enemy.IMAGE.surface.get_size())
        self.vision = Vision(Enemy.IMAGE.surface, Vision.LEFT_DIRECTION)
        self.ghost = Ghost()
        self.health = Health(20)
        self.mass = Mass()


class Bullet(Entity):
    IMAGE = Image("gfx/bullet.png")

    def __init__(self, position, velocity):
        super(Bullet, self).__init__()

        self.image = Bullet.IMAGE
        self.position = position
        self.velocity = velocity
        self.projectile = Projectile(Bullet.IMAGE.surface.get_width(),
                                     round(7.5 + random() * 15.0), 2000)


class Star(Entity):
    IMAGE = pygame.image.load("gfx/star.png")

    def __init__(self, position):
        super(Star, self).__init__()

        self.cliche = Cliche(Star.IMAGE, 200, Cliche.OSCILATING)
        self.position = position
        self.aabb = AABB((Star.IMAGE.get_height(), Star.IMAGE.get_height()))
        self.pickup = Pickup(Pickup.STAR)


class Puff(Entity):
    IMAGE = None

    def __init__(self, position):
        super(Puff, self).__init__()

        if not Puff.IMAGE:
            Puff.IMAGE = pygame.image.load("gfx/puff.png").convert()
            Puff.IMAGE.set_colorkey(Color(255, 0, 255))

        self.position = position
        self.velocity = Velocity(random() * 5.0 - 2.5, random() * 5.0 - 2.5)
        self.particle = Particle(Puff.IMAGE, 1000)


class Blood(Entity):
    IMAGE = None

    def __init__(self, position):
        super(Blood, self).__init__()

        if not Blood.IMAGE:
            Blood.IMAGE = pygame.image.load("gfx/blood.png").convert()
            Blood.IMAGE.set_colorkey(Color(255, 0, 255))

        self.position = position
        self.velocity = Velocity(random() * 3.0 - 1.5, random() * 3.0 - 1.5)
        self.particle = Particle(Blood.IMAGE, round(random() * 500.0 + 350.0))


class TextParticle(Entity):
    FONT = None

    def __init__(self, position, value, color):
        super(TextParticle, self).__init__()

        if not TextParticle.FONT:
            TextParticle.FONT = Font("gfx/armalite_rifle.ttf", 16)

        self.position = position
        self.velocity = Velocity(random() * 3.0 - 1.5, random() * 1.5 + 1.5)
        rendered = TextParticle.FONT.render(value, True, color)
        self.particle = Particle(rendered, 750)
