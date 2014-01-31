#ifndef FILE_ANIMATION_HPP
#define FILE_ANIMATION_HPP


#include <vector>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using glm::quat;

#include "Skeleton.hpp"


struct Key {
	Bone &bone;
	quat rotation;
};


class Frame {
public:
	float duration;
	std::vector<Key> keys;

	Frame(float duration, const std::vector<Key> &keys);
};


class Animation {
private:
	typedef std::vector<Frame> Frames;

	Frames frames; // List of all frames.
	Frames::iterator current; // Active fame.

	std::vector<quat> oldKeys;

	float elapsed = 0.0f; // Time that has elapsed since previous frame finished.
	bool paused = false; // Is animation paused?

	void cacheKeys();
	void nextFrame();

public:
	Animation();
	Animation(std::vector<Frame> frames);
	Animation &operator=(std::vector<Frame> frames);

	friend void swap(Animation &a, Animation &b) {
		using std::swap;

		swap(a.frames, b.frames);
		swap(a.current, b.current);
		swap(a.oldKeys, b.oldKeys);
		swap(a.elapsed, b.elapsed);
		swap(a.paused, b.paused);
	}

	bool isPaused() const {
		return paused;
	}

	void pause() {
		paused = true;
	}

	void play() {
		paused = false;
	}

	void update(float delta);
};


#endif
