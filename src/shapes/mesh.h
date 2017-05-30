/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include "../core/vector3d.h"
#include "triangle.h"

class Mesh
{
public:
	std::vector< Vector3D > vertices;	//here we store the vertices
	std::vector< Vector3D > normals;	//here we store the normals
	std::vector< Shape > triangles;	//here we store the triangles
	//std::vector< Vector2 > uvs;		//here we store the texture coordinates, OPCIONAL XD

	Mesh();
	void clear();

	void createPlane(float size);
	bool loadOBJ(const char* filename);
};



#endif