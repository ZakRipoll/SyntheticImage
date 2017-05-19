#include "triangle.h"

Triangle::Triangle(const Vector3D a_, const Vector3D b_, const Vector3D c_, Material * material_):Shape(Matrix4x4(),material_), aWorld(a_),bWorld(b_),cWorld(c_)
{
	nWorld = cross((cWorld - aWorld), (bWorld - aWorld));
}

Vector3D Triangle::getNormalWorld(const Vector3D & pt_world) const
{
	return Vector3D();
}

bool Triangle::rayIntersect(const Ray & ray, Intersection & its) const
{

	return false;
}

bool Triangle::rayIntersectT(const Ray & ray) const
{
	return false;
}

std::string Triangle::toString() const
{
	return std::string();
}

std::ostream & operator<<(std::ostream & out, const Triangle & s)
{
	return out;
}
