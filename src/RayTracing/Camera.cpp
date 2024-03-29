#include "Camera.h"

#include <iostream>
//--------------------------------------GeoRot3D----------------------------------------------------------------
GeoRot3D::GeoRot3D() : x0(1,0,0), y0(0,1,0), z0(0,0,1) {}
void GeoRot3D::rotLon(const Rot2D r) {lon+=r;}
void GeoRot3D::rotLat(const Rot2D r) {lat+=r;}
void GeoRot3D::rotTwi(const Rot2D r) {twi+=r;}
Vect3D GeoRot3D::getX() const {
	//first I rotate around y0 with longitude
	const Vect3D x1 = Axis3D(y0).rot(x0,lon);
	const Vect3D z = getZ();	//then I find out axis of twist
	return Axis3D(z).rot(x1,twi);
}
Vect3D GeoRot3D::getY() const {
	const Vect3D z = getZ();
	const Vect3D x = getX();
	return Vect3D(z,x);
}
Vect3D GeoRot3D::getZ() const {
	//this is the simpliest, it does not depend on twist
	const Vect3D z1 = Axis3D(y0).rot(z0,lon);	//we first rotate with longitude
	const Vect3D x1 = Axis3D(y0).rot(x0,lon);	//then we need to find out the axis of latitude..
	return Axis3D(x1).rot(z1,lat);
}
//--------------------------------------Movable3DTool----------------------------------------------------------------
Movable3DTool::Movable3DTool() {}
void Movable3DTool::setPos(const Vect3D pos) {this->pos = pos;}
void Movable3DTool::setHVDir(const Vect3D hdir, const Vect3D vdir, const Vect3D dir) {
	this->hdir = hdir;
	this->vdir = vdir;
	this->dir = dir;
}
void Movable3DTool::setHVDir(const GeoRot3D rot) {
	setHVDir(rot.getX()
		, rot.getY()
		, -rot.getZ());	//TODO: document this
}
void Movable3DTool::stepPos(const float thdir, const float tvdir,const float tdir) {
	pos += hdir*thdir;
	pos += vdir*tvdir;
	pos += dir*tdir;
}
Vect3D Movable3DTool::getPos() const	{return pos;}
Vect3D Movable3DTool::getHdir() const	{return hdir;}
Vect3D Movable3DTool::getVdir() const	{return vdir;}
Vect3D Movable3DTool::getDir() const	{return dir;}
//--------------------------------------AbstractCam----------------------------------------------------------------
AbstractCam::AbstractCam() : Movable3DTool() {
	space = NULL;
	aov = 1.0;
	setRes(0,0);
}
void AbstractCam::setSpace(const DetailedSpace3D * const space) {this->space = space;}
void AbstractCam::setAov(const float aov)						{this->aov = aov;}
void AbstractCam::setRes(const float xres, const float yres) 	{this->xres = xres; this->yres = yres;}
const DetailedSpace3D* AbstractCam::getSpace() const	{return space;}
float AbstractCam::getAov() const						{return aov;}
int AbstractCam::getXres() const						{return xres;}
int AbstractCam::getYres() const						{return yres;}
float AbstractCam::getAvgRes() const					{return (float)(xres+yres)/2.0;}
//--------------------------------------VectorCam----------------------------------------------------------------
VectorCam::VectorCam() {calcPlanes();}
void VectorCam::setPos(const Vect3D pos) {
	Movable3DTool::setPos(pos);
	calcPlanes();
}
void VectorCam::setHVDir(const Vect3D hdir, const Vect3D vdir, const Vect3D dir) {
	Movable3DTool::setHVDir(hdir,vdir,dir);
	calcPlanes();
}
void VectorCam::setHVDir(const GeoRot3D rot) {
	Movable3DTool::setHVDir(rot);
	calcPlanes();
}
void VectorCam::stepPos(const float thdir, const float tvdir,const float tdir) {
	Movable3DTool::stepPos(thdir,tvdir,tdir);
	calcPlanes();
}
DetailedSpace2D VectorCam::project() const {
	DetailedSpace2D result;
	for(DetailedSpace3D::const_iterator i = getSpace()->begin(); i!=getSpace()->end(); i++)
		if(isVisible(*i)) result.push_back(project(*i));
	return result;
}
//privates:
Vect2D VectorCam::project(const Vect3D & vect) const {
	//projection of 1 vector
	//assuming that the vector is before the screen plane
	const Vect3D proj = Line3D(getPos(),vect).cross(screen);
	const Vect2D result(vplain.distsign(proj)/getAov(), hplain.distsign(proj)/getAov());
	return result * getAvgRes();
}
bool VectorCam::isVisible(const DetailedTri3D & tri) const {
	//returns true if all poles of the triangle is before the screen plane
	if(screen.distsign(tri.getA()) <= 0) return false;
	if(screen.distsign(tri.getB()) <= 0) return false;
	if(screen.distsign(tri.getC()) <= 0) return false;
	return true;
}
DetailedTri2D VectorCam::project(const DetailedTri3D & tri) const {
	//assuming that the triangle is visible
	DetailedTri2D result( project(tri.getA()),
				project(tri.getB()),
				project(tri.getC()) );
	result.setColor(tri.getActive());
	return result;
}
void VectorCam::calcPlanes() {
	const Vect3D p = getPos() + getDir();	//focuspoint of projection is not on screenplane this way
	hplain = Plane3D(p, getVdir());
	vplain = Plane3D(p, getHdir());
	screen = Plane3D(p, getDir());
}
//--------------------------------------RayTracerCam----------------------------------------------------------------
RayTracerCam::RayTracerCam() : AbstractCam() {
	setFocusDist(1);
	setDof(1);
	setDensity(1);
}
void RayTracerCam::setFocusDist(float fdist)		{this->fdist = fdist;}
void RayTracerCam::setDof(float dof)				{this->dof = dof;}
void RayTracerCam::setDensity(float density)		{this->density = density;}
Color RayTracerCam::calcColor(const int x, const int y) const {
	const Vect3D pos = getPos();
	const Vect3D dir = getDir();
	const Vect3D hdir = getHdir();
	const Vect3D vdir = getVdir();
	const float pdist = getAov() / getAvgRes();
	
	const Color result =
		ViewRayGroup(pos,
				pos + (dir + hdir*x*pdist + vdir*y*pdist)*fdist,
				hdir, vdir, dof, dof/density).shotAt(*getSpace());
	return result;
}
//--------------------------------------Lamp----------------------------------------------------------------
Lamp::Lamp() {}
void Lamp::setRes(const float hangle, const float vangle, const float rangle) {
	unsigned int hres = hangle / rangle;
	unsigned int vres = vangle / rangle;
	screen = std::vector<std::vector<Vect3D> > (hres, std::vector<Vect3D>(vres));	//nice one, isnt it?
	
	//we will rotate around this
	Axis3D y(getVdir());
	
	/*const Rot2D srot(rangle);	//step rotation
	const Rot2D hrot(-hres/2);	//horizontal rotation for each rays - starting from left
	const rot2D vrot(-vres/2);	//vertical rotation for each rays - starting from bottom
	
	const Vect3D dir0 = getDir();
	const Vect3D hdir0 = getHdir();
	
	short int d = 1;	//direction of rendering
	for(unsigned int i=0; i<hres; i++) {
		const Vect3D hdir1 = y.rot(hdir0, hrot);
		const Axis3D x1(hdir1);
		
		const Vect3D dir1 = x1.rot(dir0,vrot);
		const Vect3D dir2 = y.rot(dir1,hrot);

		for(unsigned int j=0; j<vres; j++) {
			hrot += srot;
			//screen[i][j].set( 
		}
	}*/
}
void Lamp::shotAt(const DetailedSpace3D * const space) const {
	//TODO
};