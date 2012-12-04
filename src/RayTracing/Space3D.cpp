#include <Space3D.h>

//--------------------------------------Vect3D----------------------------------------------------------------
Vect3D::Vect3D(){set(0,0,0);}
Vect3D::Vect3D(float x, float y, float z) {set(x,y,z);}
Vect3D::Vect3D(const Vect3D a, const Vect3D b) {setCross(a,b);}	//cross product
void Vect3D::set(const float x, const float y, const float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
void Vect3D::setCross(const Vect3D a, const Vect3D b) {
//cross product
	x = a.y*b.z - a.z*b.y;
	y = a.z*b.x - a.x*b.z;
	z = a.x*b.y - a.y*b.x;
}
float Vect3D::len2() const 							{return x*x + y*y + z*z;}
float Vect3D::dist2(const Vect3D o) const 			{return (o-*this).len2();}
Vect3D Vect3D::operator+ (const Vect3D o) const 	{return Vect3D(x+o.x, y+o.y, z+o.z);}
Vect3D Vect3D::operator- (const Vect3D o) const 	{return Vect3D(x-o.x, y-o.y, z-o.z);}
Vect3D Vect3D::operator* (const float t) const		{return Vect3D(x*t, y*t, z*t);}
Vect3D Vect3D::operator/ (const float t) const		{return Vect3D(x/t, y/t, z/t);}
float Vect3D::operator* (const Vect3D o) const		{return x*o.x + y*o.y + z*o.z;}
void Vect3D::operator+=(const Vect3D o) 			{x+=o.x; y+= o.y; z+=o.z;}
Vect3D Vect3D::operator-() const					{return Vect3D(-x,-y,-z);}
float Vect3D::getX() const							{return x;}
float Vect3D::getY() const							{return y;}
float Vect3D::getZ() const							{return z;}
//--------------------------------------Plane3D----------------------------------------------------------------
Plane3D::Plane3D(){}
Plane3D::Plane3D(const Vect3D p, const Vect3D n) 					{set(p,n);}
Plane3D::Plane3D(const Vect3D a, const Vect3D b, const Vect3D c) 	{set(a,b,c);}
void Plane3D::set(const Vect3D p, const Vect3D n)					{this->p = p;this->n = n;}
void Plane3D::set(const Vect3D a, const Vect3D b, const Vect3D c) {
	//p = a
	//n = Vect3D(b-a,c-a) - cross product of vectors pointing to b and c from a
	//if a,b,c is in ACW then this vector will point "up"
	set(a, Vect3D(b-a,c-a));
}
float Plane3D::distsign(const Vect3D q) const {
	//distance between q and plane where unit is length of n
	//sign of value is positive if q is above plane, negative if under.
	//e: q + n*t
	//s: this plane
	//result: -t in e
	
	const float t = (n*(p-q)) / (n*n);
	return -t;
}
Vect3D Plane3D::getP() const {return p;}
Vect3D Plane3D::getN() const {return n;}
//--------------------------------------Line3D----------------------------------------------------------------
Line3D::Line3D(){}
Line3D::Line3D(const Vect3D a, const Vect3D b) {set(a,b);}
void Line3D::set(const Vect3D a, const Vect3D b) {p = a; v = b-a;}
bool Line3D::isParallel(const Plane3D s) const {
	//true if line is parallel with plain, false otherwise
	if(v*s.getN() == 0) return true;	//if dot product is 0 then two vectors have right angel.  this is the exact situation when plane is parallel with line
	return false;
}
float Line3D::distsign(const Plane3D s) const {
	//returns t parameter in p+v*t that is needed to reach plain.
	const Vect3D sp = s.getP();
	const Vect3D sn = s.getN();
	float t = ((sp-p) * sn) / (v*sn);
	return t;
	
}
Vect3D Line3D::cross(const Plane3D s) const {return p + v*distsign(s);}
Vect3D Line3D::getP() const {return p;}
Vect3D Line3D::getV() const {return v;}
//--------------------------------------HalfLine3D----------------------------------------------------------------
HalfLine3D::HalfLine3D() : Line3D() {}
HalfLine3D::HalfLine3D(const Vect3D a, const Vect3D b){set(a,b);}
bool HalfLine3D::isCrossing(const Plane3D plain) const {
	if(Line3D::isParallel(plain)) return false;

	if(distsign(plain) < 0) return false;	//if crosspoint is behind startingpoint..
	return true;
}
//--------------------------------------Sect3D----------------------------------------------------------------
Sect3D::Sect3D() : Line3D() {}
Sect3D::Sect3D(const Vect3D a, const Vect3D b){set(a,b);}
bool Sect3D::isCrossing(const Plane3D s) const {
	if(Line3D::isParallel(s)) return false;

	const float t = distsign(s);	//the parameter if intersection
	if(t < 0 || t > 1) return false;	//if its behind starting point or after ending point then its false
	return true;
}
//--------------------------------------Axis3D----------------------------------------------------------------
Axis3D::Axis3D() : Line3D() {}
Axis3D::Axis3D(const Vect3D v) : Line3D(Vect3D(0,0,0),v) {}
Axis3D::Axis3D(const Vect3D a, const Vect3D b) : Line3D(a,b) {}
void Axis3D::set(const Vect3D a, const Vect3D b) {Line3D::set(a,b);}
Vect3D Axis3D::rot(const Vect3D q, const Rot2D rot) const {
	//we need to find out the 2D coordinate system of rotation where q is (1,0) and (0,1) is q rotated 90 degrees
	//x axis of this coordinate system is simple: normal vector of Axis that points to q
	const Vect3D v = Line3D::getV();	//just to make it readable..
	const Vect3D o = cross(Plane3D(q,v));
	const Vect3D rx = q - o;	//x axis of rotation coordinate system
	
	//y axis is a bit more complex: we know its right angel to both v and rx, so we can simply use cross product
	//bbbut: y axis' length should be the same as rx, and we know cross product doesnt always generate same length:
	//len(ry) == len(rx)*len(v)*sin(v,rx) => sin(v,rx) is known to be 1, so
	//len(ry) == len(rx)*len(v) => if v is 1, we are good, if not, coordinate system will be deformed
	const Vect3D ry(v,rx);	//order is important: it defines the direction of rotation. I want it to be anticlockwise looking FROM the direction of line. Why?, because then rx,ry,v looks like a coordinate system
	return o + rx*rot.getCos() + ry*rot.getSin();
}