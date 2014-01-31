#include "Animation.hpp"


Frame::Frame(float duration, const std::vector<Key> &keys)
: duration(duration), keys(keys)
{
}


Animation::Animation()
{
}


Animation::Animation(std::vector<Frame> frames)
: frames(frames), current(std::begin(this->frames))
{
	size_t max = 0;
	for (auto &frame : frames) {
		max = std::max(max, frame.keys.size());
	}

	oldKeys.resize(max);
	cacheKeys();
}


Animation &Animation::operator =(std::vector<Frame> frames)
{
	Animation other(frames);
	swap(*this, other);
	return *this;
}


void Animation::cacheKeys()
{
	// For every node movement in new frame, keep old value.
	for (size_t i = 0; i < current->keys.size(); i++) {
		oldKeys[i] = current->keys[i].bone.rotation;
	}
}


void Animation::nextFrame()
{
	current++;
	if (current == std::end(frames)) {
		current = std::begin(frames);
	}

	cacheKeys();
}

#include <iostream>
void Animation::update(float delta)
{
	if (paused) {
		return;
	}

	elapsed += delta;
	if (elapsed > current->duration) {
		nextFrame();
		elapsed = 0;
	}

	// Interpolate rotation for every rotation in current frame.
	const float t = elapsed / current->duration;
	for (size_t i = 0; i < current->keys.size(); i++) {
		quat start = oldKeys[i], dest = current->keys[i].rotation;
		current->keys[i].bone.rotation = glm::mix(start, dest, t);
	}
}
