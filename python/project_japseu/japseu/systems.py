from random import choice
from pygame import Color, Rect

from . import tile
from .ecs import System, Component
from .components import *
from .vector import Vec, length
from .entities import Blood, Puff, TextParticle


def _spam_particles(world, kind, position, amount):
    for _ in range(amount):
        world.add(kind(Position(position.x, position.y)))


class Renderer(System):
    """
    Just puts every drawable entity (things that contain position and something
    to draw like image, animation or particle) on the screen. Also, blits
    visible fragment of the prerendered map.
    """

    LIFE_IMAGE = pygame.image.load("gfx/life.png")

    def __init__(self, screen, camera, map, player):
        super(Renderer, self).__init__()
        self.screen = screen
        self.camera = camera
        self.map = map
        self.player = player

    def _draw_tile(self, offset, xi, yi):
        if offset == tile.EMPTY:
            return

        rect = Rect((offset - 1) * tile.WIDTH, 0, tile.WIDTH, tile.HEIGHT)
        position = self.camera.project(Vec((xi + 0) * tile.WIDTH,
                                           (yi + 1) * tile.HEIGHT))
        x = round(position.x)
        y = round(self.screen.get_height() - position.y)
        self.screen.blit(self.map.tileset.image, (x, y), rect)

    def _draw_map(self):
        position = self.camera.unproject(Vec(0, 0))
        height = self.map.rendered.get_height() - self.screen.get_height()
        area = Rect((round(position.x), round(height - position.y)),
                    self.screen.get_size())
        self.screen.blit(self.map.rendered, (0, 0), area)

    def _on_screen(self, position):
        screen_position = self.camera.project(position)
        x = round(screen_position.x)
        y = round(self.screen.get_height() - screen_position.y)
        return (x, y)

    def _draw_still(self, entity):
        rect = entity.image.surface.get_rect()
        rect.center = self._on_screen(entity.position)

        if entity.has_component('vision'):
            surface = entity.vision.surface()
        else:
            surface = entity.image.surface

        self.screen.blit(surface, rect)

    def _draw_animation(self, entity):
        height = entity.cliche.surface.get_height()
        dest = Rect(0, 0, height, height)
        dest.center = self._on_screen(entity.position)

        self.screen.blit(entity.cliche.surface, dest,
                         entity.cliche.active_rect)

    def _draw_particle(self, entity):
        dest = entity.particle.surface.get_rect()
        dest.center = self._on_screen(entity.position)

        stage = entity.particle.time_elapsed / entity.particle.duration
        entity.particle.surface.set_alpha(round(255.0 * (1.0 - stage)))
        self.screen.blit(entity.particle.surface, dest)

    def _draw_lives(self):
        for i in range(self.player.lives):
            position = (i * Renderer.LIFE_IMAGE.get_width(), 10)
            self.screen.blit(Renderer.LIFE_IMAGE, position)

    def process(self, world, delta):
        self.screen.fill(Color(255, 55, 55))

        self._draw_map()
        for entity in world.query(Position, Image):
            self._draw_still(entity)
        for entity in world.query(Position, Cliche):
            self._draw_animation(entity)
        for entity in world.query(Position, Particle):
            self._draw_particle(entity)
        self._draw_lives()


class Movement(System):
    """
    Applies velocity to the entity's position. Also, responsible for colliding
    with map walls (so that entities can land on platforms etc.).
    """

    SPEED = 0.02

    def __init__(self, map):
        super(Movement, self).__init__()
        self.map = map

    def _adjust_x(self, entity):
        (xl, yb, xr, yt) = entity.aabb.pixels(entity.position)

        if entity.velocity.x < 0.0 and (
            self.map.at((xl, yb + 1)) != tile.EMPTY or
            self.map.at((xl, yt - 1)) != tile.EMPTY
        ):
            entity.velocity.x = 0.0
            tile_xl = (xl // tile.WIDTH + 1) * tile.WIDTH
            entity.position.x = tile_xl + entity.aabb.width / 2

        if entity.velocity.x > 0.0 and (
            self.map.at((xr, yb + 1)) != tile.EMPTY or
            self.map.at((xr, yt - 1)) != tile.EMPTY
        ):
            entity.velocity.x = 0.0
            tile_xr = (xr // tile.WIDTH) * tile.WIDTH
            entity.position.x = tile_xr - entity.aabb.width / 2

    def _adjust_y(self, entity):
        (xl, yb, xr, yt) = entity.aabb.pixels(entity.position)

        if entity.velocity.y < 0.0 and (
            self.map.at((xl + 1, yb)) != tile.EMPTY or
            self.map.at((xr - 1, yb)) != tile.EMPTY
        ):
            entity.velocity.y = 0.0
            tile_yb = (yb // tile.HEIGHT + 1) * tile.HEIGHT
            entity.position.y = tile_yb + entity.aabb.height / 2

        if entity.velocity.y > 0.0 and (
            self.map.at((xl + 1, yt)) != tile.EMPTY or
            self.map.at((xr - 1, yt)) != tile.EMPTY
        ):
            entity.velocity.y = 0.0
            tile_yt = (yt // tile.HEIGHT) * tile.HEIGHT
            entity.position.y = tile_yt - entity.aabb.height / 2

    def process(self, world, delta):
        for entity in world.query(Velocity, Position):
            translation = entity.velocity * Movement.SPEED * delta

            entity.position.x += translation.x
            if entity.has_component('aabb'):
                self._adjust_x(entity)

            entity.position.y += translation.y
            if entity.has_component('aabb'):
                self._adjust_y(entity)


class Turning(System):
    """
    Sets appriopiate direction for entities with Vision component based
    on entity's velocity.
    """

    def __init__(self):
        super(Turning, self).__init__()

    def process(self, world, delta):
        turnable = world.query(Velocity, Vision)
        for entity in turnable:
            if entity.velocity.x > 0.0:
                entity.vision.direction = Vision.RIGHT_DIRECTION
            if entity.velocity.x < 0.0:
                entity.vision.direction = Vision.LEFT_DIRECTION


class AI(System):
    """
    Simple enemy intelligence system. For now, it puts enemies in some state
    and changes this state after some delay. Also, applies action associated
    with each state (sets entity velocity when walking and jumping etc.).
    """

    def __init__(self, player):
        super(AI, self).__init__()

        self.player = player

    def process(self, world, delta):
        playerr = self.player.aabb.rect(self.player.position)

        enemies = world.query(Ghost, Velocity, Position, AABB)
        for entity in enemies:
            if (
                not self.player.invicibility.active and
                playerr.colliderect(entity.aabb.rect(entity.position))
            ):
                _spam_particles(world, Blood, self.player.position, 5)
                self.player.velocity = Velocity(entity.velocity.x * 2.0, 15.0)
                self.player.lives -= 1
                self.player.invicibility.time_left = 2000

            ghost = entity.ghost
            ghost.time_left -= delta

            if ghost.mode == Ghost.STANDING:
                entity.velocity.x = 0.0
            if ghost.mode == Ghost.WALKING_LEFT:
                entity.velocity.x = -10.0
            if ghost.mode == Ghost.WALKING_RIGHT:
                entity.velocity.x = +10.0
            if ghost.mode == Ghost.JUMPING:
                entity.velocity.y = +20.0
                ghost.time_left = 0

            if ghost.time_left <= 0:
                ghost.mode = choice([Ghost.STANDING,
                                     Ghost.WALKING_RIGHT, Ghost.WALKING_LEFT,
                                     Ghost.JUMPING])
                ghost.time_left = 1000


class Gravity(System):
    """
    Simply applies gravity to velocity of each entity containing mass
    component.
    """

    VECTOR = Vec(0.0, -9.81) * 0.01

    def __init__(self):
        super(Gravity, self).__init__()

    def process(self, world, delta):
        for entity in world.query(Velocity, Mass):
            entity.velocity += Gravity.VECTOR * delta


class Pickups(System):
    """
    Responsible for picking up things by player and applying effects of this.
    """

    def __init__(self, player):
        super(Pickups, self).__init__()
        self.player = player

    def _pick(self, world, pickup):
        if pickup.genre == Pickup.STAR:
            world.add(TextParticle(Position(self.player.position.x,
                                            self.player.position.y),
                                   "+1",
                                   (255, 255, 0)))
            self.player.points += 1

    def process(self, world, delta):
        player = self.player.aabb.rect(self.player.position)

        for entity in world.query(Pickup, AABB):
            if player.colliderect(entity.aabb.rect(entity.position)):
                world.remove(entity)
                self._pick(world, entity.pickup)


class Ballistics(System):
    """
    Manages projectiles - basically, it checks for collisions with walls
    (bullet is removed in this case) and entities having bounding box
    (damage is applied in this case).
    """

    def __init__(self, map):
        super(Ballistics, self).__init__()
        self.map = map

    def process(self, world, delta):
        creatures = world.query(Health, AABB, Position)
        bullets = world.query(Projectile, Position)
        for bullet in bullets:
            for creature in creatures:
                r = bullet.projectile.radius / 2.0 + creature.aabb.width / 2.0
                if length(bullet.position - creature.position) <= r:
                    _spam_particles(world, Blood, bullet.position, 5)
                    text_y = bullet.position.y + creature.aabb.height / 3.0
                    world.add(TextParticle(Position(bullet.position.x, text_y),
                                           str(bullet.projectile.damage),
                                           (255, 0, 0)))
                    creature.health.hp -= bullet.projectile.damage
                    world.remove(bullet)
                    return

            bullet.projectile.lifetime -= delta
            (x, y) = (round(bullet.position.x), round(bullet.position.y))
            if bullet.projectile.lifetime <= 0:
                world.remove(bullet)
            elif self.map.at((x, y)) != tile.EMPTY:
                world.remove(bullet)
                _spam_particles(world, Puff, bullet.position, 3)


class LifeSupport(System):
    """
    Responsible for removing dead enitites from the world and updating
    invicibility shields state.
    """

    def __init__(self, player):
        super(LifeSupport, self).__init__()
        self.player = player

    def process(self, world, delta):
        if self.player.invicibility.active:
            self.player.invicibility.time_left -= delta

        creatures = world.query(Health)
        for entity in creatures:
            if entity.health.hp <= 0:
                world.remove(entity)


class Animation(System):
    """
    Updates the animation frame, and 'rewinds' the cliche if frame duration
    has ended (according to given rewinding mode).
    """

    def __init__(self):
        super(Animation, self).__init__()

    def rewind(self, cliche):
        if not cliche.frames > 1:
            return

        if cliche.mode == Cliche.REPEATING:
            cliche.active_frame = (cliche.active_frame + 1) % cliche.frames
        elif cliche.going_up:
            cliche.active_frame += 1
            if cliche.active_frame >= cliche.frames:
                cliche.going_up = False
                cliche.active_frame -= 2
        else:
            cliche.active_frame -= 1
            if cliche.active_frame < 0:
                cliche.going_up = True
                cliche.active_frame += 2

        cliche.time_left = cliche.frame_duration

    def process(self, world, delta):
        animations = world.query(Cliche)
        for entity in animations:
            cliche = entity.cliche
            cliche.time_left -= delta
            if cliche.time_left < 0:
                self.rewind(cliche)


class ParticleManager(System):
    """
    Just updates particle entities state and removes ones that are dead (their
    time have ended).
    """

    def __init__(self):
        super(ParticleManager, self).__init__()

    def process(self, world, delta):
        particles = world.query(Particle)
        for entity in particles:
            particle = entity.particle
            particle.time_elapsed += delta
            if particle.time_elapsed > particle.duration:
                world.remove(entity)
