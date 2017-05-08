#include "phong.h"

#include <iostream>

Phong::Phong()
{ }

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
	// Implement the function "getReflectance" here
	//
	// (...)
	//

	return 0.0;
}

Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const
{
	return cross( cross(n, wi), n) * 2.0 - wi;
}