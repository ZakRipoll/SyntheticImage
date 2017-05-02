#include "sphere.h"

Sphere::Sphere(const double radius_, const Matrix4x4 &t_)
    : Shape(t_), radius(radius_)
{ }

bool Sphere::rayIntersectP(const Ray &ray) const
{
    // Pass the ray to local coordinates
    Ray r = worldToObject.transformRay(ray);

    // The ray-sphere intersection equation can be expressed in the
    double A = pow(r.d.x,2) + pow(r.d.y, 2) + pow(r.d.z, 2);
    double B = 2 * ((r.d.x*r.o.x) + (r.d.y* r.o.y) + (r.d.z*r.o.z));
    double C = pow(r.o.x, 2) + pow(r.o.y, 2) + pow(r.o.z, 2) - pow(radius,2);

    // Now we need to solve this quadratic equation for t
    EqSolver solver;
    rootValues roots;

    return solver.rootQuadEq(A, B, C, roots);
}

std::string Sphere::toString() const
{
    std::stringstream s;
    s << "[ " << std::endl
      << " Center (World) = " << objectToWorld.transformPoint(Vector3D(0,0,0)) << ", Radius = " << radius << std::endl
      << "]" << std::endl;

    return s.str();
}

std::ostream& operator<<(std::ostream &out, const Sphere &s)
{
    out << s.toString();
    return out;
}
