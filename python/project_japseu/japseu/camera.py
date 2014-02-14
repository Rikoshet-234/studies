from .vector import Vec


class Camera(object):
    SCREEN_BOUNDS = Vec(800, 600) / 2.0

    def __init__(self, center=Vec(0.0, 0.0)):
        super(Camera, self).__init__()

        self._projection = None
        self.look_at(center)

    def look_at(self, center):
        self._projection = center - Camera.SCREEN_BOUNDS

    def project(self, position):
        return position - self._projection

    def unproject(self, position):
        return position + self._projection
