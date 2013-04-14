/** @file Camera.h @brief cameras and tools used by cameras*/

#ifndef CAMERA_H
#define CAMERA_H

#include "DetailedSpaces.h"
#include "Space2D.h"
#include "RayTracing.h"

#include <vector>

/** @brief Manages three 2D rotations in 3D as using them as latitude, longitude and twist.
 * 
 * Provides 3 vectors that are ortogonal to each other: x,y,z - defining a 3 dimensional coordinate system. Longtitude rotation happens around vertical axis (y0). Latitude rotation happens around horizontal normal vector of longitude rotation (x0 rotated around y0 by longitude). Twist is around z.*/
class GeoRot3D {
public:
	/** @brief Constructs a GeoRot3D with no rotation.
	 * 
	 * Direction points up (0,0,1)
	 * Horizontal and Vertical normal Vectors are (1,0,0), (0,1,0)*/
	GeoRot3D();

	/** @brief Adds given rotation to longitude (rotation around global y0 axis)*/
	void rotLon(const Rot2D r);
	
	/** @brief Adds given rotation to latitude (rotation around r0 rotated around y0 by longitude)*/
	void rotLat(const Rot2D r);
	
	/** @brief Adds given rotation to twist (rotation around direction)*/
	void rotTwi(const Rot2D r);
	
	/** @brief Horizontal normal vector of rotation.*/
	Vect3D getX() const;
	
	/** @brief Vertical normal vector of rotation.*/
	Vect3D getY() const;
	
	/** @brief Direction of rotation.*/
	Vect3D getZ() const;
private:
	const Vect3D x0,y0,z0;	//starting values of horizontal and vertical normal vectors and direction. y0: axis of longitude
	Rot2D lon,lat,twi;	//longitude, latitude, twist
};

/** @brief defines position & direction and methods to rotate direction and to step position forward/backward, left/right, up/down in 3D
 * 
 * Base class of movable tools with direction used in 3D: cameras, lamp.*/
class Movable3DTool {
public:
	/** @brief Constructs a Movable3DTool with origo position and nilvector direction*/
	Movable3DTool();

	/** @brief Setter for position of tool*/
	void setPos(const Vect3D pos);
	
	/** @brief Setter for direction, horizontal and vertical normal vectors
	 *
	 * @param hdir Horizontal normal vector of direction.
	 * @param vdir Vertical normal vector of direction.
	 * @param dir Direction of tool.*/
	void setHVDir(const Vect3D hdir, const Vect3D vdir, const Vect3D dir);
	
	/** @brief Setter for direction, horizontal and vertical normal vectors
	 * 
	 * @param rot Rotation that holds direction and normal vectors of direction*/
	void setHVDir(const GeoRot3D rot);
	
	/** @brief Changes position of tool using given values.
	 * 
	 * @param thdir moves tool horizontally with length: hdir*thdir
	 * @param tvdir moves tool vertically with length: vdir*tvdir
	 * @param tdir moves tool forward with length: dir*tdir*/
	void stepPos(const float thdir, const float tvdir,const float tdir);

	/** @brief position of tool*/
	Vect3D getPos() const;
	
	/** @brief horizontal normal vector of direction*/
	Vect3D getHdir() const;
	
	/** @brief vertical normal vector of direction*/
	Vect3D getVdir() const;
	
	/** @brief direction of tool*/
	Vect3D getDir() const;
private:
	Vect3D pos;
	Vect3D hdir, vdir, dir;
};

/** @brief Methods and properties that different camera implementations may need.
 * 
 * Provides methods for managing position and direction of a camera. It also contains the next details: resolution, angle of view, objects to be shown.*/
class AbstractCam : public Movable3DTool {
public:
	/** @brief Constructs an abstract camera with no space definied, 1.0 angle of view and 0,0 resolution */
	AbstractCam();

	/** @brief Setter for space attribute*/
	void setSpace(const DetailedSpace3D * const space);
	
	/** @brief Setter for Angle of View (aov) attribute.
	 * 
	 * The higher the value is the wider the view is.*/
	void setAov(const float aov);
	
	/** @brief Setter for vertical and horizontal resolution.*/
	void setRes(const float x, const float y);

	/** @brief Pointer of the list that contains triangles that are visible for the camera.*/
	const DetailedSpace3D* getSpace() const;
	
	/** @brief Angle of view.*/
	float getAov() const;
	
	/** @brief Horizontal resolution of camera.*/
	int getXres() const;
	
	/** @brief Vertical resolution of camera.*/
	int getYres() const;
	
	/** @brief Average of vertical and horizontal resolution.*/
	float getAvgRes() const;
private:
	const DetailedSpace3D* space;
	float aov;
	int xres,yres;
};


/** @brief Camera that uses graphical projection.*/
class VectorCam : public AbstractCam {
public:
	/** @brief Constructs a VectCam with no space definied, 1.0 angle of view and 0,0 resolution.*/
	VectorCam();
	
	/** @brief Setter for position of VectorCam*/
	void setPos(const Vect3D pos);
	
	/** @brief Setter for direction, horizontal and vertical normal vectors
	 *
	 * @param hdir Horizontal normal vector of direction.
	 * @param vdir Vertical normal vector of direction.
	 * @param dir Direction of VectorCam.*/
	void setHVDir(const Vect3D hdir, const Vect3D vdir, const Vect3D dir);
	
	/** @brief Setter for direction, horizontal and vertical normal vectors
	 * 
	 * @param rot Rotation that holds direction and normal vectors of direction*/
	void setHVDir(const GeoRot3D rot);
	
	/** @brief Changes position of camera using given values.
	 * 
	 * @param thdir moves camera horizontally with length: hdir*thdir
	 * @param tvdir moves camera vertically with length: vdir*tvdir
	 * @param tdir moves camera forward with length: dir*tdir*/
	void stepPos(const float thdir, const float tvdir, const float tdir);

	/** @brief The projection itself*/
	DetailedSpace2D project() const;
private:
	Plane3D hplain,vplain,screen;	//horizontal plane, vertical plane, screen plane
	Vect2D project(const Vect3D & vect) const;	//projection of 1 point. Assuming that the point is before the screen plane!
	bool isVisible(const DetailedTri3D & tri) const;	//returns true if there is any pole of given triangle that is NOT before the splane
 	DetailedTri2D project(const DetailedTri3D & tri) const;	//returns a 2 dimensional triangle that is projection of given Tri3D. Assuming that all points are projectable.
	void calcPlanes();	//calculates splain,vplain,hplain
};

/** @brief Camera that renders using RayGroups.
 * 
 * Shots a raygroup for each pixel of the result.*/
class RayTracerCam : public AbstractCam {
public:
	/** @brief Constructs an empty camera that can be set with setters.*/
	RayTracerCam();
	
	/** @brief sets focus distance.
	 * 
	 * Focus distance is the distance where imiage is sharp. Before and after this point image is blured.*/
	void setFocusDist(float fdist);
	
	/** @brief sets depth of field.
	 * 
	 * Defines the 'angle of view' of each RayGroup. The higher the value is the more blured the imiages become that are not in focuspoint.*/
	void setDof(float dof);
	
	/** @brief density of rays per pixel.*/
	void setDensity(float density);
	
	/** @brief Calculates color of x,y pixel.
	 * 
	 * 0,0 is direction of camera.*/
	Color calcColor(const int x, const int y) const;
private:
	float fdist;
	float dof;	//depth of field
	float density;	//density of rays per each pixels
};

/** @brief Spread group of fotonrays in order to estimate lighing.
 * 
 * Horizontal and vertical angles between nearby fotonrays is always the same
 * (not like rays of raytracer camera where density decreases converging to middle of screen).*/
class Lamp : public Movable3DTool {
public:
	/** @brief defines a Lamp with no angles*/
	Lamp();

	/** @brief setter for resolution
	 * 
	 * @param hangle horizontal angle of light
	 * @param vangle vertical angle of light
	 * @param rangle angle between neighbour rays
	 * 
	 * @warning calculation of direction of rays happens using geological coordinates - giving high vertical angle produces high density at poles.*/
	void setRes(const float hangle, const float vangle, const float rangle);
	
	/** @brief shots rays
	 * 
	 * @note although this method changes state of triangles in space, these state changes are stored in mutable attributes -> the parameter can be const. Why are they mutable?, light information is considered (by myself :)) as not state of triangles. It is only an additional information. This definition makes rays more generic -> makes code easier to maintaine.*/
	void shotAt(const DetailedSpace3D * const space) const;
private:
	std::vector<std::vector<Vect3D> > screen;	//stores direction of each ray to be shot - this makes parallel rendering possible
};

#endif