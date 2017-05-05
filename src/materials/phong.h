#ifndef PHONG
#define PHONG

#include "../core/vector3d.h"

class Phong
{
public:
	Phong();

	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
		const Vector3D &wi) const = 0;

	virtual bool hasSpecular() const = 0;
	virtual bool hasTransmission() const = 0;
	virtual bool hasDiffuseOrGlossy() const = 0;
	virtual double getIndexOfRefraction() const;
};

#endif //PHONG