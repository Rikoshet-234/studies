#include "Renderer.hpp"

#include <glm/gtc/type_ptr.hpp>


Renderer::Renderer()
{
	gl::Shader vs(gl::VertexShader, "shaders/simple.vs");
	gl::Shader fs(gl::FragmentShader, "shaders/simple.fs");
	gl::Shader perlin(gl::FragmentShader, "shaders/perlin.glsl");
	program = unique_ptr<gl::Program>(new gl::Program({ vs, fs, perlin }));

	mvpUniform = glGetUniformLocation(*program, "mvp");
	viewUniform = glGetUniformLocation(*program, "view");
	samplerUniform = glGetUniformLocation(*program, "sampler");

	textureModeUniform = glGetUniformLocation(*program, "textureMode");
	lightsOnUniform = glGetUniformLocation(*program, "lightsOn");
	alertPhaseUniform = glGetUniformLocation(*program, "alertPhase");

	positionAttrib = glGetAttribLocation(*program, "position");
	uvAttrib = glGetAttribLocation(*program, "uv");
	normalAttrib = glGetAttribLocation(*program, "normal");
}


void Renderer::update(float delta)
{
	timeElapsed += delta;
}


void Renderer::draw(const Model &model, const Camera &camera)
{
	glUseProgram(*program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.getTexture());
	glUniform1i(samplerUniform, 0);

	mat4 view = camera.getView(), projection = camera.getProjection();
	mat4 mvp = projection * view;
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));

	glUniform1f(textureModeUniform, textureMode * 0.25f);
	glUniform1f(lightsOnUniform, (lightsOn ? 1.0f : 0.0f));
	glUniform1f(alertPhaseUniform, std::abs(std::sin(timeElapsed)));

	glEnableVertexAttribArray(positionAttrib);
	glEnableVertexAttribArray(uvAttrib);
	glEnableVertexAttribArray(normalAttrib);
	
	glBindBuffer(GL_ARRAY_BUFFER, model.getVBO());
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, model.getTBO());
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, model.getNBO());
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, model.getSize());

	glDisableVertexAttribArray(positionAttrib);
	glDisableVertexAttribArray(uvAttrib);
	glDisableVertexAttribArray(normalAttrib);
}
