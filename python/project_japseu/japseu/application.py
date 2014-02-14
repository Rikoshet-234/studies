import pygame

from .vector import Vec, length
from .ecs import World
from .components import Vision, Position, Velocity
from .systems import *
from .entities import Player, Enemy, Bullet, Star
from .tile import Map
from .camera import Camera


class App(object):

    def __init__(self, size):
        super(App, self).__init__()

        pygame.display.set_caption("Japseu")
        screen = pygame.display.set_mode(size)
        map = Map("maps/base.tmx")

        self.camera = Camera()

        self.world = World()
        self.world.add(Movement(map))
        self.world.add(Gravity())
        self.world.add(Turning())
        self.world.add(Ballistics(map))
        self.world.add(Animation())
        self.world.add(ParticleManager())

        self.world.add(Star(Position(600, 100)))

        self.player = Player(Position(550, 150))
        self.world.add(self.player)

        self.world.add(Pickups(self.player))
        self.world.add(AI(self.player))
        self.world.add(LifeSupport(self.player))
        self.world.add(Renderer(screen, self.camera, map, self.player))

        for _ in range(10):
            self.world.add(Enemy(Position(1000, 400)))

        self.running = False

    def update(self, delta):
        keys = pygame.key.get_pressed()
        for event in pygame.event.get():
            self.handle_event(event, delta)

        velocity = self.player.velocity
        if keys[pygame.K_LEFT]:
            velocity.x = max(-10.0, velocity.x - delta * 0.1)
        elif keys[pygame.K_RIGHT]:
            velocity.x = min(+10.0, velocity.x + delta * 0.1)
        else:
            speed = length(velocity)
            if speed <= delta:
                velocity.x = 0.0
            else:
                velocity.x *= (speed - delta) / speed

        self.world.process(delta)
        pygame.display.update()

    def handle_event(self, event, delta):
        if event.type == pygame.QUIT:
            self.running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                self.player.velocity.y = +30.0
            if event.key == pygame.K_LCTRL:
                if self.player.vision.direction == Vision.LEFT_DIRECTION:
                    velocity = Velocity(-30.0, 0.0)
                if self.player.vision.direction == Vision.RIGHT_DIRECTION:
                    velocity = Velocity(+30.0, 0.0)
                position = Position(self.player.position.x,
                                    self.player.position.y)
                position += velocity
                self.world.add(Bullet(position, velocity))

        if event.type == pygame.KEYUP:
            if event.key == pygame.K_ESCAPE:
                self.running = False

    def run(self):
        self.running = True
        old_time = pygame.time.get_ticks()

        while self.running:
            new_time = pygame.time.get_ticks()
            delta = new_time - old_time
            old_time = new_time

            self.camera.look_at(self.player.position)
            self.update(float(delta))

            if not self.player.lives > 0:
                fmt = "You died with total score of %d points."
                print(fmt % (self.player.points))
                self.running = False
