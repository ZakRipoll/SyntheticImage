#ifndef GLOBALSHADER
#define GLOBALSHADER

#include "shader.h"

class GlobalShader : public Shader
{
public:
	GlobalShader();
	GlobalShader(Vector3D bgColor_);

	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

	Vector3D bgColor;
};
#endif // !GLOBALSHADER
