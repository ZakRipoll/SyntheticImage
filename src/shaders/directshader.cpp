#include "directshader.h"
#include "../core/utils.h"

DirectShader::DirectShader()
{
}

DirectShader::DirectShader(Vector3D bgColor_) : bgColor(bgColor_)
{
}

Vector3D DirectShader::computeColor(const Ray & r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Intersection its;

	if (!Utils::getClosestIntersection(r, objList, its)) return bgColor;

	Vector3D color;

	Vector3D wo = -r.d;

	for (int i = 0; i < lsList.size(); i++) 
	{
		Vector3D direction (its.itsPoint - lsList[i].getPosition());

		Ray wi (its.itsPoint, direction);

		if (Utils::hasIntersection(wi, objList) || dot(its.normal, wi.d) < 0) continue;

		color += Utils::multiplyPerCanal(its.shape->getMaterial().getReflectance(its.normal, wo, wi.d), lsList[i].getIntensity(its.itsPoint));
	}

	return color;
}
