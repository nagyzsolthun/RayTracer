#include "RayTracingSettingsPanel.h"

#include <iostream>

#include <QVBoxLayout>
#include <QLabel>

RayTracingSettingsPanel::RayTracingSettingsPanel(DetailedSpace3D * space, QWidget * parent) : QFrame(parent) {
	
	this->space = space;
	cam.setSpace(space);

	renderingWidget = new RayTracingRenderingWidget();
	connect(renderingWidget, SIGNAL(finished()), this, SLOT(renderingFinished()));
	
	renderButton = new QPushButton("render", this);
	connect(renderButton, SIGNAL(clicked()), this, SLOT(render()));
	
	aovSpinBox = new QDoubleSpinBox(this);
	aovSpinBox->setSingleStep(0.2);
	aovSpinBox->setValue(1.0);
	aovSpinBox->setMinimum(0.01);
	aovSpinBox->setValue(1.0);
	connect(aovSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setAov(double)));
	emit(setAov(1.0));	//inital set of camera
	
	xSpinBox = new QSpinBox(this);
	xSpinBox->setMinimum(1);
	xSpinBox->setMaximum(2048);
	xSpinBox->setValue(640);
	xSpinBox->setSuffix(" px");
	connect(xSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setRes()));
	
	ySpinBox = new QSpinBox(this);
	ySpinBox->setMinimum(1);
	ySpinBox->setMaximum(2048);
	ySpinBox->setValue(480);
	ySpinBox->setSuffix(" px");
	connect(ySpinBox, SIGNAL(valueChanged(int)), this, SLOT(setRes()));
	emit(setRes());
	
	focusSpinBox = new QDoubleSpinBox(this);
	focusSpinBox->setSingleStep(0.5);
	focusSpinBox->setValue(20.0);
	focusSpinBox->setMinimum(0.01);
	connect(focusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setFocusDist(double)));
	emit(setFocusDist(20.0));
		
	dofSpinBox = new QDoubleSpinBox(this);
	dofSpinBox->setSingleStep(0.1);
	dofSpinBox->setValue(1.0);
	connect(dofSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDOF(double)));
	emit(setDOF(1.0));
	
	densitySpinBox = new QDoubleSpinBox(this);
	densitySpinBox->setSingleStep(0.2);
	densitySpinBox->setMinimum(1);
	densitySpinBox->setMaximum(32);
	densitySpinBox->setValue(1.0);
	connect(densitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDensity(double)));
	emit(setDensity(1.0));
	
	threadSpinBox = new QSpinBox(this);
	threadSpinBox->setMinimum(1);
	threadSpinBox->setMaximum(1024);
	threadSpinBox->setValue(2);
	connect(threadSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setThreadNum(int)));
	emit(setThreadNum(2));

	QVBoxLayout * panellayout = new QVBoxLayout(this);
	panellayout->setAlignment(Qt::AlignTop);
	panellayout->addWidget(renderButton);
	panellayout->addWidget(new QLabel("AOV:", this));
	panellayout->addWidget(aovSpinBox);
	panellayout->addWidget(new QLabel("x resolution:", this));
	panellayout->addWidget(xSpinBox);
	panellayout->addWidget(new QLabel("y resolution:", this));
	panellayout->addWidget(ySpinBox);
	panellayout->addWidget(new QLabel("focus dist:", this));
	panellayout->addWidget(focusSpinBox);
	panellayout->addWidget(new QLabel("DOF:", this));
	panellayout->addWidget(dofSpinBox);
	panellayout->addWidget(new QLabel("density:", this));
	panellayout->addWidget(densitySpinBox);
	panellayout->addWidget(new QLabel("Number of Threads:", this));
	panellayout->addWidget(threadSpinBox);
	
	setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}

void RayTracingSettingsPanel::changeCameraPos(Vect3D pos)									{cam.setPos(pos);}
void RayTracingSettingsPanel::changeCameraHVDir(Vect3D hdir, Vect3D vdir, Vect3D dir)		{cam.setHVDir(hdir,vdir,dir);}

void RayTracingSettingsPanel::setAov(double value) {
	emit changedAov(value);
	cam.setAov(value);
}
void RayTracingSettingsPanel::setRes() {
	const int x = xSpinBox->value();
	const int y = ySpinBox->value();
	cam.setRes(x,y);
}
void RayTracingSettingsPanel::setFocusDist(double value)	{cam.setFocusDist(value);}
void RayTracingSettingsPanel::setDOF(double value)			{cam.setDof(value);}
void RayTracingSettingsPanel::setDensity(double value) 	{cam.setDensity(value);}
void RayTracingSettingsPanel::setThreadNum(int value)		{renderingWidget->setNumberofThreads(value);}
void RayTracingSettingsPanel::render() {
	renderedImage = new QImage(cam.getXres(), cam.getYres(), QImage::Format_RGB32);

	renderingWidget->render(&cam, renderedImage);
	renderingWidget->show();
}
void RayTracingSettingsPanel::renderingFinished() {
	renderingWidget->hide();

	QLabel * imageLabel = new QLabel();	//TODO: garbage collection
    imageLabel->setPixmap(QPixmap::fromImage(*renderedImage));
	imageLabel->show();
}