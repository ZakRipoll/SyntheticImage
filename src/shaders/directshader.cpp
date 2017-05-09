#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader()
{
}

DirectShader::DirectShader(Vector3D hitColor, Vector3D bgColor_)
{
}

Vector3D DirectShader::computeColor(const Ray & r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Intersection its;

	if (!Utils::getClosestIntersection(r, objList, its))
		return bgColor_;

	Ray wo(its.itsPoint, abs(its.itsPoint - r.o));

	for (int i = 0; i < lsList.size(); i++) 
	{
		Ray wi (its.itsPoint, abs(its.itsPoint - lsList[i].getPosition));

		if (Utils::hasIntersection(wi, objList) || dot(its.normal, wi.d) < 0) continue;


	}

	return Vector3D();
}
