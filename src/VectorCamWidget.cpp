#include "VectorCamWidget.h"

#include <cmath>
#include <iostream>

VectorCamWidget::VectorCamWidget(DetailedSpace3D * space, QWidget * parent) : QFrame(parent) {
	this->space = space;

	cam.setPos(Vect3D(0,0,40));
	cam.setSpace(space);
	rotStep.set(M_PI/1440);

	setFocusPolicy(Qt::StrongFocus);	//to make keyboard visible
	setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}
void VectorCamWidget::emitCamDetails() {
	cam.setHVDir(rot);
	emit(changedCameraPos(cam.getPos()));
	emit(changedCameraHVDir(cam.getHdir(),cam.getVdir(),cam.getDir()));
}
void VectorCamWidget::paintEvent(QPaintEvent* evt) {
	QPainter painter(this);
	cam.setHVDir(rot);
	cam.setRes(width(), height());

	drawer.set(size());
	drawer.draw(painter, cam.project());
	painter.drawEllipse(width()/2-5, height()/2-5, 10, 10);

	QFrame::paintEvent(evt);		//frame is not visible othervise
}
void VectorCamWidget::mousePressEvent(QMouseEvent* evt) {
	mouseTracer = evt->pos();
}
void VectorCamWidget::mouseMoveEvent(QMouseEvent* evt) {
	int mx = -(evt->pos().x() - mouseTracer.x());	//negative because of working of GeoRot -> rotating with longitude is happening the same way as on maps, which is exactly the oposite way as we need..
	int my = -(evt->pos().y() - mouseTracer.y());	//negative because of the different coordinate systems (Qt uses decresing y)
	mouseTracer = evt->pos();

	rot.rotLon(rotStep*mx);
	rot.rotLat(rotStep*my);
	repaint();
	emit(changedCameraHVDir(cam.getHdir(),cam.getVdir(),cam.getDir()));
}
void VectorCamWidget::keyPressEvent(QKeyEvent *evt) {
	switch(evt->key()) {
		case Qt::Key_W: cam.stepPos(0,1,0); break;
		case Qt::Key_A: cam.stepPos(-1,0,0); break;
		case Qt::Key_S: cam.stepPos(0,-1,0); break;
		case Qt::Key_D: cam.stepPos(1,0,0); break;
		case Qt::Key_Y: rot.rotTwi(rotStep*(-1)); break;
		case Qt::Key_X: rot.rotTwi(rotStep); break;
		default: return;
	}
	repaint();
	emit(changedCameraPos(cam.getPos()));
}
void VectorCamWidget::wheelEvent(QWheelEvent* evt) {
	float v = evt->delta()/100;
	cam.stepPos(0,0,v);
	repaint();
	emit(changedCameraPos(cam.getPos()));
}

void VectorCamWidget::changeCameraAov(double value) {
	cam.setAov(value);
	repaint();
}