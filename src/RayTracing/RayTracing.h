/** @file RayTracing.h @brief different rays and groups of rays (FotonRay is a TODO)*/

#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <DetailedSpaces.h>

/** @brief HalfLine3D with methods to calculate the closest crossed triangle in space.
 * 
 * This type is not able to do recursion, but provides methods to calculate direction of reflecting and refracting rays.*/
class Ray : public HalfLine3D {
protected:
	/**
	 * @brief Construts a general Ray from given 2 vectors that are points of the Ray.
	 * 
	 * Two points can define a unique Ray. If points are the same then direction of Ray is a null vector.
	 * @param a is the starting point
	 * @param b is a point on Ray, defining direction
	 * @param startTri triangle that has to be ignored when finding closest triangle
	 */
	Ray(const Vect3D a, const Vect3D b, const DetailedTri3D* const startTri = 0);
	
	/** @brief optical reflection of ray when hiting given tri*/
	Vect3D reflV(const DetailedTri3D* const tri) const;
	
	/** @brief optical refraction of ray when hiting given tri*/
	Vect3D refrV(const DetailedTri3D* const tri) const;
	
	/** @brief changes stored values if given triangle is closer then closest and not starttri*/
	void shotAt(const DetailedTri3D & acttri) const;
	
	/** @brief closest crossed triangle*/
	const DetailedTri3D * getClosest() const;
	
	/** @brief position of closest cross*/
	Vect3D getClosestCross() const;

private:
	const DetailedTri3D* startTri;
	const DetailedTri3D mutable * closest;	//found closest
	mutable Vect3D closestCross;		//position of cross
	
};

/** @brief Ray that is shot from viewpoint through a pixel of the screen and 'recurses' itself in given depth
 * 
 * Calculates color of view in set direction.
 * Stores information about the closest triangle that it crosses.
 * Generates reflecting and refracting rays at cross point in definied recursion depth.*/
class ViewRay : public Ray {
public:
	/** @brief creates a ray that points from a to b
	 * 
	 * @param a starting point of ray
	 * @param b defines direction of ray
	 * @param startTri triangle that the ray will ignore
	 * @param depth depth of recursion*/
	ViewRay(const Vect3D a, const Vect3D b, const DetailedTri3D* const startTri = 0, const unsigned int depth = 8);
	
	/** @brief shots the ray and returns the result of the recursion*/
	Color shotAt(const DetailedSpace3D & space) const;
private:
	unsigned int depth;		//recursion depth
};

/** @brief Manages a group of ViewRays in order to generate images with depth of field.
 * 
 * All rays go through a focuspoint but their starting points are different.
 * Starting point of rays are on a surface that is defined by pos and direction of focuspoint.
 * Given hdir and vdir defines a 2D coordinate system, this surface contains starting points of the rays.
 * Rays are shot inside a circle on this surface that has r radius. Raydist defines the distance between startingpoint of rays.*/
class ViewRayGroup {
public:
	/** @brief Constructs a ViewRayGroup from given values.
	 * 
	 * @param pos starting point of raygroup
	 * @param focus point where all rays go through
	 * @param hdir horizontal normalvector (direction and unit of x axis on starting coordinate system)
	 * @param vdir vertical normal vector (direction and unit of y axis on starting coordinate system)
	 * @param r radius of circle from which rays are shot (units are length of hdir and vdir)
	 * @param raydist distance of starting point of rays (unites are length of hdir and vdir)*/
	ViewRayGroup(const Vect3D pos, const Vect3D focus, const Vect3D hdir, const Vect3D vdir, const float r, const float raydist);
	
	/** @brief Shots all rays and returns the average of their result.*/
	Color shotAt(const DetailedSpace3D & space) const;
private:
	Vect3D pos,focus;	//position and focuspoint of RayGroup
	Vect3D hdir,vdir;	//horizontal and vertical normal vectors
	float r;	//radius of circle on starting surface where startingpoint of rays are found
	float raydist;	//distance of startingpoint of rays
};

/** @brief Ray that is shot from a lighting point through a point of space containing a color parameter
 * 
 * Marks each triangle that it hits - this way it follows way of a foton.
 * Generates reflecting and refracting rays at cross point in definied recursion depth.*/
class FotonRay : public Ray {
public:
	/** @brief creates a FotonRay that points from a to b
	 * 
	 * @param a starting point of ray
	 * @param b defines direction of ray
	 * @param color color of ray
	 * @param startTri defines the triangle that the ray should ignore
	 * @param depth depth of recursion*/
	FotonRay(const Vect3D a, const Vect3D b, const Color color, const DetailedTri3D* const startTri = 0, const unsigned int depth = 8);
	
	/** Shots the ray and marks each triangles in space that it touches.
	 * 
	 * @note although this method changes state of triangles in space, these state changes are stored in mutable attributes -> the parameter can be const. Why are they mutable?, light information is considered (by myself :)) as not state of triangles. It is only an additional information. This definition makes rays more generic -> makes code easier to maintaine.*/
	void shotAt(const DetailedSpace3D & space) const;
private:
	Color color;
	unsigned int depth;		//recursion depth
};

#endif