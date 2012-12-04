#include "SceneSetterWidget.h"

#include <iostream>

SceneSetterWidget::SceneSetterWidget(DetailedSpace3D *space, QWidget *parent) : QSplitter(parent) {
	this->space = space;
	panel = new RayTracingSettingsPanel(space, this);
	scenescreen = new VectorCamWidget(space, this);

	addWidget(panel);
	addWidget(scenescreen);

	resize(640,480);
	setSizes(QList<int>() << 100 << 540);
	
	connect(panel, SIGNAL(changedAov(double)), scenescreen, SLOT(changeCameraAov(double)));
	
	connect(scenescreen, SIGNAL(changedCameraPos(Vect3D)), panel, SLOT(changeCameraPos(Vect3D)));
	connect(scenescreen, SIGNAL(changedCameraHVDir(Vect3D,Vect3D,Vect3D)), panel, SLOT(changeCameraHVDir(Vect3D,Vect3D,Vect3D)));
	
	scenescreen->emitCamDetails();
}