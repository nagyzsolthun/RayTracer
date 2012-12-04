/** @file RayTracingSettingsPanel.h @brief widget to set parameters of ray tracing*/

#ifndef RAYTRACINGSETTINGSPANEL_H
#define RAYTRACINGSETTINGSPANEL_H

#include <QFrame>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "RayTracing/Camera.h"
#include "RayTracingRenderingWidget.h"

/** @brief Widget that provides settings for ray tracing and holds tools for rendering.*/
class RayTracingSettingsPanel : public QFrame {
	Q_OBJECT
public:
	/** @brief Constructs a RayTracingSettingsPanel with given space and parent.*/
	RayTracingSettingsPanel(DetailedSpace3D * space, QWidget * parent = 0);
signals:
	/** @brief emited when angle of view is changed on panel*/
	void changedAov(double value);
	
	/** @brief emited when render button is clicked*/
	void renderButtonCLicked();
public slots:
	/** @brief change position of camera to given position*/
	void changeCameraPos(Vect3D pos);
	
	/** @brief change horizontal and vertical normal vectors + direction of camera*/
	void changeCameraHVDir(Vect3D hdir, Vect3D vdir, Vect3D dir);
private slots:
	void setAov(double value);
	void setRes();
	void setFocusDist(double value);
	void setDOF(double value);
	void setDensity(double value);
	void setThreadNum(int value);
	
	void render();
	void renderingFinished();
private:
	DetailedSpace3D * space;
	RayTracerCam cam;

	RayTracingRenderingWidget * renderingWidget;
	QImage * renderedImage;

	QPushButton* renderButton;
	QDoubleSpinBox * aovSpinBox;
	QSpinBox * xSpinBox;
	QSpinBox * ySpinBox;
	QDoubleSpinBox * focusSpinBox;
	QDoubleSpinBox * dofSpinBox;
	QDoubleSpinBox * densitySpinBox;
	QSpinBox * threadSpinBox;
};

#endif