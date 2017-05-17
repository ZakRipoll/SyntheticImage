#ifndef TRANSMISIVE
#define TRANSMISIVE

#include "../core/vector3d.h"
#include "material.h"

class Transmisive : public  Material
{
	public:
		Transmisive();
		Transmisive(double, Vector3D spec);

		 virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,const Vector3D &wi) const;
		 virtual bool hasSpecular() const;
		 virtual bool hasTransmission() const;
		 virtual bool hasDiffuseOrGlossy() const;
		 virtual double getIndexOfRefraction() const;
		 double getEta();

	private:
		double refractionIndex;
		Vector3D specular;
};


#endif // TRANSMISIVE
