#ifndef FILE_TERRAIN_GLOBE_RENDERER_HPP
#define FILE_TERRAIN_GLOBE_RENDERER_HPP


#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::mat4;

#include <gl/Shader.hpp>
#include <gl/Program.hpp>

#include <memory>
using std::unique_ptr;

#include "Sector.hpp"
#include "World.hpp"
#include "LOD.hpp"
#include "Offset.hpp"
#include "GridRenderer.hpp"
#include "AbstractRenderer.hpp"


class GlobeRenderer : public AbstractRenderer {
public:
	GlobeRenderer(const LOD &lod, const Offset &offset, vec3 camera);

	virtual void draw(const Sector &sector, mat4 mvp = mat4(1.0f));
	virtual void draw(const World &world) override;

	virtual void move(vec2 delta) override;
	virtual void zoom(float delta) override;

private:
	unique_ptr<GridRenderer> grid;

	unique_ptr<gl::Program> program;
	GLint mvpUniform, latitudeUniform, longtitudeUniform;
	GLint offsetAttrib, heightAttrib;

	mat4 projection, view;
	vec2 cameraAngle;
	void updateView();

	void initCamera();
	void initShaders();
};


#endif
