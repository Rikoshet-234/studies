#ifndef FILE_TERRAIN_WORLD_HPP
#define FILE_TERRAIN_WORLD_HPP

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;

#include <vector>
#include <string>
#include <memory>
using std::unique_ptr;

#include "Sector.hpp"
#include "LOD.hpp"


class World {
public:
	typedef std::vector<unique_ptr<Sector>> Sectors;
	
	World(const std::vector<const char *> &files);
	World(const World &other) = delete;

	void decreaseLOD();
	void increaseLOD();
	void decreaseFarthestLOD(vec3 position);
	void increaseClosestLOD(vec3 position);

	const Sectors &getSectors() const {
		return sectors;
	}

	vec2 getSouthWest() const {
		return vec2(minX, minY);
	}

	vec2 getNorthEast() const {
		return vec2(maxX, maxY);
	}

private:
	int minX = 1000, maxX = -1000, minY = 1000, maxY = -1000;

	Sectors sectors;
	void loadSector(const std::string &filepath);
};


#endif
