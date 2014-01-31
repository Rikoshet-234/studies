#ifndef FILE_TERRAIN_ABSTRACT_RENDERER_HPP
#define FILE_TERRAIN_ABSTRACT_RENDERER_HPP


#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include "LOD.hpp"
#include "Offset.hpp"
#include "World.hpp"


class AbstractRenderer {
public:
	AbstractRenderer(const LOD &lod, const Offset &offset);

	virtual void draw(const World &world) = 0;

	virtual void move(vec2 delta) = 0;
	virtual void zoom(float delta) = 0;

	vec3 getCameraPosition() const {
		return cameraPosition;
	}

protected:
	const LOD &lod;
	const Offset &offset;

	vec3 cameraPosition;
};


#endif
