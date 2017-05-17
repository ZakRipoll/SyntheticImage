#include "phong.h"

#include <iostream>
#include <math.h>       /* cos */

Phong::Phong()
{ }

Phong::Phong(Vector3D diffuse_, Vector3D specular_, int shininess_) : diffuse(diffuse_), specular(specular_), shininess(shininess_)
{
}

Phong::Phong(Vector3D diffuse_, int shininess_) : diffuse(diffuse_), specular(Vector3D(1) - diffuse_), shininess(shininess_)
{
}

bool Phong::hasSpecular() const
{
	// This material does not have specular component
	return false;
}

bool Phong::hasTransmission() const
{
	return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
	return true;
}

double Phong::getIndexOfRefraction() const
{
	return 0.0;
}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{
	Vector3D diffuse_reflectance = diffuse * dot(n.normalized(), wi.normalized());

	Vector3D wr = n * dot(n, wi) * 2.0 - wi;

	Vector3D specular_reflection = specular * pow(dot(wo.normalized(), wr.normalized()), shininess);
	return diffuse_reflectance + specular_reflection;
}