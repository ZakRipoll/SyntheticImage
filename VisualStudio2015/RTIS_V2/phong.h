#ifndef PHONG
#define PHONG

class Phong
{
public:
	Phong();
	virtual bool hasSpecular() const = 0;
	virtual bool hasTransmission() const = 0;
	virtual bool hasDiffuseOrGlossy() const = 0;
	virtual double getIndexOfRefraction() const;
};

#endif //PHONG