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
#include <time.h>

clock_t tStart;
clock_t tEnd;

void buildOurScene(Camera* &cam, Film* &film, std::vector<Shape*>* &objectsList ,std::vector<PointLightSource>* &lightSourceList, bool planes, bool spheres)
{
	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *golbat = new Phong(Vector3D(0.556,0.219,0.556), Vector3D(), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	Material * mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material * transmissive = new Transmisive(1.1, Vector3D(1));

	objectsList = new std::vector<Shape*>;
	/* OIBJECTS*/
	Material* meshMaterial = new Phong(Vector3D(0.2, 0.7, 0.3), 50);
	Shape* mesh = new Mesh("lee.obj", golbat);
	objectsList->push_back(mesh);

	Mesh * m = (Mesh*)mesh;
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(m->center.x, m->center.y, m->center.z - 3*m->halfSize.length()));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);
	
	double offset = m->halfSize.length();
	
	if (planes) {
		
		// Construct the Cornell Box
		//Shape *leftPlan = new InfinitePlane(Vector3D(m->center.x -offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
		//Shape *rightPlan = new InfinitePlane(Vector3D(m->center.x + offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
		//Shape *topPlan = new InfinitePlane(Vector3D(0, m->center.y + offset, 0), Vector3D(0, -1, 0), greyDiffuse);
		Shape *bottomPlan = new InfinitePlane(Vector3D(0, m->center.y -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
		Shape *backPlan = new InfinitePlane(Vector3D(0, 0, m->center.z + offset), Vector3D(0, 0, -1), greenDiffuse);

		//objectsList->push_back(leftPlan);
		//objectsList->push_back(rightPlan);
		//objectsList->push_back(topPlan);
		objectsList->push_back(bottomPlan);
		objectsList->push_back(backPlan);
	}

	//Place the Spheres inside the Cornell Box
	if (spheres) {
		Matrix4x4 sphereTransform1, sphereTransform2;
		double radius = 10;
		sphereTransform1 = Matrix4x4::translate(Vector3D(m->center.x - m->halfSize.length(), m->center.y, m->center.z + 2 * radius));
		sphereTransform2 = Matrix4x4::translate(Vector3D(m->center.x + m->halfSize.length(), m->center.y, m->center.z + 2 * radius));
		Shape *s1 = new Sphere(radius, sphereTransform1, mirror);
		Shape *s2 = new Sphere(radius, sphereTransform2, transmissive);
		objectsList->push_back(s1);
		objectsList->push_back(s2);
	}

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(m->center.x - offset , m->center.y + offset/2 , m->center.z - offset/2);
	Vector3D lightPosition2 = Vector3D(0 , m->center.y + offset, m->center.z - offset);
	Vector3D lightPosition3 = Vector3D(m->center.x + offset , m->center.y + offset/2, m->center.z - offset/2);
	Vector3D intensity = Vector3D(40 * offset); 
	
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

	std::cout << "\tMenu" << std::endl << std::endl;

	std::cout << "\tProject 1 -> Mesh Loader" << std::endl << std::endl;
	std::cout << "\t\t 0: Intersection Shader" << std::endl <<std::endl;
	std::cout << "\t\t 1: Normal Shader" << std::endl << std::endl;
	std::cout << "\t\t 2: Direct Illumination Shader" << std::endl << std::endl;
	std::cout << "\t\t 3: Direct Illumination Shader with Spheres" << std::endl << std::endl;
	std::cout << "\t\t 4: Global Illumination Shader" << std::endl;

	std::cout << "\n\tSelect one option:\n";

	std::cin >> option;

	return option;
}

void executeMenu(int option, Camera* &cam, Shader* &shader, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList, 
	Vector3D bgColor, Vector3D intersectionColor, std::string &fileName) {

	switch (option) {

	case 0:
		shader = new IntersectionShader(intersectionColor, bgColor);
		buildOurScene(cam, film, objectsList, lightSourceList,0,0);
		fileName = "Mesh_IntersectionShader";
		break;
	
	case 1:
		shader = new NormalShader();
		buildOurScene(cam, film, objectsList, lightSourceList, 0, 0);
		fileName = "Mesh_NormalShader";
		break;

	case 2:
		shader = new DirectShader(bgColor);
		buildOurScene(cam, film, objectsList, lightSourceList,1,0);
		fileName = "Mesh_Direct_Illumination_Shader";
		break;
	
	case 3:
		shader = new DirectShader(bgColor);
		buildOurScene(cam, film, objectsList, lightSourceList, 1, 1);
		fileName = "Mesh_Direct_Illumination_Shader_Spheres";
		break;

	case 4:
		int bounces, rays;
		std::cout << "Number of Rays: ";
		std::cin >> rays;
		std::cout << "Number of bounces: ";
		std::cin >> bounces;
		shader = new GlobalShader(bgColor, rays, bounces);
		buildOurScene(cam, film, objectsList, lightSourceList, 1,0);
		fileName = "Mesh_Global_Illumination_Shader";
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

	tStart = clock();
	//Menu APPLICATION Call
	executeMenu(menu(), cam, shader, film, objectsList, lightSourceList, bgColor, intersectionColor, fileName);
	// Launch some rays!
	raytrace(cam, shader, film, objectsList, lightSourceList);
	tEnd = clock();
	printf("\n\n\tTime taken: %.2fs\n", (double)(tEnd - tStart) / CLOCKS_PER_SEC);
	
	// Save the final result to file
	std::cout << "\n\nSaving the result to file " << fileName << "\n" << std::endl;
	film->save(fileName);

	std::cout << "\n\n" << std::endl;
	
    return 0;
}
