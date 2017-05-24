﻿#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplane.h"
#include "shapes/triangle.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/directshader.h"
#include "shaders/normalshader.h"
#include "shaders/globalshader.h"

#include "materials\phong.h"
#include "materials\mirror.h"
#include "materials\transmisive.h"

void buildSceneCornellBox(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *transmissive = new Transmisive(1.1, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);

	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);

	// Place the Spheres inside the Cornell Box
	Matrix4x4 sphereTransform1;
	double radius = 1;
	sphereTransform1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 3.5));
	Shape *s1 = new Sphere(1.5, sphereTransform1, mirror);
	Matrix4x4 sphereTransform2;
	sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, 0.0, 2));
	Shape *s2 = new Sphere(1, sphereTransform2, transmissive);
	Matrix4x4 sphereTransform3;
	radius = 1;
	sphereTransform3 = Matrix4x4::translate(Vector3D(0.3, -offset + radius, 5));
	Shape *s3 = new Sphere(radius, sphereTransform3, red_100);
	objectsList->push_back(s1);
	objectsList->push_back(s2);
	objectsList->push_back(s3);

	if (!0) {
		float p = 0.5;
		Shape *triangle = new Triangle(Vector3D(0.6, -0.6, p), Vector3D(1.4, -0.6, p), Vector3D(1, 0.6, p+1), mirror);
		objectsList->push_back(triangle);
	}

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D intensity = Vector3D(10, 10, 10); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}

void buildSceneTraiangle(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;

	Shape *triangle = new Triangle(Vector3D(-2, -1.7, 0), Vector3D(2, -1.7, 0), Vector3D(0, 1.75, 0), redDiffuse);
	objectsList->push_back(triangle);


	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition3 = Vector3D(0, 0 , -1);
	Vector3D intensity = Vector3D(10, 10, 10); // Radiant intensity (watts/sr)
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS3);
}

void buildSceneSphere(Camera* &cam, Film* &film,
                      std::vector<Shape*>* &objectsList,
                      std::vector<PointLightSource>* &lightSourceList)
{
    /* **************************** */
    /* Declare and place the camera */
    /* **************************** */
    // By default, this gives an ID transform
    //  which means that the camera is located at (0, 0, 0)
    //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */
    Material *green_50 = new Phong(Vector3D(0.2, 0.7, 0.3), 50);
	Material *purple_100 = new Phong(Vector3D(.305, .062, .698), 100);
	Material *pink_50 = new Phong(Vector3D(.976, .062, .843), 50);
    
	/* ******* */
    /* Objects */
    /* ******* */
    // Create a heterogeneous list of objects of type shape
    // (some might be triangles, other spheres, plans, etc)
    objectsList = new std::vector<Shape*>;

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-1.0, -0.5, 2*std::sqrt(2.0)));
    Shape *s1 = new Sphere (0.25, sphereTransform1, green_50);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.0, 0.0, 6));
    Shape *s2 = new Sphere (1, sphereTransform2, purple_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(0.3, -0.75, 3.5));
    Shape *s3 = new Sphere (0.25, sphereTransform3, pink_50);
	
	if (!0) {
		int p = 2;
		Shape *triangle = new Triangle(Vector3D(-1.0, -0.5, 2 * std::sqrt(2.0)), Vector3D(0.3, -0.75, 3.5),/*Big*/Vector3D(1.0, 0.0, 6), green_50);
		objectsList->push_back(triangle);
	}

    // Store the objects in the object list
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);

	// Infinite Plane
	if (!0) {
		Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
		double offset = 3.0;
		Shape *bottomPlane = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
		objectsList->push_back(bottomPlane);
	}
	

    /* ****** */
    /* Lights */
    /* ****** */

	lightSourceList = new std::vector<PointLightSource>;

	PointLightSource front(Vector3D(0, 0, -10), Vector3D(60));
	PointLightSource top(Vector3D(0, 10, 0), Vector3D(60));
	PointLightSource right(-Vector3D(-10, 0, 0), Vector3D(60));

	lightSourceList->push_back(front);
	lightSourceList->push_back(top);
	lightSourceList->push_back(right);
}


void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + .5) / resX;
            double y = (double)(lin + .5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList );

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 576);


    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
	Shader *shader;
	
	if (0)
		shader = new IntersectionShader(intersectionColor, bgColor);
	else if (0)
		shader = new DepthShader(Vector3D(.4, 1, .4), 8, bgColor);
	else if (0)
		shader = new DirectShader(bgColor);
	else
		shader = new GlobalShader(bgColor, 2, 1);

    // Declare pointers to all the variables which describe the scene
    Camera *cam;
    std::vector<Shape*> *objectsList;
    std::vector<PointLightSource> *lightSourceList;

    // Build the scene
	if (0)buildSceneSphere(cam, film, objectsList, lightSourceList);
	else buildSceneCornellBox(cam, film, objectsList, lightSourceList);

    // Launch some rays!
    raytrace(cam, shader, film, objectsList, lightSourceList);

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
