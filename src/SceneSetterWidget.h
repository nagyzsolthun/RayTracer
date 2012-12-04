/** @file SceneSetterWidget.h @brief main widget of application*/

#ifndef SCENESETTERWIDGET_H
#define SCENESETTERWIDGET_H

#include <QSplitter>

#include "RayTracingSettingsPanel.h"
#include "VectorCamWidget.h"
#include "RayTracingRenderingWidget.h"
#include "RayTracing/Camera.h"

/**
 * @brief Widget for setting position, direction and other (raytracing related) parameters for a scene.
 * 
 * Widget is devided into 2 parts: left part contains settings for raytracing, right part shows an image determining the position of triangles in space.
 */
class SceneSetterWidget : public QSplitter {
	Q_OBJECT
public:
	/**
	* @brief Constructs a SceneSetterWidget with given space and parent.
	*/
	SceneSetterWidget(DetailedSpace3D *space, QWidget *parent = 0);
private:
	RayTracingSettingsPanel * panel;
	VectorCamWidget * scenescreen;
	
	DetailedSpace3D * space;
};

#endif