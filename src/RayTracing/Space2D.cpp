#include "Space2D.h"

#include <cmath>
#include <iostream>
//--------------------------------------Vect2D----------------------------------------------------------------
Vect2D::Vect2D()								{set(0,0);}
Vect2D::Vect2D(const float x, const float y)	{set(x,y);}
void Vect2D::set(const float x, const float y)	{this->x = x; this->y = y;}
float Vect2D::comp(const Vect2D p) const {
	//Vector component of p that is parallel with this.
	//WARNING: this is null vector => result is nan
	//v := this
	//e: p + nt [n: 90 degree of this to left => n = (-y, x) ]
	//f: 0 + v*t
	//result: t in f
	const Vect2D v = *this;
	return (v*p) / (v*v);
}
float Vect2D::len2() const						{return x*x+y*y;}
float Vect2D::dist2(const Vect2D p) const		{return (*this-p).len2();}
Vect2D Vect2D::rot90() const					{return Vect2D(-y,x);}	//rot 90 degrees to left
Vect2D Vect2D::operator+(const Vect2D p) const	{return Vect2D(x+p.x,y+p.y);}
Vect2D Vect2D::operator-(const Vect2D p) const	{return Vect2D(x-p.x,y-p.y);}
Vect2D Vect2D::operator*(const float t) const	{return Vect2D(x*t,y*t);}
Vect2D Vect2D::operator/(const float t) const	{return Vect2D(x/t,y/t);}
float Vect2D::operator*(const Vect2D p) const	{return x*p.x + y*p.y;}
void Vect2D::operator+=(const Vect2D p)			{x+=p.x; y+=p.y;}
float Vect2D::getX() const						{return x;}
float Vect2D::getY() const						{return y;}
//--------------------------------------Line2D----------------------------------------------------------------
Line2D::Line2D() {}
Line2D::Line2D(const Vect2D a, const Vect2D b) {set(a,b);}
void Line2D::set(const Vect2D a, const Vect2D b) {p = a; v = b-a;}
float Line2D::distsign(const Vect2D q) const {
	//distance between q and line where unit is length of v; sign of value is positive if q is right to the line, negative if left
	//WARNING: v is null => result is nan
	//e: p + v*t
	//f: q + n*t
	//result: -t in f (solving: writing up e as equity using n: n*(x,y) = n*p  =>  n*(q + n*t) = n*p
	const Vect2D n = v.rot90();
	const float t = (n*(p-q)) / (n*n);
	return -t;
}
float Line2D::dist2(const Vect2D q) const {
	//square of distance between q and line
	const float t = distsign(q);
	return q.dist2(p+v*t);
}
Vect2D Line2D::getP() const	{return p;}
Vect2D Line2D::getV() const {return v;}
//--------------------------------------Rot2D----------------------------------------------------------------
Rot2D::Rot2D(const float f) {set(f);}
void Rot2D::set(const float f) {
	//f: rotation in [0, M_PI[ range
	this->f = f;
	
	//sin and cos are defined on some widely used values:
	//switch is not working with float values
	//TODO: check and make more precise - and also think over the type..
	if(f == 0) 					{cos=1;				sin=0; 					return;}
	if(f == (float)M_PI/4)		{cos=0;				sin=1;					return;}
	if(f == (float)M_PI/360)	{cos=0.999847695156;sin=0.0174524064373;	return;}
	if(f == (float)M_PI/1440)	{cos=0.999990480721;sin=0.00436330928475;	return;}
	
	cos = std::cos(f);
	sin = std::sin(f);
	std::cout << "used sin() and cos(), baad" << std::endl;
}
void Rot2D::operator+=(const Rot2D o) {
	float ncos = cos*o.cos - sin*o.sin;	//new value of cos, we store it so we can calcualte with the old one
	sin = sin*o.cos + cos*o.sin;
	cos = ncos;
	f += o.f;
}
void Rot2D::operator-=(const Rot2D o) {
	float ncos = cos*o.cos + sin*o.sin;
	sin = sin*o.cos - cos*o.sin;
	cos = ncos;
	f -= o.f;
}
Rot2D Rot2D::operator*(int v) const {
	Rot2D result;
	if(v>0) for(int i=0; i<v; i++) result+=*this;
	if(v<0) for(int i=v; i<0; i++) result-=*this;
	return result;
}
Vect2D Rot2D::dir() const	{return Vect2D(cos,sin);}
float Rot2D::getCos() const		{return cos;}
float Rot2D::getSin() const		{return sin;}
float Rot2D::getF() const	{return f;}