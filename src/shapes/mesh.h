/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include "../core/vector3d.h"
#include "triangle.h"
#include "../materials/phong.h"
#include "../shapes/sphere.h"
#include "../core/utils.h"

class Mesh : public Shape
{
public:
	
	Mesh() = delete;
	Mesh::Mesh(const char* name,Material *material_);
	
	void clear();
	bool loadOBJ(const char* filename);
	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	std::vector< Vector3D > vertices;
	//std::vector<Vector3D> normals;
	std::vector< Shape*> triangles;
	Vector3D xyzMin, xyzMax;
	Vector3D center, halfSize;
	Sphere * sphereBBox;					
	
};



#endif