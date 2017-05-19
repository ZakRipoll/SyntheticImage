#ifndef TRIANGLE
#define TRIANGLE

#include <iostream>
#include <string>

#include "shape.h"
#include "../core/eqsolver.h"

class Triangle : public Shape
{
public:
	Triangle() = delete;
	Triangle(const Vector3D a, const Vector3D b, const Vector3D c, Material *material_);

	Vector3D getNormalWorld(const Vector3D &pt_world) const;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	std::string toString() const;

private:
	Vector3D aWorld;
	Vector3D bWorld;
	Vector3D cWorld;
	Vector3D nWorld;
};

std::ostream& operator<<(std::ostream &out, const Triangle &s);

#endif // TRIANGLE
