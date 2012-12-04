#include "Space2DDrawer.h"
#include "RayTracing/DetailedSpaces.h"

#include <iostream>

Space2DDrawer::Space2DDrawer()	{}
void Space2DDrawer::set(QSize size) {calcOrigo(size);}
void Space2DDrawer::draw(QPainter & painter, const DetailedTri2D & tri) const {
	const QPointF a(tri.getA().getX(), -tri.getA().getY());
	const QPointF b(tri.getB().getX(), -tri.getB().getY());
	const QPointF c(tri.getC().getX(), -tri.getC().getY());

	QPolygonF qtri;
	qtri << origo+a << origo+b << origo+c;

	painter.drawPolygon(qtri);
}
void Space2DDrawer::draw(QPainter & painter, const DetailedSpace2D & space) const {
	for(DetailedSpace2D::const_iterator i = space.begin(); i != space.end(); i++) draw(painter, *i);
}
//privates:
void Space2DDrawer::calcOrigo(QSize size) {
	origo.setX(size.width()/ 2.0);
	origo.setY(size.height()/2.0);
}

