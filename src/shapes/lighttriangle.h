#ifndef LIGHT_TRIANGLE_H
#define LIGHT_TRIANGLE_H

#include <iostream>
#include <string>
#include "../core/eqsolver.h"
#include <vector>
#include "../core/utils.h"

class LightTriangle
{
public:
	LightTriangle() = delete;
	LightTriangle(const Vector3D a, const Vector3D b, const Vector3D c, const Vector3D normal);
	LightTriangle(const Vector3D a, const Vector3D b, const Vector3D c);

	Vector3D getNormalWorld(const Vector3D &pt_world) const;

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;
	std::vector<Vector3D*> * getVertex() const;
	std::string toString() const;

	

private:
	Vector3D aWorld;
	Vector3D bWorld;
	Vector3D cWorld;
	Vector3D nWorld;
	Vector3D vab, vac, vca, vbc;
	std::vector<Vector3D*> *vertex;
};


#endif // TRIANGLE
