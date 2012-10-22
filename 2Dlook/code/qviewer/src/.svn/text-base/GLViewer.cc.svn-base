/*****************************************************************************\

GLViewer.cc
Author: Forrester Cole (fcole@cs.princeton.edu)
Copyright (c) 2009 Forrester Cole

dpix is distributed under the terms of the GNU General Public License.
See the COPYING file for details.

\*****************************************************************************/

#include "GLViewer.h"
#include <XForm.h>
#include <assert.h>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include "Scene.h"
#include "Stats.h"

static dkStringList _shadersList("Current->Shader",QStringList());
static dkStringList _model("Current->Mesh",QStringList(modelChoices));

GLViewer::GLViewer(QWidget* parent) : QGLViewer( parent )
{ 
	_inited = false;
	_visible = false;
	_display_timers = false;
	_new_scene = false;
	_scene = NULL;

    camera()->frame()->setWheelSensitivity(-1.0);
}

void GLViewer::resetView()
{
	vec center;
	float radius;

	if (_scene)
	{
		_scene->boundingSphere(center, radius,(ModelType) _model.index());

		setSceneRadius( radius );
		setSceneCenter( qglviewer::Vec( center[0], center[1], center[2] ) );
		camera()->setFieldOfView(3.1415926f / 6.0f);
		camera()->setZNearCoefficient(0.01f);
		camera()->setOrientation(0,0);
		xform cam_xf = xform(camera()->frame()->matrix());
		_scene->setCameraTransform(cam_xf);
		showEntireScene();
	}
}

void GLViewer::setScene( Scene* scene )
{ 
	bool was_inited = _inited;

	// Temporarily clear initialized flag to stop drawing
	// during initFromDOMElement.
	_inited = false;
	_scene = scene;
	_new_scene = true;

	if (!_scene->viewerState().isNull())
		initFromDOMElement(_scene->viewerState());

	_inited = was_inited;
}

void GLViewer::finishInit()
{
	_inited = true;
}

void GLViewer::resizeGL( int width, int height )
{
	if (width < 0 || height < 0) {
		_visible = false;
		return;
	}

	_visible = (width * height != 0);

	QGLViewer::resizeGL( width, height);
}

static bool in_draw_function = false;

void GLViewer::draw()
{  
	if (in_draw_function)
		return; // recursive draw

	if (!(_visible && _inited && _scene))
		return;

	in_draw_function = true;

	if (GQShaderManager::status() == GQ_SHADERS_NOT_LOADED)
	{
		GQShaderManager::initialize();
		_shadersList.clear();
		_shadersList.setChoices(GQShaderManager::getProgramsList());
	}

	if(_new_scene)
	{
		glClearColor(1,1,1,1);
		_scene->init();
		_new_scene =false;
		resetView();
	}

	if(_model.changedLastFrame())
		resetView();

	Stats& perf = Stats::instance();
	if (_display_timers)
	{
		perf.reset();
	}
	DialsAndKnobs::incrementFrameCounter();

	xform modelView_xf;
	camera()->getModelViewMatrix(modelView_xf);	
	_scene->setModelViewMatix(modelView_xf);

	xform proj_xf;
	camera()->getProjectionMatrix(proj_xf);
	_scene->setProjectionMatrix(proj_xf);
	qglviewer::Vec cameraPos = camera()->position();
	_scene->setCameraPosition(vec4f(cameraPos[0],cameraPos[1],cameraPos[2],cameraPos[3]));

	_scene->drawScene(_shadersList.value(),(ModelType) _model.index());

	if (_display_timers)
	{
		perf.updateView();
	}

	in_draw_function = false;
}


