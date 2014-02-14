import pygame
from pygame import Rect

from .vector import Vec
from .ecs import Component


class Velocity(Vec, Component):

    def __init__(self, x, y):
        super(Velocity, self).__init__(x, y)


class Position(Vec, Component):

    def __init__(self, x, y):
        super(Position, self).__init__(x, y)


class AABB(Component):
    """
    Bounding box of the entity (for collision purposes).
    """

    def __init__(self, size):
        (self.width, self.height) = size

    def pixels(self, position):
        xl = round(position.x - self.width / 2)
        yb = round(position.y - self.height / 2)
        xr = round(position.x + self.width / 2)
        yt = round(position.y + self.height / 2)
        return (xl, yb, xr, yt)

    def rect(self, position):
        rect = Rect(0, 0, self.width, self.height)
        rect.center = (round(position.x), round(position.y))
        return rect


class Ghost(Component):
    """
    Part of the entity's 'AI' (just a current 'state' of enemy - whether he is
    walking, jumping or ideling).
    """

    STANDING = 0
    WALKING_LEFT = 1
    WALKING_RIGHT = 2
    JUMPING = 3

    def __init__(self):
        self.mode = Ghost.STANDING
        self.time_left = 1000


class Mass(Component):
    """
    Include this if you want an entity to be subject of gravity.
    """

    pass


class Image(Component):

    def __init__(self, path):
        super(Image, self).__init__()
        self.surface = pygame.image.load(path)


class Cliche(Component):
    """
    Contains animation frames for entity and hold information like active
    frame, frame duration, etc. Generally, animation-related stuff.
    """

    REPEATING = 1
    OSCILATING = 2

    def __init__(self, surface, frame_duration, mode=REPEATING):
        super(Cliche, self).__init__()
        self.surface = surface
        self.frame_duration = frame_duration
        self.mode = mode
        self.active_frame = 0
        self.time_left = frame_duration

        if self.mode == Cliche.OSCILATING:
            self.going_up = True

    @property
    def frames(self):
        return self.surface.get_width() // self.surface.get_height()

    @property
    def active_rect(self):
        size = self.surface.get_height()
        return Rect(self.active_frame * size, 0, size, size)


class Vision(Component):
    """
    Holds information about direction that entity is facing and apriopiate
    entity surfaces for each of directions (because it is impoossible to
    efficiently flip images on the fly in Pygame).
    """

    RIGHT_DIRECTION = 0
    LEFT_DIRECTION = 1

    def __init__(self, rsurface, direction):
        super(Vision, self).__init__()

        self.direction = direction
        self.rsurface = rsurface
        self.lsurface = pygame.transform.flip(rsurface, True, False)

    def surface(self):
        if self.direction == Vision.LEFT_DIRECTION:
            return self.lsurface
        else:
            return self.rsurface


class Pickup(Component):
    STAR = 0

    def __init__(self, genre):
        super(Pickup, self).__init__()

        self.genre = genre


class Projectile(Component):

    def __init__(self, radius, damage, lifetime=1000):
        self.radius = radius
        self.damage = damage
        self.lifetime = lifetime


class Health(Component):

    def __init__(self, hp=100):
        self.hp = hp


class Invicibility(Component):
    """
    Temporary "shielding" component for entity, making it invunerable for any
    kind of damage.
    """

    def __init__(self):
        self.time_left = 0

    @property
    def active(self):
        return self.time_left > 0


class Particle(Component):

    def __init__(self, surface, duration):
        super(Particle, self).__init__()
        self.surface = surface
        self.duration = duration
        self.time_elapsed = 0
