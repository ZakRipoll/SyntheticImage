#include <iostream>
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
#include "shapes/mesh.h"

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
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, 
	bool reflect, bool snell, bool triangle)
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
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	Material *transmissive, *mirror;

	if( reflect )
		mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	else
		mirror = new Phong(Vector3D(0.0, 0.9, 0.9), Vector3D(0.1, 0.9, 0.9), 50);

	if( snell )
		transmissive = new Transmisive(1.1, Vector3D(1));
	else
		transmissive = new Phong(Vector3D(1, 1, 0.2), Vector3D(1, 1, 0.2), 20);

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

	if (triangle) {
		float p = 0.5;
		Shape *triangle = new Triangle(Vector3D(0.6, -0.6, p), Vector3D(1.4, -0.6, p), Vector3D(1, 0.6, p + 1), mirror);
		objectsList->push_back(triangle);
	}

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D intensity = Vector3D(5, 5, 5); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}


void buildSceneSphere(Camera* &cam, Film* &film, std::vector<Shape*>* &objectsList,
    std::vector<PointLightSource>* &lightSourceList, 
	bool plane, bool triangle)
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
	
	if (triangle) {
		Shape *triangle = new Triangle(Vector3D(-1.0, -0.5, 2 * std::sqrt(2.0)), Vector3D(0.3, -0.75, 3.5),/*Big*/Vector3D(1.0, 0.0, 6), green_50);
		objectsList->push_back(triangle);
	}

    // Store the objects in the object list
    objectsList->push_back(s1);
    objectsList->push_back(s2);
    objectsList->push_back(s3);

	// Infinite Plane
	if (plane) {
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

void buildOurScene(Camera* &cam, Film* &film, std::vector<Shape*>* &objectsList ,std::vector<PointLightSource>* &lightSourceList)
{
	objectsList = new std::vector<Shape*>;
	/* OIBJECTS*/
	Material* meshMaterial = new Phong(Vector3D(0.2, 0.7, 0.3), 50);
	Shape* mesh = new Mesh("lee.obj", meshMaterial);
	objectsList->push_back(mesh);

	Mesh * m = (Mesh*)mesh;
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(m->center.x, m->center.y, -50));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);
	
	
	double offset = 50;

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

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

	//Place the Spheres inside the Cornell Box
	Material * mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material * transmissive = new Transmisive(1.1, Vector3D(1));
	Matrix4x4 sphereTransform1,sphereTransform2;
	double radius = 10;
	sphereTransform1 = Matrix4x4::translate(Vector3D(m->center.x-m->halfSize.length(),m->center.y , m->center.z + 2*radius));
	sphereTransform2 = Matrix4x4::translate(Vector3D(m->center.x+ m->halfSize.length(), m->center.y, m->center.z + 2*radius));
	Shape *s1 = new Sphere(radius, sphereTransform1, transmissive);
	Shape *s2 = new Sphere(radius, sphereTransform2, mirror);
	objectsList->push_back(s1);
	objectsList->push_back(s2);

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(offset - 10, 10, -35);
	Vector3D lightPosition2 = Vector3D(-offset + 10, 10 , -35);
	Vector3D lightPosition3 = Vector3D(0, 0, offset - 10);
	Vector3D intensity = Vector3D(3000); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*> * &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();
    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
	int red = 0 ; 
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

			Vector3D pixelColor = Vector3D(0);
            //Compute ray color according to the used shader
			pixelColor = shader->computeColor(cameraRay, *objectsList, *lightSourceList);
			// Store the pixel color
			film->setPixelValue(col, lin, pixelColor);

        }
    }
}

int menu() {

	int option;

	std::cout << "Menu" << std::endl;

	std::cout << "Assignment 3" << std::endl;
	std::cout << "\t 0: 3.2 Intersection Shader" << std::endl;
	std::cout << "\t 1: 4.2 Depth Shader" << std::endl;
	std::cout << "\t 2: 6.2 Direct Illumination Shader" << std::endl;

	std::cout << "Assignment 4" << std::endl;
	std::cout << "\t 3: 2 The Infinite Plane Class" << std::endl;
	std::cout << "\t 4: 2.2 Cornell box" << std::endl;
	std::cout << "\t 5: 3.2 Mirror material" << std::endl;
	std::cout << "\t 6: 3.3 Transmissive material" << std::endl;
	std::cout << "\t 7: 3.4 Ray triangle" << std::endl;

	std::cout << "Assignment 5" << std::endl;
	std::cout << "\t 8: 3.2 (2-bounces Indirect Illumination)" << std::endl;
	std::cout << "\t 9: 3.2 (n-bounces Indirect Illumination)" << std::endl;

	std::cout << "\nSelect one option: ";

	std::cin >> option;

	return option;
}

void executeMenu(int option, Camera* &cam, Shader* &shader, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, 
	Vector3D bgColor, Vector3D intersectionColor, std::string &fileName) {

	switch (option) {

	case 0:
		shader = new IntersectionShader(intersectionColor, bgColor);
		buildSceneSphere(cam, film, objectsList, lightSourceList, 0, 0);
		fileName = "0 Intersection Shader";
		break;

	case 1:
		shader = new DepthShader(Vector3D(.4, 1, .4), 8, bgColor);
		buildSceneSphere(cam, film, objectsList, lightSourceList, 0, 0);
		fileName = "1 Depth Shader";
		break;

	case 2:
		shader = new DirectShader(bgColor);
		buildSceneSphere(cam, film, objectsList, lightSourceList, 0, 0);
		fileName = "2 Direct Illumination Shader";
		break;

	case 3:
		shader = new DirectShader(bgColor);
		buildSceneSphere(cam, film, objectsList, lightSourceList, 1, 0);
		fileName = "3 The Infinite Plane Class";
		break;

	case 4:
		shader = new DirectShader(bgColor);
		buildSceneCornellBox(cam, film, objectsList, lightSourceList, 0, 0, 0);
		fileName = "4 Cornell box";
		break;

	case 5:
		shader = new DirectShader(bgColor);
		buildSceneCornellBox(cam, film, objectsList, lightSourceList, 1, 0, 0);
		fileName = "5 Mirror material";
		break;

	case 6:
		shader = new DirectShader(bgColor);
		buildSceneCornellBox(cam, film, objectsList, lightSourceList, 1, 1, 0);
		fileName = "6 Transmissive material";
		break;

	case 7:
		shader = new DirectShader(bgColor);
		buildSceneCornellBox(cam, film, objectsList, lightSourceList, 1, 1, 1);
		fileName = "7 Ray triangle";
		break;

	case 8:
		int raynum;
		std::cout << "Number of Rays: ";
		std::cin >> raynum;
		shader = new GlobalShader(bgColor, raynum, 2);
		buildSceneCornellBox(cam, film, objectsList, lightSourceList, 1, 1, 1);
		fileName = "8 (2-bounces Indirect Illumination";
		break;

	case 9:
		int bounces,rays;
		std::cout << "Number of Rays: ";
		std::cin >> rays;
		std::cout << "Number of bounces: ";
		std::cin >> bounces;
		

		shader = new GlobalShader(bgColor, rays, bounces);
		buildSceneCornellBox(cam, film, objectsList, lightSourceList, 1, 1, 1);
		fileName = "9 (n-bounces Indirect Illumination";
		break;

	default:
		exit(0);
	}
	

	fileName += ".bmp";
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 576);

	//EVALUATION
	string fileName = "./output.bmp";

    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
	Shader *shader;

	// Declare pointers to all the variables which describe the scene
	Camera *cam;
	std::vector<Shape*> *objectsList;
	std::vector<PointLightSource> *lightSourceList;

	//executeMenu(menu(), cam, shader, film, objectsList, lightSourceList, bgColor, intersectionColor, fileName);

	buildOurScene(cam, film,objectsList,lightSourceList);

	//shader = new NormalShader();
	shader = new DirectShader(Vector3D(0));
	//shader = new IntersectionShader(Vector3D(1, 0, 0), bgColor);

	// Launch some rays!
	raytrace(cam, shader, film, objectsList, lightSourceList);

	// Save the final result to file
	std::cout << "\n\nSaving the result to file " << fileName << "\n" << std::endl;
	film->save(fileName);

	std::cout << "\n\n" << std::endl;
	
    return 0;
}
