#include "phong.h"

// Implement the Constructors here
//
// (...)
//

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

// Implement the function "getReflectance" here
//
// (...)
//