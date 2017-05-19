#include "mirror.h"

Mirror::Mirror() {}

Mirror::Mirror(Vector3D spec) : specular(spec)
{}

Vector3D Mirror::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	return  n * dot(n, wo) * 2.0 - wo;
}

bool Mirror::hasSpecular() const
{
	return true;
}

bool Mirror::hasTransmission() const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}

double Mirror::getIndexOfRefraction() const
{
	return 0.0;
}
