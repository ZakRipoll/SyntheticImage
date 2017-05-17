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

	if (its.shape->getMaterial().hasSpecular()) {
		Vector3D wr = Utils::computeReflectionDirection(wo,its.normal);
			//its.shape->getMaterial().getReflectance(its.normal, wo, NULL);
		Ray refRay = Ray(its.itsPoint, wr, r.depth + 1);
		return computeColor(refRay, objList, lsList);
	}

	for (int i = 0; i < lsList.size(); i++) 
	{
		Vector3D direction (lsList[i].getPosition() - its.itsPoint );
		float distanceLS = direction.length();
		Vector3D wi = direction.normalized();

		Ray shadowRay(its.itsPoint, wi);
		shadowRay.maxT = distanceLS;

		if (!Utils::hasIntersection(shadowRay, objList) && dot(its.normal, wi) > 0) {
			color += Utils::multiplyPerCanal(its.shape->getMaterial().getReflectance(its.normal, wo, wi),
				lsList[i].getIntensity(its.itsPoint));
		}
	}

	return color;
}
