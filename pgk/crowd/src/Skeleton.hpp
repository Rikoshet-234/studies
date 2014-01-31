#ifndef FILE_SKELETON_HPP
#define FILE_SKELETON_HPP


#include <vector>
#include <memory>
using std::unique_ptr;

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using glm::vec2;
using glm::quat;


class Bone {
private:
	typedef std::vector<unique_ptr<Bone>> Bones;
	friend class Skeleton;

	Bone *parent;
	Bones children;

	Bone(Bone *parent, float length, float thickness, quat rotation);

	// The Big Four and Half.
	Bone(const Bone &other);
	Bone &operator =(Bone other);

	friend void swap(Bone &a, Bone &b) {
		using std::swap;

		swap(a.rotation, b.rotation);
		swap(a.length, b.length);
		swap(a.thickness, b.thickness);		
		
		swap(a.parent, b.parent);
		swap(a.children, b.children);
	}

public:
	quat rotation;
	float length, thickness;

	const Bones &getChildren() const {
		return children;
	}
};


class Skeleton {
private:
	typedef std::vector<unique_ptr<Bone>> Bones;
	Bones bones;

public:
	Bone &createBone(
		float length, float thickness, quat rotation = quat());
	Bone &createBone(
		Bone &parent,
		float length, float thickness, quat rotation = quat());

	const Bones &getBones() const {
		return bones;
	}
};


#endif
