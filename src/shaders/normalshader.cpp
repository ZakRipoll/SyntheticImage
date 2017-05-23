#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader()
{}

NormalShader::NormalShader(Vector3D bgColor_) :
    Shader(bgColor_)
{}

Vector3D NormalShader::computeColor(const Ray &r, const std::vector<Shape *> &objList, const std::vector<PointLightSource> &lsList) const
{
    Intersection its;

	return Utils::getClosestIntersection(r, objList, its) ? (its.normal + Vector3D(1.0)) * .5 : bgColor;
}
