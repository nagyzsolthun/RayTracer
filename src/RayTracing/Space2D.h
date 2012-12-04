/** @file Space2D.h @brief basic 2D tools*/

#ifndef SPACE2D_H
#define SPACE2D_H

/**
 * @brief basic operators and functions for using 2D vectors
 */
class Vect2D {
public:
	/**
	 * @brief Construts a null vector (both coordinates are 0).
	 * 
	 * Null vector has right angle to any vector.
	 */
	Vect2D();

	/**
	 * @brief Construts a vector with x and y coordinates.
	 * 
	 * @param x horizontal coordinate.
	 * @param y vertical coordinate.
	 */
	Vect2D(const float x, const float y);
	
	/** @brief Set coordinates to x and y.
	 * 
	 * @param x horizontal coordinate.
	 * @param y vertical coordinate.
	 */
	void set(const float x, const float y);

	/**
	 * @brief Vector component of p in direction of this.
	 * 
	 * See wiki for more details: <a href="http://en.wikipedia.org/wiki/Vector_projection">Vector Projection</a>.
	 * @warning: If this is null, function results nan.
	 * @return Vector component of p that is parallel with this.
	 */
	float comp(const Vect2D p) const;

	/** @brief Square of length of vector.TODO: isneeded?*/
	float len2() const;
	
	/** @brief Square of distance between p and this.TODO: isneeded?*/
	float dist2(const Vect2D p) const;

	/**
	 * @brief This 90 degree rotated to left.
	 * 
	 * Using 90 degree rotation is a frequently used method in solving equities of 2 dimensional elements.
	 * @return This 90 degree rotated to left, that equals to: (-y,x).
	 */
	Vect2D rot90() const;
	
	/** @brief Addition of this and o.*/
	Vect2D operator+(const Vect2D o) const;
	
	/** @brief Distinction of this and o.*/
	Vect2D operator-(const Vect2D o) const;
	
	/** @brief Product of this and t where t is scalar.*/
	Vect2D operator*(const float t) const;
	
	/** @brief Product of this and 1/t where t is scalar.*/
	Vect2D operator/(const float t) const;
	
	/**
	 * @brief Dot product of this and o.
	 *
	 * Dot product is a frequently used method in solving equities in 2 and 3 dimensional space.
	 * See wiki for more details: <a href="http://en.wikipedia.org/wiki/Dot_product">Dot Product</a>.
	 */
	float operator*(const Vect2D o) const;
	
	/** @brief Adds o to this.*/
	void operator+=(const Vect2D o);
	
	/** @brief Horizontal coordinate of vector.*/
	float getX() const;
	
	/** @brief Vertical coordinate of vector.*/
	float getY() const;
private:
	float x,y;
};

/**
 * @brief Basic functions for using 2D lines.
 * 
 * Line is stored by 2 vectors: a vector that is on the line (p) and a vector that designates direction of the line (v).
 * @note Mathematical definition of the item is: p + v*t where t is a scalar parameter, its value is from range of ]-infinity, infinity[.
 * @note Equity with direction: v.y*x - v.x*y = v.y*p.x - v.x * v.y.
 * @note Equity using normal vector: n.x*x + n.y*y = n.x*p.x + n.y*p.y where n is v rotated 90 degrees (v.rot90()) - that is called normal vector.
 */
class Line2D {
public:
	/**
	 * @brief Construts a line with null starting point and null direction.
	 */
	Line2D();
	
	/**
	 * @brief Construts a line given 2 vectors that are points of the line.
	 * 
	 * Two points can define a unique line. If points are the same then direction of line is a null vector.
	 * @param a is a point on line.
	 * @param b is a point on line.
	 */
	Line2D(const Vect2D a, const Vect2D b);

	/**
	 * @brief Sets line from given 2 vectors that are points of the line.
	 * 
	 * Two points can define a unique line. If points are the same then direction of line is a null vector.
	 * @param a is a point on line.
	 * @param b is a point on line.
	 */
	void set(const Vect2D a, const Vect2D b);
	
	/**
	 * @brief Distance between q and line where unit is length of v; sign of value is positive if q is left to the line, negative if right
	 * 
	 * @note Positive and negative directions were decided by the way triangles are usually written up (ACW) - points inside triangle should be in positive side.
	 * @warning If direction of line is null, function returns nan.
	 */
 	float distsign(const Vect2D q) const;
	
	/**
	 * @brief Square of distance between q and line.
	 * 
	 * @warning If direction of line is null, function results nan.
	 */
	float dist2(const Vect2D q) const;

	/**
	 * @brief Point that is used as starting point in definition of line.
	 * 
	 * p in definition of line: p + v*t
	 */
 	Vect2D getP() const;
	
	/**
	 * @brief Direction of line.
	 * 
	 * v in definition of line: p + v*t
	 */
	Vect2D getV() const;
private:
	Vect2D p,v;
};

/**
 * @brief Rotation in 2D.
 * 
 * Rotation itself is an angle with 2 defined operators. The aim of the class is to minimize useage of expensive functions (cos(), sin()). Addition and substraction of 2 angles can be calculated using this class without asking for cos() or sin() functions.
 */
class Rot2D {
public:
	/**
	 * @brief Construts a rotation with given direction.
	 * 
	 * @param f is direction of rotation in [0, M_PI[ range. 0 equals 0 degree and M_PI equals 360 degrees.
	 */
	Rot2D(const float f = 0);

	/**
	 * @brief Sets rotation with given direction.
	 * 
	 * @param f is direction of rotation in [0, M_PI[ range. 0 equals 0 degree and M_PI equals 360 degrees.
	 */
	void set(const float f);
	
	/**
	 * @brief Adds o to this without using expensive functions.
	 */
	void operator+=(const Rot2D o);
	
	/**
	 * @brief Deduct o from this without using expensive functions.
	 */
	void operator-=(const Rot2D o);
	
	/**
	 * @brief Multiplies rotation with given integer.
	 */
	Rot2D operator*(int v) const;

	/**
	 * @brief Direction of rotation as vector.
	 * 
	 * @returns Direction of rotation as a Vect2D: coordinates of returned vector are cos and sin values of rotation.
	 */
	Vect2D dir() const;
	
	/**
	 * @brief Returns cosinus of rotation.
	 */
	float getCos() const;
	
	/**
	 * @brief Returns sinus of rotation.
	 */
	float getSin() const;
	
	/**
	 * @brief Returns numeric value of rotation: the angle in [0,M_PI[ range. 0 equals 0 degree and M_PI equals 360 degrees.
	 */
	float getF() const;
private:
	float cos,sin;	//we only deal with cos and sin directly.
	float f;	//[0,M_PI[ range. Stored for external useage (e.g. drawing a rotating object) and for recalculating sin and cos values
	//TODO: recalculator
	//TODO: keep f in range 0, M_PI
};

#endif
