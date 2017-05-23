#include "directshader.h"
#include "../core/utils.h"
#include "../materials/transmisive.h"

DirectShader::DirectShader()
{
}

DirectShader::DirectShader(Vector3D bgColor_) : bgColor(bgColor_)
{
}

Vector3D DirectShader::computeColor(const Ray & r, const std::vector<Shape*>& objList, 
	const std::vector<PointLightSource>& lsList) const
{
	Intersection its;

	if (!Utils::getClosestIntersection(r, objList, its)) return bgColor;

	Vector3D color, wr, wo = -r.d.normalized();

	Ray refRay;

	if (its.shape->getMaterial().hasSpecular()) {
		wr = Utils::computeReflectionDirection(r.d,its.normal);
		refRay = Ray(its.itsPoint, wr, r.depth + 1);
		return computeColor(refRay, objList, lsList);
	}

	if (its.shape->getMaterial().hasTransmission()) {
		double eta, cosThetaI, cosThetaT_out;
		Vector3D normal = its.normal.normalized();

		cosThetaI = dot(normal, wo);
		eta = its.shape->getMaterial().getIndexOfRefraction();
		
		if (cosThetaI < 0) {
			eta = 1 / eta;
			normal = -normal;
			cosThetaI = dot(wo, normal);
		}


		if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT_out))
			wr = Utils::computeTransmissionDirection(r, normal, eta, cosThetaI, cosThetaT_out);

		else
			wr = Utils::computeReflectionDirection(r.d, normal);

		refRay = Ray(its.itsPoint, wr, r.depth + 1);
		return computeColor(refRay, objList, lsList);
	}

	for (int i = 0; i < lsList.size(); i++) 
	{
		Vector3D direction (lsList[i].getPosition() - its.itsPoint );
		double distanceLS = direction.length();
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
