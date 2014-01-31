#ifndef FILE_CROWD_HPP
#define FILE_CROWD_HPP


#include <array>
#include <vector>
#include <memory>
using std::unique_ptr;

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
using glm::vec3;
using glm::quat;

#include "Skeleton.hpp"
#include "Animation.hpp"


class Person {
private:
	enum Element {
		el_torso, el_head,
		el_larm, el_lforearm, el_rarm, el_rforearm,
		el_lthigh, el_lshin, el_rthigh, el_rshin,
		el_count
	};

	std::array<Bone *, el_count> bones;

	Skeleton skeleton;
	Animation standing, walking;
	Animation *activeAnimation;

	enum class State {
		standing, walking
	} state = State::standing;
	float stateRemaining = 0.0f;

	void initSkeleton();
	void initAnimation();

public:
	vec3 position, velocity, color;

	Person(vec3 position, vec3 color = vec3(1.0f, 1.0f, 1.0f));

	void update(float delta);

	const Skeleton &getSkeleton() const {
		return skeleton;
	}
};


class Crowd {
private:
	typedef std::vector<unique_ptr<Person>> People;
	
	People people;

public:
	Crowd(size_t count = 10);

	void update(float delta);

	const People &getPeople() const {
		return people;
	}

	const Person &getBatman() const {
		return *people.front();
	}
};


#endif
