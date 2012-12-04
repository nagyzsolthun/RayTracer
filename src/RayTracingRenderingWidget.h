/** @file RayTracingRenderingWidget.h @brief thread and widget managing and showing process of rendering*/

#ifndef RAYTRACINGRENDERINGWIDGET_H
#define RAYTRACINGRENDERINGWIDGET_H

#include <QThread>
#include <QWidget>
#include <QProgressBar>

#include "RayTracing/Camera.h"

/** @brief Thread for rendering part of image: always whole lines.*/
class RayTracingThread : public QThread {
	Q_OBJECT
public:
	/**
	 * @brief construts a thread with given parameters.
	 * 
	 * @param cam camera for raytracing
	 * @param starty row where rendering starts on coordinate system of img
	 * @param endy row where rendering is finished on coordinate system of img
	 * @param img image storing result of rendering
	 * @param parent parent of thread
	 */
	RayTracingThread(const RayTracerCam * cam, const int starty, const int endy, QImage * img, QWidget * parent);
protected:
	/** @brief http://doc.qt.digia.com/qt/qthread.html#run */
	void run();
signals:
	/** @brief signal emited when rendering of one row is finished*/
	void rowRendered();
private:
	QColor toQColor(const Color color);

	const RayTracerCam * cam;
	int starty, endy;
	QImage * img;
};

/** @brief Wwidget that manage process of ray tracing and informs user about the status of rendering.*/
class RayTracingRenderingWidget : public QWidget {
	Q_OBJECT
public:
	/** @brief constructs a RayTracingRenderingWidget*/
	RayTracingRenderingWidget(QWidget * parent = 0);
	
	/**
	 * @brief starts rendering
	 * 
	 * @param cam camera for raytracing
	 * @param img QImage that stores result
	 */
	void render(const RayTracerCam * cam, QImage * img);
	
	/** @brief sets number of threads for rendering*/
	void setNumberofThreads(unsigned int numberofThreads);

signals:
	/** @brief emited when rendering is finished*/
	void finished();
private slots:
	void stepProgressBar();
	void threadFinished();
private:
	void renderThread(const RayTracerCam * cam, unsigned int starty, unsigned int endy, QImage * img);
	QProgressBar * progressBar;
	unsigned int progress;
	QList<RayTracingThread*> threads;
	unsigned int numberofThreads;
	unsigned int numberofActiveThreads;
	
};

#endif