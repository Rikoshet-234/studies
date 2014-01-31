#ifndef FILE_PADDLE_H
#define FILE_PADDLE_H


class Paddle {
private:
	float position = 0.0f, velocity = 0.0f;

public:
	static const float WIDTH;
	static const float HEIGHT;

	void update(float delta);

	float getPosition() const {
		return position;
	}

	float getVelocity() const {
		return velocity;
	}

	void setVelocity(float velocity) {
		// Truncate velocity.
		if (velocity > 1.0f) {
			velocity = 1.0f;
		} else if (velocity < -1.0f) {
			velocity = -1.0f;
		}

		this->velocity = velocity;
	}
};


#endif