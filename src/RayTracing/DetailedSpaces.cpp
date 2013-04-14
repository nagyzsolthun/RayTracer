#include "DetailedSpaces.h"
//--------------------------------------CrossableTri3D----------------------------------------------------------------
CrossableTri3D::CrossableTri3D() {}
CrossableTri3D::CrossableTri3D(const Vect3D a, const Vect3D b, const Vect3D c) {
	set(a,b,c);
	s = Plane3D(a,b,c);	//s points up if verticies are in ACW

	//there are 2 rules for planes of sides:
	//they contain sides of triangle
	//their positive side is the triangle itself (they cannot be parallel with surface of triangle!)
	
	//sides define 2 points in space - a third point is needed for creating these planes
	const Vect3D d = (a+b+c)/3 - s.getN();	//weight point - n (-n so planes are not parallel with triangle)

	//this order is ACW from triangle => triangle is in their positive side
	bc = Plane3D(b,c, d);
	ca = Plane3D(c,a, d);
	ab = Plane3D(a,b, d);
}
void CrossableTri3D::set(const Vect3D a, const Vect3D b, const Vect3D c) {
	this->a = a;
	this->b = b;
	this->c = c;
}
Plane3D CrossableTri3D::surface() const {return s;}
bool CrossableTri3D::isCrossed(const Line3D line) const {
	if(line.isParallel(s)) return false;

	const Vect3D p = line.cross(s);
	if(bc.distsign(p) < 0 || ca.distsign(p) < 0 || ab.distsign(p) < 0) return false;

	return true;
}

bool CrossableTri3D::isCrossed(const HalfLine3D hline) const {
	if(! hline.isCrossing(s)) return false;
	return isCrossed((Line3D)hline);	//TODO: there is a faster way: if we open both functions here then we can reuse t
}

bool CrossableTri3D::isCrossed(const Sect3D sect) const {
	if(! sect.isCrossing(s)) return false;
	return isCrossed((Line3D)sect);	//TODO: there is a faster way: if we open both functions here then we can reuse t
}

Vect3D CrossableTri3D::getA() const {return a;}
Vect3D CrossableTri3D::getB() const {return b;}
Vect3D CrossableTri3D::getC() const {return c;}
//--------------------------------------Color----------------------------------------------------------------
Color::Color()		{set(0,0,0);}
Color::Color(const float r, const float g, const float b)		{set(r,g,b);}
void Color::set(const float r, const float g, const float b)	{this->r=r;this->g=g;this->b=b;}
bool Color::operator==(const Color o) const	{return r==o.r && b==o.g && g==o.b;}
bool Color::operator!=(const Color o) const	{return ! operator==(o);}
Color Color::operator+(const Color o) const	{return Color(r+o.r, g+o.g, b+o.b);}
Color Color::operator*(const Color o) const	{return Color(r*o.r, g*o.g, b*o.b);}
Color Color::operator/(const float v) const	{return Color(r/v, g/v, b/v);}
void Color::operator+=(const Color o)			{r+=o.r; g+=o.g; b+=o.b;}
float Color::getR() const	{return r;}
float Color::getG() const	{return g;}
float Color::getB() const	{return b;}
//--------------------------------------Foton----------------------------------------------------------------
Foton::Foton() {}
Foton::Foton(const Vect3D pos, const Color color) {
	this->pos = pos;
	this->color = color;
}
Vect3D Foton::getPos() const		{return pos;}
Color Foton::getColor() const		{return color;}
//--------------------------------------DetailedTri3D----------------------------------------------------------------
DetailedTri3D::DetailedTri3D(const Vect3D a, const Vect3D b, const Vect3D c) : CrossableTri3D(a,b,c)		{}
void DetailedTri3D::setActive(const Color active)		{this->active = active;}
void DetailedTri3D::setRefl(const Color refl)			{this->refl = refl;}
void DetailedTri3D::setTransp(const Color transp)		{this->transp = transp;}
void DetailedTri3D::setRefr(const float refr)			{this->refr = refr;}
Color DetailedTri3D::getActive() const					{return active;}
Color DetailedTri3D::getRefl() const					{return refl;}
Color DetailedTri3D::getTransp() const					{return transp;}
float DetailedTri3D::getRefr() const					{return refr;}
void DetailedTri3D::addFoton(const Foton foton) const	{fotons.push_back(foton);}
Color DetailedTri3D::getBrightess(const Vect3D pos, const float r) const {
	Color result;
	for(std::list<Foton>::const_iterator i = fotons.begin(); i != fotons.end(); i++) {
		const float dist2 = pos.dist2((*i).getPos());
		if(dist2 < r*r) result += (*i).getColor();
	}
	return result;
}
//--------------------------------------DetailedTri2D----------------------------------------------------------------
DetailedTri2D::DetailedTri2D() {}
DetailedTri2D::DetailedTri2D(const Vect2D a, const Vect2D b, const Vect2D c)	{set(a,b,c);}
void DetailedTri2D::set(const Vect2D a, const Vect2D b, const Vect2D c)		{this->a = a; this->b = b; this->c = c;}
void DetailedTri2D::setColor(const Color color)								{this->color = color;}
Vect2D DetailedTri2D::getA() const		{return a;}
Vect2D DetailedTri2D::getB() const		{return b;}
Vect2D DetailedTri2D::getC() const		{return c;}
Color DetailedTri2D::getColor() const	{return color;}