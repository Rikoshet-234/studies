#include <glm/gtx/rotate_vector.hpp>
#include "Crowd.hpp"


static inline float myrand()
{
	return static_cast<float>(std::rand()) / RAND_MAX;
}


Person::Person(vec3 position, vec3 color)
: position(position), color(color), activeAnimation(&standing)
{
	initSkeleton();
	initAnimation();
}


void Person::initSkeleton()
{
	// Main components.
	bones[el_torso] = &skeleton.createBone(0.75f, 0.1f);
	bones[el_head] = &skeleton.createBone(*bones[el_torso], 0.35f, 0.2f);

	// Arms.
	bones[el_larm] = &skeleton.createBone(
		*bones[el_torso], 0.4f, 0.07f,
		glm::quat_cast(glm::rotate(+130.0f, 0.0f, 1.0f, 0.0f))
	);
	bones[el_lforearm] = &skeleton.createBone(
		*bones[el_larm], 0.45f, 0.05f,
		glm::quat_cast(glm::rotate(+30.0f, 0.0f, 1.0f, 0.0f))
	);

	bones[el_rarm] = &skeleton.createBone(
		*bones[el_torso], 0.4f, 0.07f,
		glm::quat_cast(glm::rotate(-130.0f, 0.0f, 1.0f, 0.0f))
	);
	bones[el_rforearm] = &skeleton.createBone(
		*bones[el_rarm], 0.45f, 0.05f,
		glm::quat_cast(glm::rotate(-30.0f, 0.0f, 1.0f, 0.0f))
	);

	// Legs.
	bones[el_lthigh] = &skeleton.createBone(
		0.5f, 0.07f,
		glm::quat_cast(glm::rotate(+165.0f, 0.0f, 1.0f, 0.0f))
	);
	bones[el_lshin] = &skeleton.createBone(*bones[el_lthigh], 0.5f, 0.05f);
	
	bones[el_rthigh] = &skeleton.createBone(
		0.5f, 0.07f,
		glm::quat_cast(glm::rotate(-165.0f, 0.0f, 1.0f, 0.0f))
	);
	bones[el_rshin] = &skeleton.createBone(*bones[el_rthigh], 0.5f, 0.05f);
}


void Person::initAnimation()
{
	// Walking animation.
	Frame walking1(0.7f, {
		{ *bones[el_rthigh], glm::quat_cast(glm::rotate(+30.0f, 1.0f, 0.0f, 0.0f)) * bones[el_rthigh]->rotation },
		{ *bones[el_rshin], glm::quat_cast(glm::rotate(+5.0f, 1.0f, 0.0f, 0.0f)) },
		{ *bones[el_lthigh], glm::quat_cast(glm::rotate(-20.0f, 1.0f, 0.0f, 0.0f)) * bones[el_lthigh]->rotation },
		{ *bones[el_lshin], glm::quat_cast(glm::rotate(+20.0f, 1.0f, 0.0f, 0.0f)) },
		{ *bones[el_torso], glm::quat_cast(glm::rotate(+30.0f, 0.0f, 0.0f, 1.0f)) },
		{ *bones[el_rforearm], glm::quat_cast(glm::rotate(+20.0f, 1.0f, 0.0f, 0.0f)) * bones[el_rforearm]->rotation },
		{ *bones[el_lforearm], glm::quat_cast(glm::rotate(-45.0f, 1.0f, 0.0f, 0.0f)) * bones[el_lforearm]->rotation }
	});

	Frame walking2(0.7f, {
		{ *bones[el_rthigh], glm::quat_cast(glm::rotate(-20.0f, 1.0f, 0.0f, 0.0f)) * bones[el_rthigh]->rotation },
		{ *bones[el_rshin], glm::quat_cast(glm::rotate(+20.0f, 1.0f, 0.0f, 0.0f)) },
		{ *bones[el_lthigh], glm::quat_cast(glm::rotate(+30.0f, 1.0f, 0.0f, 0.0f)) * bones[el_lthigh]->rotation },
		{ *bones[el_lshin], glm::quat_cast(glm::rotate(+5.0f, 1.0f, 0.0f, 0.0f)) },
		{ *bones[el_torso], glm::quat_cast(glm::rotate(-30.0f, 0.0f, 0.0f, 1.0f)) },
		{ *bones[el_rforearm], glm::quat_cast(glm::rotate(-45.0f, 1.0f, 0.0f, 0.0f)) * bones[el_rforearm]->rotation },
		{ *bones[el_lforearm], glm::quat_cast(glm::rotate(+20.0f, 1.0f, 0.0f, 0.0f)) * bones[el_lforearm]->rotation }
	});

	walking = { walking1, walking2 };

	// Standing animation.
	Frame standing1(1.0f, {
		{ *bones[el_rthigh], glm::quat_cast(glm::rotate(+5.0f, 1.0f, 0.0f, 0.0f)) * bones[el_rthigh]->rotation },
		{ *bones[el_rshin], glm::quat_cast(glm::rotate(+15.0f, 1.0f, 0.0f, 0.0f)) },
		{ *bones[el_lthigh], glm::quat_cast(glm::rotate(+5.0f, 1.0f, 0.0f, 0.0f)) * bones[el_lthigh]->rotation },
		{ *bones[el_lshin], glm::quat_cast(glm::rotate(+15.0f, 1.0f, 0.0f, 0.0f)) },
		{ *bones[el_torso], glm::quat_cast(glm::rotate(+5.0f, 0.0f, 0.0f, 1.0f)) },
	});

	Frame standing2(1.0, {
		{ *bones[el_rthigh], bones[el_rthigh]->rotation },
		{ *bones[el_rshin], quat() },
		{ *bones[el_lthigh], bones[el_lthigh]->rotation },
		{ *bones[el_lshin], quat() },
		{ *bones[el_torso], glm::quat_cast(glm::rotate(-5.0f, 0.0f, 0.0f, 1.0f)) },
	});

	standing = { standing1, standing2 };
}


void Person::update(float delta)
{
	activeAnimation->update(delta);
	
	stateRemaining -= delta;
	if (stateRemaining <= 0.0f) {
		stateRemaining = myrand() * 3.0f + 4.0f;
		switch (state) {
		case State::standing: {
			state = State::walking;
			vec2 vel2 = glm::rotate(vec2(1.0f, 0.0f), myrand() * 360.0f);
			velocity = vec3(vel2.x, vel2.y, 0.0f);
			activeAnimation = &walking;
			break;
		}
		case State::walking:
			state = State::standing;
			activeAnimation = &standing;
			break;
		}
	}

	if (state == State::standing) {
		return;
	}

	position += velocity * delta;
	if (std::abs(position.x) > 10.0f) {
		velocity.x *= -1;
		position.x = 10.0f * ((position.x > 0) - (position.x < 0));
	}
	if (std::abs(position.y) > 10.0f) {
		velocity.y *= -1;
		position.y = 10.0f * ((position.y > 0) - (position.y < 0));
	}
}


Crowd::Crowd(size_t count)
{
	vec3 black(0.3f, 0.3f, 0.3f); // Batman color.
	vec3 red(1.0f, 0.0f, 0.0f), green(0.0f, 1.0f, 0.0f); // Others colors.

	// Batman.
	people.push_back(unique_ptr<Person>(
		new Person(vec3(0.0f, 0.0f, 0.0f), black)
	));

	// Random people.
	for (auto i = 1; i < count; i++) {
		vec3 position(myrand() * 20.0f - 10.0f, myrand() * 20.f - 10.0f, 0.0f);
		vec3 color = (myrand() > 0.5f) ? red : green;
		people.push_back(unique_ptr<Person>(
			new Person(position, color)
		));
	}
}


void Crowd::update(float delta)
{
	for (auto &person : people) {
		person->update(delta);
	}
}
