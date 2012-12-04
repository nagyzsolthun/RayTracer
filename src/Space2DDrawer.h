/** @file Space2DDrawer.h @brief tool for DetailedSpaces to QPainter conversion*/

#ifndef SPACE2DDRAWER_H
#define SPACE2DDRAWER_H

#include <QPainter>
#include <DetailedSpaces.h>

/** @brief draws objects defined in DetailedSpaces on screen using Qt tools*/
class Space2DDrawer {
public:
	/** @brief creates a drawer with no size*/
	Space2DDrawer();
	
	/** @brief calculates origo of drawing: Qt uses different coordinate system than DetailedSpaces*/
	void set(QSize size = QSize(0,0));
	
	/** @brief draws one triangle with given painter*/
	void draw(QPainter & painter, const DetailedTri2D & tri) const;
	
	/** @brief draws a list of triangles with given painter*/
	void draw(QPainter & painter, const DetailedSpace2D & space) const;
private:
	QPointF origo;
	void calcOrigo(QSize size);
};

#endif