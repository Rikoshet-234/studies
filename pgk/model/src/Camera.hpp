#ifndef FILE_MODEL_LOADING_CAMERA_HPP
#define FILE_MODEL_LOADING_CAMERA_HPP


#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::mat4;


class Camera {
public:
	Camera();

	void strafe(float delta);
	void zoom(float delta);

	mat4 getVP() const {
		return projection * view;
	}

	mat4 getView() const {
		return view;
	}

	mat4 getProjection() const {
		return projection;
	}

private:
	vec2 position;
	mat4 projection, view;

	void updateView();
};


#endif
