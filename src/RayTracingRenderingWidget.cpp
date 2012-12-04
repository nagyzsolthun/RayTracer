#include "RayTracingRenderingWidget.h"

#include <iostream>

//--------------------------------------RayTracingThread----------------------------------------------------------------
RayTracingThread::RayTracingThread(const RayTracerCam * cam, const int starty, const int endy, QImage * img, QWidget * parent) : QThread(parent) {
	this->cam = cam;
	this->starty = starty;
	this->endy = endy;
	this->img = img;
}
void RayTracingThread::run() {
	const int resx = cam->getXres();
	const int resy = cam->getYres();
	for(int y=starty; y<endy; y++) {
		for(int x=0; x<resx; x++) {
			Color color = cam->calcColor(x - resx/2, resy/2-y);
			img->setPixel(x,y, toQColor(color).rgb() );
		}
		emit rowRendered();
	}
}
QColor RayTracingThread::toQColor(const Color color) {
	QColor result(255,255,255);
	const float r = color.getR();
	const float g = color.getG();
	const float b = color.getB();
	if(r <= 1.0) result.setRed(r*255);
	if(g <= 1.0) result.setGreen(g*255);
	if(b <= 1.0) result.setBlue(b*255);
	return result;
}
//--------------------------------------RayTracingRenderingWidget----------------------------------------------------------------
RayTracingRenderingWidget::RayTracingRenderingWidget(QWidget * parent) : QWidget(parent) {
	progressBar = new QProgressBar(this);

	progress = 0;
	numberofActiveThreads = 0;
}

void RayTracingRenderingWidget::render(const RayTracerCam * cam, QImage * img) {
	progressBar->setMinimum(0);
	progressBar->setMaximum(cam->getYres());
	progress = 0;
	progressBar->setValue(progress);

	numberofActiveThreads = numberofThreads;

	unsigned int ystep = cam->getYres()/numberofThreads;
	for(unsigned int i=0; i<numberofThreads-1; i++) renderThread(cam, i*ystep, (i+1)*ystep, img);
	//if number of lines cannot be seperated equally, some line may be missing
	renderThread(cam, (numberofThreads-1)*ystep, cam->getYres(), img);
}

void RayTracingRenderingWidget::setNumberofThreads(unsigned int numberofThreads) {this->numberofThreads = numberofThreads;}

void RayTracingRenderingWidget::renderThread(const RayTracerCam * cam, unsigned int starty, unsigned int endy, QImage * img) {
	RayTracingThread * thread = new RayTracingThread(cam,starty,endy, img, this);
	connect(thread, SIGNAL(rowRendered()), this, SLOT(stepProgressBar()));
	connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
	threads << thread;
	thread->start();
}

void RayTracingRenderingWidget::stepProgressBar() {
	progress++;
	progressBar->setValue(progress);
}

void RayTracingRenderingWidget::threadFinished() {
	numberofActiveThreads--;
	if(! numberofActiveThreads) {
		emit(finished());
		QList<RayTracingThread*>::iterator i;
		for (i = threads.begin(); i!=threads.end(); i++) delete(*i);
		threads.clear();
	}
}