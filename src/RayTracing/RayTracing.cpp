#include "RayTracing.h"

#include <cstdlib>

//--------------------------------------Ray----------------------------------------------------------------
Ray::Ray(const Vect3D a, const Vect3D b, const DetailedTri3D* const startTri) : HalfLine3D(a,b) {
	this->startTri = startTri;
	closest = 0;
}
Vect3D Ray::reflV(const DetailedTri3D* const tri) const {
	//direction of reflection
	//assuming that tri is exist
	const Vect3D v = getV();
	const Plane3D surf = tri->surface();
	const Vect3D n = surf.getN();
	const float t = surf.distsign(surf.getP()+v);
	return v - n*t*2;
}
Vect3D Ray::refrV(const DetailedTri3D* const tri) const {
	return getV();	//TODO
}
void Ray::shotAt(const DetailedTri3D & acttri) const {
	if(startTri == &acttri) return;
	if(! acttri.isCrossed(*this)) return;

	const Vect3D actCross = cross(acttri.surface());
	const Vect3D p = getP();
	if(closest && p.dist2(actCross)>p.dist2(closestCross)) return;

	closest = &acttri;
	closestCross = actCross;
}
const DetailedTri3D * Ray::getClosest() const		{return closest;}
Vect3D Ray::getClosestCross() const					{return closestCross;}
//--------------------------------------ViewRay----------------------------------------------------------------
ViewRay::ViewRay(const Vect3D a, const Vect3D b, const DetailedTri3D* const startTri, const unsigned int depth) : Ray(a,b,startTri) {
	this->depth = depth;
}
Color ViewRay::shotAt(const DetailedSpace3D & space) const {
	const Color BLACK = Color();	//TODO: global constant

	for(DetailedSpace3D::const_iterator i = space.begin(); i!=space.end(); i++) Ray::shotAt(*i);
	const DetailedTri3D * closest = getClosest();	//TODO: nicer

	if(! closest) return BLACK;	//no hit, return black

	Color result = closest->getActive();
	if(depth < 1) return result;	//no more recursion!
	
	const Color refl = closest->getRefl();
	const Color transp = closest->getTransp();
	//these values could be put directly into the next expressions -> but it is readable this way
	//also compiler should recognize this optimizing option
	
	if(refl != BLACK) {
		const Vect3D a = getClosestCross();
		const Vect3D b = getClosestCross()+reflV(closest);
		//these values could be put directly as parameters down -> it is only readable this way
		result += ViewRay(a,b, getClosest(), depth-1).shotAt(space) * refl;
	}
	if(transp != BLACK) {
		const Vect3D a = getClosestCross();
		const Vect3D b = getClosestCross()+refrV(closest);
		result += ViewRay(a,b, getClosest(), depth-1).shotAt(space) * transp;
	}
	return result;
}
//--------------------------------------ViewRayGroup----------------------------------------------------------------
ViewRayGroup::ViewRayGroup(const Vect3D pos, const Vect3D focus, const Vect3D hdir, const Vect3D vdir, const float r, const float raydist) {
	this->pos = pos;
	this->focus = focus;
	this->hdir = hdir;
	this->vdir = vdir;
	this->r = r;
	this->raydist = raydist;
}
Color ViewRayGroup::shotAt(const DetailedSpace3D & space) const {
	Color resultSum;
	unsigned int count = 0;
	//float rndx,rndy;
	for(float x=-r; x<r; x+=raydist)
		for(float y=-r; y<r; y+=raydist)
			if(x*x + y*y < r*r) {
				count++;
				//rndx = (float)(rand()%1000)*0.001*raydist;
				//rndy = (float)(rand()%1000)*0.001*raydist;
				//resultSum += ViewRay(pos + hdir*(x*rndx) + vdir*(y*rndy), focus).shotAt(space);
				resultSum += ViewRay(pos + hdir*x + vdir*y, focus).shotAt(space);
			}
	return resultSum / count;
}
//--------------------------------------FotonRay----------------------------------------------------------------
FotonRay::FotonRay(const Vect3D a, const Vect3D b, const Color color, const DetailedTri3D* const startTri, const unsigned int depth) : Ray(a,b,startTri) {
	this->color = color;
	this->depth = depth;
}
void FotonRay::shotAt(const DetailedSpace3D & space) const {
	for(DetailedSpace3D::const_iterator i = space.begin(); i!=space.end(); i++) Ray::shotAt(*i);
	const DetailedTri3D * closest = getClosest();

	if(! closest) return;	//no hit
	if(depth < 1) return;	//no more recursion! TODO: check... number of recursion
	
	const Color BLACK = Color();
	const Color refl = closest->getRefl();
	const Color transp = closest->getTransp();
	//these values could be put directly into the next expressions -> but it is readable this way
	//also compiler should recognize this optimizing option
	
	if(refl != BLACK) { 
		const Vect3D a = getClosestCross();
		const Vect3D b = getClosestCross()+reflV(closest);
		//these values could be put directly as parameters down -> it is only readable this way
		FotonRay(a,b, color*refl, closest, depth-1).shotAt(space);
	}
	if(transp != BLACK) {
		const Vect3D a = getClosestCross();
		const Vect3D b = getClosestCross()+refrV(closest);
		FotonRay(a,b, color*transp, closest, depth-1).shotAt(space);
	}
}