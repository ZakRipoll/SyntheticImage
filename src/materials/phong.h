#ifndef PHONG
#define PHONG

#include "../core/vector3d.h"

#include "material.h"

class Phong : public Material
{
public:
	Phong();
	Phong(Vector3D, Vector3D, int);
	Phong(Vector3D, int);

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const;

	virtual bool hasSpecular() const;
	virtual bool hasTransmission() const;
	virtual bool hasDiffuseOrGlossy() const;
	virtual Vector3D getDiffuseCoefficient() const;
	double Phong::getIndexOfRefraction() const;
	

private:
	Vector3D diffuse;
	Vector3D specular;
	int shininess;
};

#endif //PHONG