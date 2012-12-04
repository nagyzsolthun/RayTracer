#include <QApplication>

#include <iostream>
#include <cmath>

#include "RayTracing/Space2D.h"
#include "RayTracing/Space3D.h"
#include "RayTracing/Camera.h"
#include "SceneSetterWidget.h"

int main(int argc, char *argv[])
{
	/*GeoRot3D rot;
	rot.rotTwi(Rot2D(M_PI/4));
	
	Vect3D d = rot.getX();
	std::cout << d.getX() << ' ' << d.getY() << ' ' << d.getZ() << std::endl;*/
	
	DetailedTri3D bob( Vect3D(-7,-5,0), Vect3D(5,-5,0), Vect3D(0,5,0) );
	DetailedTri3D joe( Vect3D(-7,-5,-5), Vect3D(5,-5,-5), Vect3D(0,5,-5) );
	DetailedTri3D sue( Vect3D(-7,-5,-10), Vect3D(5,-5,-10), Vect3D(0,5,-10) );
	
	bob.setActive(Color(0.5,0.5,0.6));
	joe.setActive(Color(0,1,0));
	sue.setActive(Color(0,0,1));
	
	bob.setTransp(Color(0.6,0.6,0.6));
	joe.setTransp(Color(0.6,0.6,0.6));
	sue.setRefl(Color(0.6,0.6,0.6));
	
	DetailedSpace3D space;
	space.push_back(bob);
	space.push_back(joe);
	space.push_back(sue);

	QApplication app(argc, argv);
	SceneSetterWidget* window = new SceneSetterWidget(&space);
	//RayTracingWidget* window = new RayTracingWidget(cam);
	window->show();
	return app.exec();
}