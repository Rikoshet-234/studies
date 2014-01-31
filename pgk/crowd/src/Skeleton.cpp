#include "Skeleton.hpp"


Bone::Bone(Bone *parent, float length, float thickness, quat rotation)
: parent(parent), length(length), thickness(thickness), rotation(rotation)
{
}


Bone::Bone(const Bone &other)
: rotation(other.rotation), length(other.length), thickness(other.thickness)
{
	parent = other.parent;
	for (auto &bone : children) {
		children.push_back(unique_ptr<Bone>(new Bone(*bone)));
	}
}


Bone &Bone::operator =(Bone other)
{
	swap(*this, other);
	return *this;
}


Bone &Skeleton::createBone(
	float length, float thickness, quat rotation)
{
	auto bone = new Bone(nullptr, length, thickness, rotation);
	bones.push_back(unique_ptr<Bone>(bone));

	return *bone;
}


Bone &Skeleton::createBone(
	Bone &parent,
	float length, float thickness, quat rotation)
{
	auto bone = new Bone(&parent, length, thickness, rotation);
	parent.children.push_back(unique_ptr<Bone>(bone));
	
	return *bone;
}
