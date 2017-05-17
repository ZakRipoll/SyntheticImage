#include "transmisive.h"

Transmisive::Transmisive() {}

Transmisive::Transmisive(double eta_, Vector3D spec) : refractionIndex(eta_), specular(spec)
{
}

Vector3D Transmisive::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	return  n * dot(n, wo) * 2.0 - wo;
}

bool Transmisive::hasSpecular() const
{
	return false;
}

bool Transmisive::hasTransmission() const
{
	return true;
}

bool Transmisive::hasDiffuseOrGlossy() const
{
	return false;
}

double Transmisive::getIndexOfRefraction() const
{
	return refractionIndex;
}