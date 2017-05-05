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