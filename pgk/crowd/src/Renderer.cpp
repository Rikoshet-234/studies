#include "Renderer.hpp"


Renderer::Renderer() : good(true)
{
	gl::Shader boneVS(gl::VertexShader, "shaders/bone.vs");
	gl::Shader boneFS(gl::FragmentShader, "shaders/bone.fs");

	if (!(boneProgram = { boneVS, boneFS })) {
		good = false;
		return;
	}

	boneMVP = glGetUniformLocation(boneProgram, "mvp");
	boneColor = glGetUniformLocation(boneProgram, "color");
}


void Renderer::draw(
	const std::vector<unique_ptr<Bone>> &bones,
	mat4 mvp, mat4 rstack
)
{
	for (auto &bone : bones) {
		draw(*bone, mvp, rstack);
	}
}


void Renderer::draw(
	const Bone &bone,
	mat4 mvp, mat4 rstack
)
{
	mat4 rotated = rstack * glm::mat4_cast(bone.rotation);
	mat4 current = mvp * rotated * glm::scale(
		bone.thickness / 2.0f, bone.thickness / 2.0f, bone.length
	);

	glUniformMatrix4fv(boneMVP, 1, GL_FALSE, glm::value_ptr(current));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, boneMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, boneBrightness);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, 24);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	draw(
		bone.getChildren(),
		mvp, rotated * glm::translate(0.0f, 0.0f, bone.length)
	);
}


void Renderer::draw(const Skeleton &skeleton, mat4 mvp)
{
	glUseProgram(boneProgram);
	draw(skeleton.getBones(), mvp, mat4(1.0f));
}


void Renderer::draw(const Person &person, mat4 vp)
{
	glUniform3fv(boneColor, 1, glm::value_ptr(person.color));
	
	float angle = std::acos(glm::dot(person.velocity, vec3(0.0f, 1.0f, 0.0f)));
	mat4 mvp = vp * glm::translate(person.position) * glm::rotate(
		glm::degrees(angle), 0.0f, 0.0f, 1.0f
	);

	draw(person.getSkeleton(), mvp);
}


void Renderer::draw(const Crowd &crowd, mat4 vp)
{
	for (auto &person : crowd.getPeople()) {
		draw(*person, vp);
	}
}
