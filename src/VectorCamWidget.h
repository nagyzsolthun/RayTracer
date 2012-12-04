/** @file VectorCamWidget.h @brief Widget for showing picture of -, and for setting direction and position of camera.*/

#ifndef VECTORCAMWIDGET_H
#define VECTORCAMWIDGET_H

#include <QFrame>
#include <QKeyEvent>

#include "Space2DDrawer.h"

#include "RayTracing/Camera.h"

/**
 * @brief Widget showing image of -, and controlling a VectorCam.
 * 
 * Changing position of camera: using keyboard and mouse: W,A,S,D => moving camera in space up,left,down,right. Pressing a mouse button and moving mouse => rotation of camera. Using mouse wheel => moving camera forward and backwards. These changes are communicated via signals.
 */
class VectorCamWidget : public QFrame {
	Q_OBJECT
public:
	/** @brief Constructs a VectorCamWidget with given space and parent.*/
	VectorCamWidget(DetailedSpace3D * space, QWidget * parent = 0);
	
	/** @brief emits signals that are conencted to details of camera -> for initialize raytracer cam*/
	void emitCamDetails();
protected:
	/**
	 * @brief Draws borders of triangles.
	 * 
	 * More information: http://qt-project.org/doc/qt-4.8/qwidget.html#paintEvent
	 */
	void paintEvent(QPaintEvent* evt);
	
	/**
	 * @brief Used for calculation of mouse movement while rotating camera.
	 * 
	 * Moving mouse while left button is pressed: rotation of camera.
	 * More information: http://qt-project.org/doc/qt-4.8/qwidget.html#mousePressEvent
	 */
	void mousePressEvent(QMouseEvent* evt);
	
	/**
	 * @brief Used for calculation of mouse movement while rotating camera.
	 * 
	 * Moving mouse while left button is pressed: rotation of camera.
	 * More information: http://qt-project.org/doc/qt-4.8/qwidget.html#mousePressEvent
	 */
	void mouseMoveEvent(QMouseEvent* evt);
	
	/**
	 * @brief Function that handles key presses for moving camera.
	 * 
	 * W: move camera up<br/>
	 * A: move camera left<br/>
	 * S: move camera right<br/>
	 * D: move camera down<br/>
	 * http://qt-project.org/doc/qt-4.8/qwidget.html#keyPressEvent
	 */
	void keyPressEvent(QKeyEvent *evt);
	
	/**
	 * @brief function that handes mouse wheel for moving camera.
	 * 
	 * Wheeling up: moves camera forward (in direction camera)<br/>
	 * Wheeling down: moves camera backwards<br/>
	 * http://qt-project.org/doc/qt-5.0/qwidget.html#wheelEvent
	 */
	void wheelEvent(QWheelEvent* evt);
signals:
	/** @brief emited when position of camera is changed*/
	void changedCameraPos(Vect3D pos);
	
	/** @brief emoted when horizontal or vertical normalvector or direction of camera is changed*/
	void changedCameraHVDir(Vect3D hdir, Vect3D vdir, Vect3D dir);
public slots:
	/** @brief Slot for changing Angle of View of camera*/
	void changeCameraAov(double value);
private:
	DetailedSpace3D * space;
	GeoRot3D rot;
	VectorCam cam;

	Space2DDrawer drawer;
	QPoint mouseTracer;		//variable for calculating movement of mouse
	Rot2D rotStep;		//variable holding size of a rotation
};

#endif