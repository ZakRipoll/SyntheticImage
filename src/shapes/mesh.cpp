#include "mesh.h"
#include <cassert>

#include <string>
#include <sys/stat.h>

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);
//Vector2 parseVector2(const char* text);
Vector3D parseVector3(const char* text, const char separator);


Mesh::Mesh(const char* name_, Material *material_) : Shape(Matrix4x4(), material_){
	loadOBJ(name_);
}


void Mesh::clear()
{
	vertices.clear();
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

	FILE* f = fopen(filename,"rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(filename, &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size+1];
	fread(data,size,1,f);
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

	xyzMin = Vector3D (max_float);
	xyzMax = Vector3D (min_float);
	//parse file
	while(*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while(i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line,pos,i);
		line[i] = 0;
		pos = pos + i;

		//std::cout << "Line: \"" << line << "\"" << std::endl;
		if (*line == '#' || *line == 0) continue; //comment

		//tokenize line
		std::vector<std::string> tokens = tokenize(line," ");

		if (tokens.empty()) continue;

		if (tokens[0] == "v" && tokens.size() == 4)
		{
			Vector3D v( atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()) );
			computemaxMinVertex(&xyzMin, &xyzMax, v);
			indexed_positions.push_back(v);
		}
		/*else if (tokens[0] == "vt" && tokens.size() == 4)
		{
			Vector2 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()) );
			indexed_uvs.push_back(v);
		}*/
		else if (tokens[0] == "vn" && tokens.size() == 4)
		{
			Vector3D v( atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()) );
			indexed_normals.push_back(v);
		}
		/*else if (tokens[0] == "s") //surface? it appears one time before the faces ????????????????????????
		{
			//process mesh
			if (uvs.size() == 0 && indexed_uvs.size() )
				uvs.resize(1);
		}*/
		else if (tokens[0] == "f" && tokens.size() >= 4)
		{
			Vector3D v1,v2,v3;
			v1 = parseVector3( tokens[1].c_str(), '/' );

			for (int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{
				v2 = parseVector3( tokens[iPoly].c_str(), '/' );
				v3 = parseVector3( tokens[iPoly+1].c_str(), '/' );

				vertices.push_back( indexed_positions[ unsigned int(v1.x) -1 ] );
				vertices.push_back( indexed_positions[ unsigned int(v2.x) -1] );
				vertices.push_back( indexed_positions[ unsigned int(v3.x) -1] );

				/*triangles.push_back(new Triangle( indexed_positions[unsigned int(v1.x) - 1], 
					indexed_positions[unsigned int(v2.x) - 1],
					indexed_positions[unsigned int(v3.x) - 1], new Phong( normal que sigui, 50) ) ); //not needed
				*/
				triangles.push_back( new Triangle(vertices[vertex_i], vertices[vertex_i+1], vertices[vertex_i+2], material)); //not needed

				vertex_i += 3;

				/*if (indexed_uvs.size() > 0)
				{
					uvs.push_back( indexed_uvs[ unsigned int(v1.y) -1] );
					uvs.push_back( indexed_uvs[ unsigned int(v2.y) -1] );
					uvs.push_back( indexed_uvs[ unsigned int(v3.y) -1] );
				}*/

				/*
				if (indexed_normals.size() > 0)
				{
					normals.push_back( indexed_normals[ unsigned int(v1.z) -1] );
					normals.push_back( indexed_normals[ unsigned int(v2.z) -1] );
					normals.push_back( indexed_normals[ unsigned int(v3.z) -1] );
				}*/
			}
		}
	}

	center = (xyzMax + xyzMin) * 0.5;
	halfSize = xyzMax - center;

	Material *pink_50 = new Phong(Vector3D(.976, .062, .843), 50);
	Matrix4x4 sphereTransform;
	sphereTransform = sphereTransform.translate(center);
	sphereBBox = new Sphere(halfSize.length(),sphereTransform,pink_50);
	delete data;

	return true;
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

// Chapter 3 PBRT, page 117
bool Mesh::rayIntersect(const Ray &ray, Intersection &its) const
{
	if (sphereBBox->rayIntersectP(ray)) {
		for (int objindex = 0; objindex < triangles.size(); objindex++) {
			bool firsthit = triangles.at(objindex)->rayIntersectP(ray);
			if (firsthit) {
				bool golpea = triangles.at(objindex)->rayIntersect(ray, its);
				return golpea;
			}
		}
	}
	return false;
}
// Chapter 3 PBRT, page 117
bool Mesh::rayIntersectP(const Ray &ray) const
{
	if (sphereBBox->rayIntersectP(ray)) {
		
		for (int objindex = 0; objindex < triangles.size(); objindex++) {
			return triangles.at(objindex)->rayIntersectP(ray);
		}
	}
	return false;
}