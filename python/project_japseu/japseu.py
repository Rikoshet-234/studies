import pygame
from japseu.application import App


if __name__ == "__main__":
    pygame.init()
    app = App((800, 600))
    app.run()
