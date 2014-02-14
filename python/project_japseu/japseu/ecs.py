

class World(object):

    def __init__(self):
        self.systems = list()
        self.entities = dict()

    def query(self, *components):
        try:
            result = set(self.entities[components[0]])
            for component in components:
                result &= self.entities[component]
            return result
        except (KeyError, IndexError):
            return set()

    def add(self, element):
        if isinstance(element, System):
            self._add_system(element)
        elif isinstance(element, Entity):
            self._add_entity(element)
        else:
            raise TypeError("only systems and entities can be added")

    def remove(self, element):
        if isinstance(element, System):
            self._remove_system(element)
        elif isinstance(element, Entity):
            self._remove_entity(element)
        else:
            raise TypeError("only systems and entities can be removed")

    def process(self, delta):
        for system in self.systems:
            system.process(self, delta)

    def _add_system(self, system):
        self.systems.append(system)

    def _add_entity(self, entity):
        for (name, component) in vars(entity).items():
            if not isinstance(component, Component):
                continue

            if not type(component) in self.entities:
                self.entities[type(component)] = set()

            self.entities[type(component)].add(entity)

    def _remove_system(self, system):
        self.systems.remove(system)

    def _remove_entity(self, entity):
        for (name, component) in vars(entity).items():
            if not isinstance(component, Component):
                continue

            self.entities[type(component)].remove(entity)


class System(object):

    def process(self, world, delta):
        raise NotImplementedError()


class Entity(object):

    def has_component(self, name):
        return (hasattr(self, name) and
                isinstance(getattr(self, name), Component))


class Component(object):
    pass
