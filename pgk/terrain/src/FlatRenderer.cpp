#include "FlatRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


FlatRenderer::FlatRenderer(const LOD &lod, const Offset &offset, vec3 camera)
: AbstractRenderer(lod, offset)
{
	cameraPosition = camera;
	initCamera();
	initShaders();
}


void FlatRenderer::initCamera()
{
	updateView();
	updateProjection();
}


void FlatRenderer::initShaders()
{
	gl::Shader vs(gl::VertexShader, "shaders/flat.vs");
	gl::Shader fs(gl::FragmentShader, "shaders/flat.fs");
	program = unique_ptr<gl::Program>(new gl::Program({ vs, fs }));

	mvpUniform = glGetUniformLocation(*program, "mvp");
	latitudeUniform = glGetUniformLocation(*program, "latitude");
	longtitudeUniform = glGetUniformLocation(*program, "longtitude");
	offsetAttrib = glGetAttribLocation(*program, "offset");
	heightAttrib = glGetAttribLocation(*program, "height");
}


void FlatRenderer::draw(const World &world)
{
	mat4 mvp = projection * view;

	for (auto &sector : world.getSectors()) {
		draw(*sector, mvp);
	}
}


void FlatRenderer::draw(const Sector &sector, mat4 mvp)
{
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


void FlatRenderer::move(vec2 delta)
{
	cameraPosition += vec3(delta, 0.0f);
	updateView();
}


void FlatRenderer::zoom(float delta)
{
	cameraPosition.z -= delta;
	updateProjection();
}


void FlatRenderer::updateView()
{
	auto trans = -vec2(cameraPosition);
	view = glm::translate(mat4(1.0f), vec3(trans, 0.0f));
}


void FlatRenderer::updateProjection()
{
	auto zoom = cameraPosition.z;
	projection = glm::ortho(-zoom, +zoom, -zoom, +zoom);
}
