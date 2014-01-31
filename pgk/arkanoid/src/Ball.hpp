#ifndef FILE_BALL_H
#define FILE_BALL_H


#include <cmath>

#include <glm/glm.hpp>
using glm::vec2;


class Ball {
private:
	vec2 position = vec2(0.0f);

public:
	static const float RADIUS;

	vec2 velocity = vec2(0.0f);

	void update(float delta);

	vec2 getPosition() const {
		return position;
	}

	vec2 getVelocity() const {
		return velocity;
	}

	void setVelocity(vec2 velocity) {
		this->velocity = velocity;
	}
};


#endif
