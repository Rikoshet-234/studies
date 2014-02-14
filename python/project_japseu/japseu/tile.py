import pygame
from pygame import Rect, Surface
from math import floor
from xml.etree import ElementTree
from io import StringIO
import re
import csv


WIDTH = 64
HEIGHT = 64

EMPTY = 0


class Tileset(object):

    def __init__(self, path):
        super(Tileset, self).__init__()
        self.image = pygame.image.load(path)


class Map(object):
    """
    Loads map tiles from given TMX file and prerenders it for faster drawing.
    """

    def __init__(self, path):
        super(Map, self).__init__()

        self.tileset = Tileset("gfx/tiles.png")
        self._load_map(path)
        self.rendered = Surface((self.width * WIDTH, self.height * HEIGHT))
        self._render_map(self.rendered)

    def _load_map(self, path):
        layers = ElementTree.parse(path).findall("layer")
        for layer in layers:
            name = layer.get("name")
            if name == "tiles":
                self.width = int(layer.get("width"))
                self.height = int(layer.get("height"))
                self.tiles = []
                self._load_tiles(layer.find("data"))
            if name == "enemies":
                pass
            if name == "pickups":
                pass

    def _load_tiles(self, data):
        raw = re.sub(r"\s+", "", data.text)
        reader = csv.reader(StringIO(raw))

        row = []
        for tile in list(reader)[0]:
            row.append(int(tile))
            if len(row) == self.width:
                self.tiles.append(row)
                row = []

        self.tiles.reverse()

    def _render_map(self, surface):
        surface.fill((0, 0, 0))
        for y in range(self.height):
            for x in range(self.width):
                self._render_tile(surface, self.tiles[y][x], x, y)

    def _render_tile(self, surface, tile, xi, yi):
        if tile == EMPTY:
            return

        rect = Rect((tile - 1) * WIDTH, 0, WIDTH, HEIGHT)
        x = xi * WIDTH
        y = surface.get_height() - (yi + 1) * HEIGHT
        surface.blit(self.tileset.image, (x, y), rect)

    def at(self, position):
        x = position[0] // WIDTH
        y = position[1] // HEIGHT
        return self.tiles[y][x]
