/** @file Space3D.h @brief basic 3D tools*/

#ifndef SPACE3DELEMENTS_H
#define SPACE3DELEMENTS_H

#include "Space2D.h"

/** @brief basic operators and functions for using 3D vectors.*/
class Vect3D {
public:
	/**
	 * @brief Construts a null vector (all coordinates are 0).
	 * 
	 * Null vector has right angle to any vector.
	 */
	Vect3D();
	
	/** @brief Construts a vector with x,y,z coordinates.*/
	Vect3D(const float x, const float y, const float z);
	/**
	 * @brief Construts a vector as cross product of a and b vectors.
	 * 
	 * Constructed vector has right angel to both a and b.
	 * If we look at direction of result, we need to rotate a right to reach b:
	 * <a href="http://upload.wikimedia.org/wikipedia/commons/thumb/d/df/Crossproduct.png/200px-Crossproduct.png">image</a>.
	 * @note Length of vector is product of length of parameters multiplied with sin of their angle: len(this) == len(a) * len(b) * sin(a,b).
	 */
	Vect3D(const Vect3D a, const Vect3D b);
	
	/** @brief Set coordinates to x,y,z.*/
	void set(const float x, const float y, const float z);
	
	/**
	 * @brief Sets vector as cross product of a and b vectors.
	 * 
	 * After setting vector has right angel to both a and b.
	 * If we look at direction of result, we need to rotate a right to reach b:
	 * <a href="http://upload.wikimedia.org/wikipedia/commons/thumb/d/df/Crossproduct.png/200px-Crossproduct.png">image</a>.
	 * @note Length of vector is product of length of parameters multiplied with sin of their angle: len(this) == len(a) * len(b) * sin(a,b).
	 */
	void setCross(const Vect3D a, const Vect3D b);
	
	/** @brief Square of length of vector.*/
	float len2() const;	//TODO: isneeded?
	
	/** @brief Square of distance between p and this.*/
	float dist2(const Vect3D p) const;	//TODO: isneeded?
	
	/** @brief Addition of this and o.*/
	Vect3D operator+(const Vect3D o) const;
	
	/** @brief Distinction of this and o.*/
	Vect3D operator-(const Vect3D o) const;
	
	/** @brief Product of this and t where t is scalar.*/
	Vect3D operator*(const float t) const;
	
	/** @brief Product of this and 1/t where t is scalar.*/
	Vect3D operator/(const float t) const;
	
	/**
	 * @brief Dot product of this and o.
	 *
	 * Dot product is a frequently used method in solving equities in 2 and 3 dimensional space.
	 * See wiki for more details: <a href="http://en.wikipedia.org/wiki/Dot_product">Dot Product</a>.
	 */
	float operator*(const Vect3D o) const;
	
	/** @brief Adds o to this.*/
	void operator+=(const Vect3D o);
	
	/** @brief -1 * vector*/
	Vect3D operator-() const;
	
	/**
	 * @brief X coordinate of vector.
	 */
	float getX() const;
	
	/** @brief Y coordinate of vector.*/
	float getY() const;
	
	/** @brief Z coordinate of vector.*/
	float getZ() const;
private:
	float x,y,z;
};

/**
 * @brief basic operators and functions for using planes in 3 dimensions.
 * 
 * Plane is stored using 2 values: a vector that is a point of the plane: p; and a normal vector: n (a vector that has right angel to plane).
 * @note Mathematical definition: n*(x,y,z) = n*p.
 */
class Plane3D {
public:
	/** @brief Construts a plane with null starting point and null normal vector.*/
	Plane3D();
	
	/** @brief Construts a plane with p starting point and n normal vector.*/
	Plane3D(const Vect3D p, const Vect3D n);
	
	/**
	 * @brief Construts a plane defined by 3 points.
	 * 
	 * 3 points can define a unique plane. If points are drown ACW (usuall way of drawing traingle) then n of result will point up.
	 * @warning If any of (a,b,c) points are identical then n of result is null.
	 */
	Plane3D(const Vect3D a, const Vect3D b, const Vect3D c);
	
	/** @brief Sets p as starting point and n as normal vector of plane.*/
	void set(const Vect3D p, const Vect3D n);
	
	/** @brief Sets p as a point on (a,b,c) plane and n as normal vector of (a,b,c) plane.*/
	void set(const Vect3D a, const Vect3D b, const Vect3D c);
	
	/**
	 * @brief Distance between q and plane where unit is length of n; sign of value is positive if q is above plane, negative if under.
	 * 
	 * @warning If n is null then function returns nan.
	 */
	float distsign(const Vect3D q) const;
	
	/** @brief Point that is on plane.*/
	Vect3D getP() const;
	
	/** @brief Normal vector of plane.*/
	Vect3D getN() const;
private:
	Vect3D p,n;
};

/**
 * @brief Basic functions for using 3D lines.
 * 
 * Line is stored by 2 vectors: a vector that is on the line (p) and a vector that designates direction of the line (v).
 * @note Mathematical definition: p + v*t where t is a scalar parameter, its value is from range of ]-infinity, infinity[.
 */
class Line3D {
public:
	/** @brief Construts a line with null starting point and null direction.*/
	Line3D();
	
	/**
	 * @brief Construts a line given 2 vectors that are points of the line.
	 * 
	 * Two points can define a unique line. If points are the same then direction of line is a null vector.
	 * @param a is a point on line.
	 * @param b is a point on line.
	 */
	Line3D(const Vect3D a, const Vect3D b);

	/**
	 * @brief Sets line from given two vectors that are points of the line.
	 * 
	 * Two points can define a unique line. If points are the same then direction of line is a null vector.
	 * @param a is a point on line.
	 * @param b is a point on line.
	 */
	void set(const Vect3D a, const Vect3D b);
	
	/**
	 * @brief Value designating whether line is parallel with plane.
	 * 
	 * @return true if line is parallel with plane, false otherwise
	 * @warning If v is null then result is true.
	 * @warning If normal vector of plane is null then result is true.
	 */
	bool isParallel(const Plane3D s) const;
	
	/**
	 * @brief Distance between p and intersection of line and plane where unit is length of v; sign of value is positive if p is under plane, negative if above.
	 * 
	 * Function returns t parameter in p+v*t that is needed to reach plane. TODO: check if needed, check if needed here
	 * @warning If line is parallel with plane then function returns nan.
	 * @warning If v of line is null then function returns nan.
	 * @warning If n of plane is null then function returns nan.
	 */
	float distsign(const Plane3D s) const;
	
	
	/**
	 * @brief Intersection of line and plane.
	 * 
	 * @warning If line is parallel with plane then function returns (nan,nan,nan).
	 * @warning If v of line is null then function returns (nan,nan,nan).
	 * @warning If n of plane is null then function returns (nan,nan,nan).
	 */
	Vect3D cross(const Plane3D s) const;
	
	/**
	 * @brief Point that is used as starting point in definition of line.
	 * 
	 * p in definition of line: p + v*t
	 * @warning returns nan if line is parallel with plane
	 */
 	Vect3D getP() const;
	
	/**
	 * @brief Direction of line.
	 * 
	 * v in definition of line: p + v*t
	 */
	Vect3D getV() const;
private:
	Vect3D p,v;
};

/**
 * @brief 3D line with a starting point.
 * 
 * HalfLine3D is a line with a definied starting point. It is stored by 2 vectors: starting point of half-line (p) and a vector that designates direction of the half-line (v).
 * @note Mathematical definition: p + v*t where t is a scalar parameter, its value is from range of [0, infinity[.
 */
class HalfLine3D : public Line3D {
public:
	/** @brief Construts a half-line with null starting point and null direction.*/
	HalfLine3D();
	
	/**
	 * @brief Construts a half-line given 2 vectors that are points of the half-line.
	 * 
	 * Two points can define a unique half-line. If points are the same then direction of line is a null vector.
	 * @param a is the starting point of half-line.
	 * @param b is a point on half-line, defining direction.
	 */
	HalfLine3D(const Vect3D a, const Vect3D b);
	
	/**
	 * @brief Value designating whether half-line crosses plane.
	 * 
	 * @return true if half-line crosses plane, false otherwise.
	 * @warning If v is null then result is false.
	 * @warning If normal vector of plane is null then result is false.
	 */
	bool isCrossing(const Plane3D s) const;
};

/**
 * @brief basic functions for using 3D sections
 * 
 * Sect3D is a line with a definied starting and ending points. It is stored by 2 vectors: starting point (p) and a vector that designates direction and ending point of the half-line (v).
 * @note Mathematical definition: p + v*t where t is a scalar parameter, its value is from range of [0,1].
 */
class Sect3D : public Line3D {
public:
	/** @brief Construts a section with null starting point and null direction.*/
	Sect3D();
	/**
	 * @brief Construts a section with a starting point and b ending point.
	 * 
	 * Direction of section is b-a.
	 */
	Sect3D(const Vect3D a, const Vect3D b);
	
	/**
	 * @brief Value designating whether section crosses plane.
	 * 
	 * @return true if section crosses plane, false otherwise.
	 * @warning If v is null then result is false.
	 * @warning If normal vector of plane is null then result is false.
	 */
	bool isCrossing(const Plane3D s) const;
};

/**
 * @brief functionality for rotating vectors in 3D.
 * 
 * Rot3D is a line that behaves as an axis in 3D.
 */
class Axis3D : public Line3D {
public:
	/** @brief Construts an axis with null starting point and null direction.*/
	Axis3D();
	
	/**
	 * @brief Construts an axis from given vector that is point of axis. Starting point is origin.
	 * 
	 * Two points can define a unique line. We use origin as first point.
	 * @param v direction of axis.
	 * @warning if length of v is not 1, then rotation will happen around an ellipse: the longer v is, the "higher" the ellipse will be.
	 */
	Axis3D(const Vect3D v);
	
	/**
	 * @brief Construts an axis given 2 vectors that are points of the axis.
	 * 
	 * Two points can define a unique line. If points are the same then direction of axis is a null vector.
	 * @param a is a point on axis.
	 * @param b is a point on axis.
	 * @warning if distance of a and b is not 1, then rotation will happen around an ellipse: the longer a-b is, the "higher" the ellipse will be.
	 */
	Axis3D(const Vect3D a, const Vect3D b);

	/**
	 * @brief Sets axis from given two vectors that are points of the axis.
	 * 
	 * Two points can define a unique axis. If points are the same then direction of axis is a null vector.
	 * @param a is a point on axis.
	 * @param b is a point on axis.
	 * @warning if distance of a and b is not 1, then rotation will happen around an ellipse: the longer a-b is, the "higher" the ellipse will be.
	 */
	void set(const Vect3D a, const Vect3D b);
	
	/**
	 * @brief Rotates q around axis with given rot angle.
	 * 
	 * Rotation is anticlockwise FROM the direction of the axis.
	 * @warning if length of v is not 1, then rotation happens around an ellipse: the longer v is the "higher" the ellipse is.
	 * @note the direction was decided this way to use x,y,v as a coordinate system
	 */
	Vect3D rot(const Vect3D q, const Rot2D rot) const;
};

#endif