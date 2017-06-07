#include "mesh.h"
#include <cassert>

#include <string>
#include <sys/stat.h>

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);
//Vector2 parseVector2(const char* text);
Vector3D parseVector3(const char* text, const char separator);


Mesh::Mesh(const char* name_, Material *material_) : Shape(Matrix4x4(), material_){
	loadOBJ(name_);
	createBoundingBox(xyzMin,xyzMax);
	createSphereBox();
	createVoxels();
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
}

void computemaxMinVertex(Vector3D *xyzMin, Vector3D *xyzMax, Vector3D aux) {

	//computexMIn
	if (xyzMin->x > aux.x) xyzMin->x = aux.x;
	//computexMAx
	else if (xyzMax->x < aux.x) xyzMax->x = aux.x;
	
	if (xyzMin->y > aux.y) xyzMin->y = aux.y;
	else if (xyzMax->y < aux.y) xyzMax->y = aux.y;

	if (xyzMin->z > aux.z) xyzMin->z = aux.z;
	else if (xyzMax->z < aux.z) xyzMax->z = aux.z;
}

bool Mesh::loadOBJ(const char* filename)
{
	struct stat stbuffer;

	std::cout << "Loading Mesh: " << filename << std::endl;

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(filename, &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size + 1];
	fread(data, size, 1, f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[255];
	int i = 0;

	std::vector<Vector3D> indexed_positions;
	std::vector<Vector3D> indexed_normals;
	//std::vector<Vector2D> indexed_uvs;

	const float max_float = 10000000;
	const float min_float = -10000000;

	unsigned int vertex_i = 0;

	xyzMin = Vector3D(max_float);
	xyzMax = Vector3D(min_float);
	//parse file
	while (*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while (i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line, pos, i);
		line[i] = 0;
		pos = pos + i;

		//std::cout << "Line: \"" << line << "\"" << std::endl;
		if (*line == '#' || *line == 0) continue; //comment

		//tokenize line
		std::vector<std::string> tokens = tokenize(line, " ");

		if (tokens.empty()) continue;

		if (tokens[0] == "v" && tokens.size() == 4)
		{
			Vector3D v(atof(tokens[1].c_str()), atof(tokens[2].c_str()), -atof(tokens[3].c_str()));
			computemaxMinVertex(&xyzMin, &xyzMax, v);
			indexed_positions.push_back(v);
		}
		
		else if (tokens[0] == "vn" && tokens.size() == 4)
		{
			Vector3D v(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
			indexed_normals.push_back(v);
		}
		
		else if (tokens[0] == "f" && tokens.size() >= 4)
		{
			Vector3D v1, v2, v3;
			v1 = parseVector3(tokens[1].c_str(), '/');

			for (int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{
				v2 = parseVector3(tokens[iPoly].c_str(), '/');
				v3 = parseVector3(tokens[iPoly + 1].c_str(), '/');

				vertices.push_back(indexed_positions[unsigned int(v1.x) - 1]);
				vertices.push_back(indexed_positions[unsigned int(v2.x) - 1]);
				vertices.push_back(indexed_positions[unsigned int(v3.x) - 1]);

				if (indexed_normals.size() > 0)
				{
					normals.push_back(indexed_normals[unsigned int(v1.z) - 1]);
					normals.push_back(indexed_normals[unsigned int(v2.z) - 1]);
					normals.push_back(indexed_normals[unsigned int(v3.z) - 1]);
				}

				triangles.push_back(new LightTriangle(vertices[vertex_i], vertices[vertex_i + 1], vertices[vertex_i + 2]));

				vertex_i += 3;
			}
		}
	}
	delete data;

	return true;
}

void Mesh::createBoundingBox(Vector3D xyzMin,Vector3D xyzMax)
{
	Vector3D x1(xyzMin.x, xyzMin.y, xyzMin.z);
	Vector3D x2(xyzMax.x, xyzMin.y, xyzMin.z);
	Vector3D x3(xyzMax.x, xyzMax.y, xyzMin.z);
	Vector3D x4(xyzMin.x, xyzMax.y, xyzMin.z);
	Vector3D x5(xyzMin.x, xyzMin.y, xyzMax.z);
	Vector3D x6(xyzMax.x, xyzMin.y, xyzMax.z);
	Vector3D x7(xyzMax.x, xyzMax.y, xyzMax.z);
	Vector3D x8(xyzMin.x, xyzMax.y, xyzMax.z);

	boundingBox.push_back(new Triangle(x1, x2, x3, NULL));
	boundingBox.push_back(new Triangle(x1, x3, x4, NULL));
	boundingBox.push_back(new Triangle(x2, x6, x7, NULL));
	boundingBox.push_back(new Triangle(x2, x7, x3, NULL));
	boundingBox.push_back(new Triangle(x6, x5, x8, NULL));
	boundingBox.push_back(new Triangle(x6, x8, x7, NULL));
	boundingBox.push_back(new Triangle(x5, x1, x4, NULL));
	boundingBox.push_back(new Triangle(x5, x4, x8, NULL));
	boundingBox.push_back(new Triangle(x4, x3, x7, NULL));
	boundingBox.push_back(new Triangle(x4, x7, x8, NULL));
	boundingBox.push_back(new Triangle(x5, x6, x2, NULL));
	boundingBox.push_back(new Triangle(x5, x2, x1, NULL));
}

void Mesh::createSphereBox() {
	center = (xyzMax + xyzMin) * 0.5;
	halfSize = xyzMax - center;

	Material *pink_50 = new Phong(Vector3D(.976, .062, .843), 50);
	Matrix4x4 sphereTransform;
	sphereTransform = sphereTransform.translate(center);
	sphereBBox = new Sphere(halfSize.length(), sphereTransform, pink_50);
}

void Mesh::createVoxels()
{
	Vector3D auxMin, auxMax;
	double dx, dy, dz;
	auxMin = Vector3D(xyzMin);
	auxMax = Vector3D(center);
	dx = (xyzMax.x - auxMin.x) * 0.5;
	dy = (xyzMax.y - auxMin.y) * 0.5; 
	dz = (xyzMax.z - auxMin.z) * 0.5;

	//Voxel1
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel2
	auxMin.x += dx, auxMax.x += dx;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel3
	auxMin.z += dz, auxMax.z += dz;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel4
	auxMin.x -= dx, auxMax.x -= dx;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel5
	auxMin.y += dy, auxMax.y += dy;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel6
	auxMin.x += dx, auxMax.x += dx;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel7
	auxMin.z -= dz, auxMax.z -= dz;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
	//Voxel8
	auxMin.x -= dx, auxMax.x -= dx;
	voxels.push_back(new Voxel(auxMin, auxMax, &triangles));
}

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings )
{
	std::vector<std::string> tokens;

	std::string str;
	char del_size = strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	int i = 0;
	while(*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\'') )
		{ 
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}
			
			in_string = (in_string != 0 ? 0 : *pos );
			if (in_string == 0) 
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}

Vector3D parseVector3(const char* text, const char separator)
{
	int pos = 0;
	char num[255];
	const char* start = text;
	const char* current = text;
	Vector3D result;

	while(1)
	{
		if (*current == separator || (*current == '\0' && current != text) )
		{
			strncpy(num, start, current - start);
			num[current - start] = '\0';
			start = current + 1;
			if (num[0] != 'x') //¿?
				switch(pos)
				{
					case 0: result.x = (float)atof(num); break;
					case 1: result.y = (float)atof(num); break;
					case 2: result.z = (float)atof(num); break;
					default: return result; break;
				}
			++pos;
			if (*current == '\0')
				break;
		}
		++current;
	}
	return result;
};

bool Mesh::rayIntersect(const Ray &ray, Intersection &its) const
{
	bool collide = false;
	if (Utils::hasIntersection(ray, boundingBox)) {
		for (size_t voxInd = 0; voxInd < voxels.size(); voxInd++) {
			if (Utils::hasIntersection(ray, voxels.at(voxInd)->boundingBox)) {
				for (int objindex = 0; objindex < voxels.at(voxInd)->index_triangles.size(); objindex++) {
					int index = voxels.at(voxInd)->index_triangles.at(objindex);
					const LightTriangle * obj = triangles.at(index);
					if (obj->rayIntersect(ray, its)) {
						collide = true;
						its.shape = this;
					}
				}
			}
		}
	}
	return collide;
}

bool Mesh::rayIntersectP(const Ray &ray) const
{
	if (Utils::hasIntersection( ray, boundingBox ) ) {
		for (size_t voxInd = 0; voxInd < voxels.size(); voxInd++) {
			if (Utils::hasIntersection(ray, voxels.at(voxInd)->boundingBox)) {
				for (int objindex = 0; objindex < voxels.at(voxInd)->index_triangles.size(); objindex++) {
					int index = voxels.at(voxInd)->index_triangles.at(objindex);
					const LightTriangle * obj = triangles.at(index);
					if (obj->rayIntersectP(ray)) return true;
				}
			}
		}
	}
	return false;
}

//VOXELS

Voxel::Voxel(Vector3D xyzMin, Vector3D xyzMax, std::vector<LightTriangle*>*triangles) {
	
	this->xyzMin = xyzMin;
	this->xyzMax = xyzMax;
	createBoundingBox(xyzMin, xyzMax);
	
	for (size_t triangle = 0; triangle < triangles->size(); triangle++) {
		LightTriangle * t = triangles->at(triangle);
		std::vector<Vector3D*> * aux = t->getVertex();
		for (size_t vertex = 0; vertex < aux->size(); vertex++) {
			Vector3D *aux2 = aux->at(vertex);
			if (aux2->x >= xyzMin.x && aux2->y >= xyzMin.y && aux2->z >= xyzMin.z && aux2->x <= xyzMax.x && aux2->y <= xyzMax.y && aux2->z <= xyzMax.z) {
				index_triangles.push_back(triangle);
				break;
			}
		}
	}
}

void Voxel::createBoundingBox(Vector3D xyzMin, Vector3D xyzMax)
{
	Vector3D x1(xyzMin.x, xyzMin.y, xyzMin.z);
	Vector3D x2(xyzMax.x, xyzMin.y, xyzMin.z);
	Vector3D x3(xyzMax.x, xyzMax.y, xyzMin.z);
	Vector3D x4(xyzMin.x, xyzMax.y, xyzMin.z);
	Vector3D x5(xyzMin.x, xyzMin.y, xyzMax.z);
	Vector3D x6(xyzMax.x, xyzMin.y, xyzMax.z);
	Vector3D x7(xyzMax.x, xyzMax.y, xyzMax.z);
	Vector3D x8(xyzMin.x, xyzMax.y, xyzMax.z);

	boundingBox.push_back(new Triangle(x1, x2, x3, NULL));
	boundingBox.push_back(new Triangle(x1, x3, x4, NULL));
	boundingBox.push_back(new Triangle(x2, x6, x7, NULL));
	boundingBox.push_back(new Triangle(x2, x7, x3, NULL));
	boundingBox.push_back(new Triangle(x6, x5, x8, NULL));
	boundingBox.push_back(new Triangle(x6, x8, x7, NULL));
	boundingBox.push_back(new Triangle(x5, x1, x4, NULL));
	boundingBox.push_back(new Triangle(x5, x4, x8, NULL));
	boundingBox.push_back(new Triangle(x4, x3, x7, NULL));
	boundingBox.push_back(new Triangle(x4, x7, x8, NULL));
	boundingBox.push_back(new Triangle(x5, x6, x2, NULL));
	boundingBox.push_back(new Triangle(x5, x2, x1, NULL));
}
