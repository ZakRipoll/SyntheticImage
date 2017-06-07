#include "lighttriangle.h"

LightTriangle::LightTriangle(const Vector3D a_, const Vector3D b_, const Vector3D c_, const Vector3D normal_): aWorld(a_),bWorld(b_),cWorld(c_), nWorld(normal_)
{
	vab = (bWorld - aWorld);
	vca = (aWorld - cWorld);
	vac = (cWorld - aWorld);
	vbc = (cWorld - bWorld);

	vertex = new std::vector<Vector3D*>();
	vertex->push_back(&aWorld);
	vertex->push_back(&bWorld);
	vertex->push_back(&cWorld);
}

LightTriangle::LightTriangle(const Vector3D a_, const Vector3D b_, const Vector3D c_) : aWorld(a_), bWorld(b_), cWorld(c_)
{

	vab = (bWorld - aWorld);
	vca = (aWorld - cWorld);
	vac = (cWorld - aWorld);
	vbc = (cWorld - bWorld);
	nWorld = cross(vac, vab).normalized();
	vertex = new std::vector<Vector3D*>();
	vertex->push_back(&aWorld);
	vertex->push_back(&bWorld);
	vertex->push_back(&cWorld);
}


Vector3D LightTriangle::getNormalWorld(const Vector3D & pt_world) const	
{
	return nWorld;
}

bool LightTriangle::rayIntersect(const Ray & ray, Intersection & its) const
{
	double denominator = dot(ray.d, nWorld);

	if (std::abs(denominator) < Epsilon) return false;

	double tHit = dot((aWorld - ray.o), nWorld) / denominator;

	if (tHit < ray.minT || tHit > ray.maxT) return false;

	Vector3D phit = ray.o + ray.d * tHit;

	//Infinite Plane
	//double comp = 0.0;
	if (dot(cross(vab, (phit - aWorld)), nWorld) > 0) return false;

	if (dot(cross(vbc, (phit - bWorld)), nWorld) > 0) return false;

	if (dot(cross(vca, (phit - cWorld)), nWorld) > 0) return false;


	// Fill the intersection details
	its.itsPoint = phit;
	its.normal = nWorld;
	//its.shape = this;

	// Update the ray maxT
	ray.maxT = tHit;

	return true;
}

bool LightTriangle::rayIntersectP(const Ray & ray) const
{
	double denominator = dot(ray.d, nWorld);

	if (std::abs(denominator) < Epsilon) return false;

	double tHit = dot((aWorld - ray.o), nWorld) / denominator;

	if (tHit < ray.minT || tHit > ray.maxT) return false;

	Vector3D phit = ray.o + ray.d * tHit;

	//Infinite Plane
	double comp = 0.0;
	if (dot(cross(vab, (phit - aWorld)), nWorld) > comp) return false;

	if (dot(cross(vbc, (phit - bWorld)), nWorld) > comp) return false;

	if (dot(cross(vca, (phit - cWorld)), nWorld) > comp) return false;

	return true;
}

std::vector<Vector3D*>* LightTriangle::getVertex() const
{
	return vertex;
}

std::string LightTriangle::toString() const
{
	return std::string();
}

std::ostream & operator<<(std::ostream & out, const LightTriangle & s)
{
	return out;
}
