#include "RayTracing.h"

#include <cstdlib>

//--------------------------------------ViewRay----------------------------------------------------------------
ViewRay::ViewRay(const Vect3D a, const Vect3D b, const DetailedTri3D* startTri, const unsigned int depth) : HalfLine3D(a,b) {
	this->startTri = startTri;
	this->depth = depth;
	closest = 0;
}
Color ViewRay::shotAt(const DetailedSpace3D & space) {
	const Color BLACK = Color();

	for(DetailedSpace3D::const_iterator i = space.begin(); i!=space.end(); i++) shotAt(*i);
	if(! closest) return BLACK;	//no hit, return black

	Color result = closest->getActive();
	if(depth < 1) return result;	//no more recursion!

	if(closest->getRefl() != BLACK)
		result += ViewRay(closestCross, closestCross+reflV(), closest, depth-1).shotAt(space) * closest->getRefl();
	if(closest->getTransp() != BLACK)
		result += ViewRay(closestCross, closestCross+refrV(), closest, depth-1).shotAt(space) * closest->getTransp();
	return result;
}
void ViewRay::shotAt(const DetailedTri3D & acttri) {
	if(startTri == &acttri) return;
	if(! acttri.isCrossed(*this)) return;

	Vect3D actCross = cross(acttri.surface());
	const Vect3D p = getP();
	if(closest && p.dist2(actCross)>p.dist2(closestCross)) return;

	closest = &acttri;
	closestCross = actCross;
}
Vect3D ViewRay::reflV() const {
	//direction of reflection
	//assuming that closest is exist
	const Vect3D n = closest->surface().getN();
	const Vect3D v = getV();
	const float t = closest->surface().distsign(closestCross+v);
	return v - n*t*2;
}
Vect3D ViewRay::refrV() const {
	return getV();	//TODO
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