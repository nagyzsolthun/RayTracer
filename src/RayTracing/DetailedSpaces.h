/**
 * @file DetailedSpaces.h
 * @brief detailed 3D and 2D tools
 * 
 * These tools are defined in order to use ray tracing efficently. Optical details and tools that makes rendering faster added.
 */

#ifndef DETAILEDSPACES_H
#define DETAILEDSPACES_H

#include "Space3D.h"
#include <list>

/**
 * @brief 3D triangle that has efficent functions for designating intersection of lines.
 * 
 * Stores plane that is defined by verticies of triangle => it doesn't need to be calculated for each cross-checking.<br/>
 * Stores planes that contain sides of triangle => its faster to decide if a line (, halfline, section) crosses triangle.
 */
class CrossableTri3D {
public:
	/**
	 * @brief Construts a triangle with null vector vertices.
	 */
	CrossableTri3D();
	
	/**
	 * @brief Construts a triangle with given verticies.
	 */
	CrossableTri3D(const Vect3D a, const Vect3D b, const Vect3D c);

	/**
	 * @brief Sets up a triangle with given verticies.
	 */
	void set(const Vect3D a, const Vect3D b, const Vect3D c);
	
	/**
	 * @brief Surface that is defined by 3 verticies of triangle.
	 * 
	 * Surface designates 'above' and 'under' parts of triangle. Positive side is where verticies are visible ACW.
	 */
	Plane3D surface() const;

	/**
	 * @brief Returns if line is crossing triangle.
	 */
	bool isCrossed(const Line3D line) const;
	
	/**
	 * @brief Returns if half-line is crossing triangle.
	 */
	bool isCrossed(const HalfLine3D hline) const;
	
	/**
	 * @brief Returns if section is crossing triangle.
	 */
	bool isCrossed(const Sect3D sect) const;

	/**
	 * @brief First vertex of triangle.
	 */
	Vect3D getA() const;
	
	/**
	 * @brief Second vertex of triangle.
	 */
	Vect3D getB() const;
	
	/**
	 * @brief Third vertex of triangle.
	 */
	Vect3D getC() const;
private:
	Vect3D a,b,c;	//vertices
	Plane3D s;		//surface that is defined by triangle
	Plane3D bc,ca,ab;	//planes: sides of triangle are on these planes, and these planes are not parallel with s
};

/**
 * @brief type for managing colors from black to infinite
 * 
 * Stores red, green and blue components of a color which are in scale [0,infinity[ where (1,1,1) is white => Colors can be brighter then white.
 */
class Color {
public:
	/**
	 * @brief Constructs black color.
	 */
	Color();
	
	/**
	 * @brief Constructs color from given values.
	 * 
	 * @param r Red component of color.
	 * @param g Green component of color.
	 * @param b Ble component of color.
	 */
	Color(const float r, const float g, const float b);
	
	/**
	 * @brief Sets color with given values.
	 * 
	 * @param r Red component of color.
	 * @param g Green component of color.
	 * @param b Ble component of color.
	 */
	void set(const float r, const float g, const float b);
	
	/**
	 * @brief Returns true if this equals o, false otherwise.
	 */
	bool operator==(const Color o) const;
	
	/**
	 * @brief Returns oposite of operator==
	 */
	bool operator!=(const Color o) const;
	
	/**
	 * @brief Returns sum of this and o.
	 */
	Color operator+(const Color o) const;
	
	/**
	 * @brief Multiplies each value of colors.
	 */
	Color operator*(const Color o) const;
	
	/**
	 * @brief Darken color by v.
	 * 
	 * @warning Given 0 as parameter results (nan,nan,nan) that is not a color.
	 */
	Color operator/(const float v) const;
	
	/**
	 * @brief Adds o to this.
	 */
	void operator+=(const Color o);
	
	/**
	 * @brief red component.
	 */
	float getR() const;
	
	/**
	 * @brief green component.
	 */
	float getG() const;
	
	/**
	 * @brief blue component.
	 */
	float getB() const;
private:
	float r,g,b;
};

/**
 * @brief 3D triangle that has a color, reflection, transparency and transulency
 */
class DetailedTri3D : public CrossableTri3D {
public:
	
	/**
	 * @brief Construts a triangle with given verticies.
	 */
	DetailedTri3D(const Vect3D a, const Vect3D b, const Vect3D c);
	
	/**
	 * @brief Setter for active color.
	 */
	void setActive(const Color active);
	
	/**
	 * @brief Setter for reflection.
	 */
	void setRefl(const Color refl);
	
	/**
	 * @brief Setter for transparency.
	 */
	void setTransp(const Color transp);
	
	/**
	 * @brief Setter for refraction. TODO document it!
	 */
	void setRefr(const float refr);
	
	/**
	 * @brief active color.
	 */
	Color getActive() const;
	
	/**
	 * @brief reflection.
	 */
	Color getRefl() const;
	
	/**
	 * @brief transparency.
	 */
	Color getTransp() const;
	
	/**
	 * @brief refraction.	TODO document it
	 */
	float getRefr() const;
private:
	Color active;
	Color refl;
	Color transp;
	float refr;
};

/**
 * @brief List that has DetailedTri3D elements.
 */
typedef std::list<DetailedTri3D> DetailedSpace3D;

/**
 * @brief 2D triangle with color
 */
class DetailedTri2D {
public:
	/**
	 * @brief Construts a triangle with null vector vertices.
	 */
	DetailedTri2D();
	
	/**
	 * @brief Construts a triangle with given vertices.
	 */
	DetailedTri2D(const Vect2D a, const Vect2D b, const Vect2D c);
	
	/**
	 * @brief Sets up a triangle with given verticies.
	 */
	void set(const Vect2D a, const Vect2D b, const Vect2D c);
	
	/**
	 * @brief Sets color of triangle.
	 */
	void setColor(const Color color);
	
	/**
	 * @brief First vertex of triangle.
	 */
	Vect2D getA() const;
	
	/**
	 * @brief Second vertex of triangle.
	 */
	Vect2D getB() const;
	
	/**
	 * @brief Third vertex of triangle.
	 */
	Vect2D getC() const;
	
	/**
	 * @brief Color of triangle.
	 */
	Color getColor() const;
private:
	Color color;
	Vect2D a,b,c;
};

/**
 * @brief List that has DetailedTri2D elements.
 */
typedef std::list<DetailedTri2D> DetailedSpace2D;

#endif