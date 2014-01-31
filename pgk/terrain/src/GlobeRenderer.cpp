#include "GlobeRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::vec4;

#include <array>


GlobeRenderer::GlobeRenderer(const LOD &lod, const Offset &offset, vec3 camera)
: AbstractRenderer(lod, offset)
{
	cameraPosition = camera;
	initCamera();
	initShaders();

	grid = unique_ptr<GridRenderer>(new GridRenderer());
}


void GlobeRenderer::initCamera()
{
	cameraAngle = vec2(1.57f, 0.0f);
	updateView();

	projection = glm::perspective(45.0f, 1.0f, 0.001f, 100.0f);
}


void GlobeRenderer::initShaders()
{
	gl::Shader vs(gl::VertexShader, "shaders/globe.vs");
	gl::Shader fs(gl::FragmentShader, "shaders/globe.fs");
	program = unique_ptr<gl::Program>(new gl::Program({ vs, fs }));

	mvpUniform = glGetUniformLocation(*program, "mvp");
	latitudeUniform = glGetUniformLocation(*program, "latitude");
	longtitudeUniform = glGetUniformLocation(*program, "longtitude");
	offsetAttrib = glGetAttribLocation(*program, "offset");
	heightAttrib = glGetAttribLocation(*program, "height");
}


static inline vec2 screenPosition(vec2 angle, mat4 mvp)
{
	float phi = glm::radians(angle.x);
	float theta = glm::radians(angle.y);
	vec4 position = mvp * vec4(
		glm::cos(theta) * glm::cos(phi),
		glm::cos(theta) * glm::sin(phi),
		glm::sin(theta),
		1.0f
	);

	return vec2(position) / position.w;
}


template <typename T>
static inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}


static inline bool isVisible(const Sector &sector, mat4 mvp)
{
	auto latitude = sector.getLatitude(), longtitude = sector.getLongtitude();
	std::array<vec2, 4> box = {
		screenPosition(vec2(latitude, longtitude), mvp),
		screenPosition(vec2(latitude + 1, longtitude), mvp),
		screenPosition(vec2(latitude + 1, longtitude + 1), mvp),
		screenPosition(vec2(latitude, longtitude + 1), mvp)
	};

	// Check if whether any of points is on screen.
	for (auto &p : box) {
		if ((-1.0f <= p.x && p.x <= +1.0f) &&
		    (-1.0f <= p.y && p.y <= +1.0))
		{
			return true;
		}
	}

	// Check for segment intersection with screen borders.
	for (int i = 0; i < box.size(); i++) {
		auto j = (i + 1) % box.size();

		if (sign(box[i].x - 1.0f) != sign(box[j].x - 1.0f) ||
		    sign(box[i].x + 1.0f) != sign(box[j].x + 1.0f) ||
		    sign(box[i].y - 1.0f) != sign(box[j].y - 1.0f) ||
		    sign(box[i].y + 1.0f) != sign(box[j].y + 1.0f))
		{
			return true;
		}
	}

	return false;
}


void GlobeRenderer::draw(const World &world)
{
	mat4 mvp = projection * view;
	
	grid->draw(mvp);
	for (auto &sector : world.getSectors()) {
		draw(*sector, mvp);
	}
}


void GlobeRenderer::draw(const Sector &sector, mat4 mvp)
{
	if (!isVisible(sector, mvp)) {
		return;
	}

	glUseProgram(*program);

	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1i(latitudeUniform, sector.getLatitude());
	glUniform1i(longtitudeUniform, sector.getLongtitude());

	glBindBuffer(GL_ARRAY_BUFFER, offset.getVBO());
	glEnableVertexAttribArray(offsetAttrib);
	glVertexAttribPointer(offsetAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, sector.getVBO());
	glEnableVertexAttribArray(heightAttrib);
	glVertexAttribPointer(heightAttrib, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

	int level = sector.getLOD();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lod.getEBO(level));
	glDrawElements(GL_TRIANGLES, lod.getSize(level), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(offsetAttrib);
	glDisableVertexAttribArray(heightAttrib);
}


static inline vec3 direction(vec2 angle)
{
	return vec3(
		std::cos(angle.y) * std::cos(angle.x),
		std::cos(angle.y) * std::sin(angle.x),
		std::sin(angle.y)
	);
}


void GlobeRenderer::move(vec2 delta)
{
	cameraAngle += vec2(-delta.x, delta.y);
	updateView();
}


void GlobeRenderer::zoom(float delta)
{
	cameraPosition += direction(cameraAngle) * delta;
	updateView();
}


void GlobeRenderer::updateView()
{
	view = glm::lookAt(
		cameraPosition, cameraPosition + direction(cameraAngle),
		vec3(0.0f, 0.0f, 1.0f)
	);
}
