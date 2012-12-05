/** @file RayTracing.h @brief different rays and groups of rays (FotonRay is a TODO)*/

#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <DetailedSpaces.h>

/**
 * @brief Halfline that is shot from viewpoint through a pixel of the screen.
 * 
 * Stores information about the closest triangle that it crosses.
 * Generates reflecting and refracting rays at cross point in definied recursion depth.
 */
class ViewRay : public HalfLine3D {
public:
	/**
	 * @brief creates a ray that points from a to b
	 * 
	 * @param a starting point of ray
	 * @param b defines direction of ray
	 * @param startTri defines the triangle that the ray should ignore
	 * @param depth depth of recursion
	 */
	ViewRay(const Vect3D a, const Vect3D b, const DetailedTri3D* startTri = 0, const unsigned int depth = 8);
	
	/**
	 * Shots the ray and returns the result of the recursion.
	 */
	Color shotAt(const DetailedSpace3D & space) const;
private:
	void shotAt(const DetailedTri3D & acttri) const;
	Vect3D reflV() const;
	Vect3D refrV() const;
	
	unsigned int depth;		//recursion depth
	const DetailedTri3D* startTri;
	const DetailedTri3D mutable * closest;	//found closest
	mutable Vect3D closestCross;		//position of cross
};

/**
 * @brief Manages a group of ViewRays in order to generate images with depth of field.
 * 
 * All rays go through a focuspoint but their starting points are different.
 * Starting point of rays are on a surface that is defined by pos and direction of focuspoint.
 * Given hdir and vdir defines a 2D coordinate system, this surface contains starting points of the rays.
 * Rays are shot inside a circle on this surface that has r radius. Raydist defines the distance between startingpoint of rays.
 */
class ViewRayGroup {
public:
	/**
	 * Constructs a ViewRayGroup from given values.
	 * 
	 * @param pos starting point of raygroup
	 * @param focus point where all rays go through
	 * @param hdir horizontal normalvector (x axis on starting coordiante system)
	 * @param vdir vertical normal vector (y axis on starting coordiante system)
	 * @param r radius of circle from which rays are shot
	 * @param raydist distance of starting point of rays
	 */
	ViewRayGroup(const Vect3D pos, const Vect3D focus, const Vect3D hdir, const Vect3D vdir, const float r, const float raydist);
	
	/**
	 * Shots all rays and returns the average of their result.
	 */
	Color shotAt(const DetailedSpace3D & space) const;
private:
	Vect3D pos,focus;	//position and focuspoint of RayGroup
	Vect3D hdir,vdir;	//horizontal and vertical normal vectors
	float r;	//radius of circle on starting surface where startingpoint of rays are found
	float raydist;	//distance of startingpoint of rays
};

#endif