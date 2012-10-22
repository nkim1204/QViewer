/*****************************************************************************\

Scene.cc
Author: Forrester Cole (fcole@cs.princeton.edu)
Copyright (c) 2009 Forrester Cole

qviewer is distributed under the terms of the GNU General Public License.
See the COPYING file for details.

\*****************************************************************************/

#include "Scene.h"
#include "GLViewer.h"
#include "TriMesh.h"

#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>

#include "GQDraw.h"
using namespace GQDraw;

#include <assert.h>

const int CURRENT_VERSION = 1;

static dkBool _useSilhouettes("Silhouettes->Enable", false);
static dkFloat _seuil("Silhouettes->Threshold", 0.0005);

static dkBool _useEnvMap("Env. Map.->Enable", false);

static dkBool  _useHDR("Tone-mapping->Enable", false);
static dkFloat _gamma("Tone-mapping->Gamma", 1.0, 0.01, 10.0, 0.01);
static dkFloat _exposure("Tone-mapping->Exposure", 1.0, 0.01, 10.0, 0.01);

static dkBool _warp("Warp-> Enable", false);
static dkFloat _warpDepthThres("Warp->Depth Threshold", 0.0005);
static dkFloat _warpNormalThres("Warp->Normal Threshold",0.0005);

static float t;
static GLint viewport[4];

Scene::Scene()
{
	_trimesh = NULL;
	_shadowTextureSize = 1024;
    t = 0;
}

Scene::~Scene()
{
	clear();
	delete _cube;
	delete _sphereBackground;
	delete _sphere;
	delete _quad;
	delete _ground;
	delete _hdrTexReflection;
	delete _tex_earth_day;
	delete _tex_earth_night;
	delete _tex_earth_bump;
	delete _tex_earth_normals;
    delete _tex_gray_scale;
}

void Scene::init()
{
	// Chargement des textures
	_tex_earth_day   = new GQTexture2D();
	_tex_earth_night = new GQTexture2D();
	_tex_earth_bump = new GQTexture2D();
	_tex_earth_normals = new GQTexture2D();
    _tex_gray_scale = new GQTexture2D();

	if (!_tex_earth_day->load("textures/earth1.png")   || 
		!_tex_earth_night->load("textures/earth2.png") ||
		!_tex_earth_normals->load("textures/earth3.png") ||
        !_tex_earth_bump->load("textures/earth4.png") ||
        !_tex_gray_scale->load("textures/grayscale.png"))
	{
		QMessageBox::critical(NULL, "Open Failed", "Failed to load texture");
	}

    _tex_gray_scale->setWarpMode(GL_CLAMP_TO_EDGE);

	_sphere = new Sphere();
	_sphereBackground = new Sphere(8.f);
	_cube = new Cube();
	_quad = new Quad();
	_ground = new Quad();

	_hdrTexReflection = new GQCubeMap();
	_hdrTexReflection->load("textures/hdr/uffizi_cross2.hdr");
}

void Scene::clear()
{
	delete _trimesh;
	_trimesh = NULL;
	_vertex_buffer_set.clear();
	_viewer_state.clear();
	_dials_and_knobs_state.clear();
}


bool Scene::load( const QString& filename )
{
	if (filename.endsWith(fileExtension()))
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly))
		{
			qWarning("Could not open %s", qPrintable(filename));
			return false;
		}

		QDomDocument doc("scene");
		QString parse_errors;
		if (!doc.setContent(&file, &parse_errors))
		{
			qWarning("Parse errors: %s", qPrintable(parse_errors));
			return false;
		}

		file.close();

		QDomElement root = doc.documentElement();
		QDir path = QFileInfo(filename).absoluteDir();

		return load(root, path); 
	}
	else
	{
		_trimesh = TriMesh::read(qPrintable(filename));
		_trimesh_filename = filename;
		if (!_trimesh)
		{
			clear();
			return false;
		}
		_viewer_state.clear();
		_dials_and_knobs_state.clear();
		setupMesh();

		return true;
	}
}

bool Scene::load( const QDomElement& root, const QDir& path )
{
	int version = root.attribute("version").toInt();
	if (version != CURRENT_VERSION)
	{
		qWarning("Scene::load: file version out of date (%d, current is %d)", 
			version, CURRENT_VERSION);
		return false;
	}

	QDomElement model = root.firstChildElement("model");
	if (model.isNull())
	{
		qWarning("Scene::load: no model node found.\n");
		return false;
	}

	QString relative_filename = model.attribute("filename");
	QString abs_filename = path.absoluteFilePath(relative_filename);

	_trimesh = TriMesh::read(qPrintable(abs_filename));
	_trimesh_filename = abs_filename;
	if (!_trimesh)
	{
		qWarning("Scene::load: could not load %s\n", 
			qPrintable(abs_filename));
		return false;
	}

	_viewer_state = root.firstChildElement("viewerstate");
	if (_viewer_state.isNull())
	{
		qWarning("Scene::load: no viewerstate node found.\n");
		clear();
		return false;
	}

	_dials_and_knobs_state = root.firstChildElement("dials_and_knobs");
	if (_dials_and_knobs_state.isNull())
	{
		qWarning("Scene::load: no dials_and_knobs node found.\n");
		return false;
	}
	setupMesh();

	return true;
}

bool Scene::save(const QString& filename, const GLViewer* viewer,
				 const DialsAndKnobs* dials_and_knobs)
{
	QDomDocument doc("scene");
	QDomElement root = doc.createElement("scene");
	doc.appendChild(root);

	_viewer_state = viewer->domElement("viewerstate", doc);
	_dials_and_knobs_state = 
		dials_and_knobs->domElement("dials_and_knobs", doc);

	QDir path = QFileInfo(filename).absoluteDir();

	bool ret = save(doc, root, path);
	if (!ret)
		return false;

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		qWarning("Scene::save - Could not save %s", qPrintable(filename));
		return false;
	}

	file.write(doc.toByteArray());

	file.close();

	return true;
}

bool Scene::save( QDomDocument& doc, QDomElement& root, const QDir& path )
{
	root.setAttribute("version", CURRENT_VERSION);

	QDomElement model = doc.createElement("model");
	model.setAttribute("filename", path.relativeFilePath(_trimesh_filename));
	root.appendChild(model);

	root.appendChild(_viewer_state);
	root.appendChild(_dials_and_knobs_state);

	return true;
}

void Scene::loadEnvMap( const QString &filename )
{
	delete _hdrTexReflection;
	_hdrTexReflection = new GQCubeMap();
	_hdrTexReflection->load(filename);
}

void Scene::boundingSphere(vec& center, float& radius, const ModelType type)
{
	center = vec();
	switch(type)
	{
	case CUBE:
		radius = _cube->radius();
		break;
	case SPHERE:
		radius = _sphere->radius();
		break;
    case MESH:
		center = _trimesh->bsphere.center;
		radius = _trimesh->bsphere.r;
		break;
	default:
		break;
	}
	if(_useEnvMap)
	{
		delete _sphereBackground;
		_sphereBackground = new Sphere(8.0f*radius);
		radius = _sphereBackground->radius();
	}
}

void Scene::setupTextures(GQShaderRef& shader)
{
	if(shader.uniformLocation("textureDay")>=0)
		shader.bindNamedTexture("textureDay",_tex_earth_day);
	if(shader.uniformLocation("textureNight")>=0)
		shader.bindNamedTexture("textureNight",_tex_earth_night);
	if(shader.uniformLocation("textureBump")>=0)
		shader.bindNamedTexture("textureBump",_tex_earth_bump);
	if(shader.uniformLocation("textureNormals")>=0)
		shader.bindNamedTexture("textureNormals",_tex_earth_normals);
    if(shader.uniformLocation("textureGrayScale")>=0)
        shader.bindNamedTexture("textureGrayScale",_tex_gray_scale);
}

void Scene::drawScene(const QString &programName, const ModelType type)
{
	if (GQShaderManager::status() != GQ_SHADERS_OK)
		return;

    glGetIntegerv(GL_VIEWPORT, viewport);

    GQShaderRef shader;

    if(_useHDR)
    {
        _fbo.initFullScreen(1,GQ_ATTACH_DEPTH,GQ_COORDS_NORMALIZED);
        _fbo.bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    if(_useEnvMap)
        drawEnvMap();

    glViewport(viewport[0],viewport[1],4*viewport[2],4*viewport[3]);


    shader = GQShaderManager::bindProgram("wf");

    _warpFbo.initFullScreen(1,GQ_ATTACH_DEPTH_TEXTURE,GQ_COORDS_NORMALIZED,GQ_FORMAT_RGBA_FLOAT);
//    _warpFbo.init(2*viewport[2],2*viewport[3],1,GQ_ATTACH_DEPTH_TEXTURE,GQ_COORDS_NORMALIZED,GQ_FORMAT_RGBA_FLOAT);
    glClearColor(0,0,0,0);
    _warpFbo.bind(GQ_CLEAR_BUFFER);

    shader.setUniform1f("width", _warpFbo.colorTexture(0)->width());
    shader.setUniform1f("height", _warpFbo.colorTexture(0)->height());
    shader.setUniform1f("t",t);

    _quad->draw(shader);
    _warpFbo.unbind();
    shader.unbind();


	shader = GQShaderManager::bindProgram(programName);

    glClearColor(1,1,1,1);

	// Envoie des variables uniformes (matrices de transformation)
	shader.setUniformXform("projection_matrix",_projection_matrix);
	shader.setUniformXform("model_view_matrix", _modelView_matrix);
	if(shader.uniformLocation("normal_matrix")>=0)
	{
		xform normal_matrix;
		normal_matrix = inv(_modelView_matrix);
		normal_matrix = transpose(normal_matrix);
		shader.setUniformMatrixUpper3x3("normal_matrix", normal_matrix);
	}

	setupLighting(shader);

	setupTextures(shader);


	if(_useSilhouettes)
	{
        _fbo.initFullScreen(1,GQ_ATTACH_DEPTH_TEXTURE,GQ_COORDS_NORMALIZED,GQ_FORMAT_RGBA_BYTE);
		_fbo.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

    if(_warp)
    {
        _fbo.initFullScreen(3,GQ_ATTACH_DEPTH_TEXTURE,GQ_COORDS_NORMALIZED,GQ_FORMAT_RGBA_FLOAT);
//        _fbo.init(2*viewport[2],2*viewport[3],3,GQ_ATTACH_DEPTH_TEXTURE,GQ_COORDS_NORMALIZED,GQ_FORMAT_RGBA_FLOAT);
        glClearColor(0,0,0,0);
        _fbo.bind(GQ_CLEAR_BUFFER);
    }

	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if(_useEnvMap && shader.uniformLocation("cubemap")>=0)
	{
		shader.bindNamedTexture("cubemap",_hdrTexReflection);
		shader.setUniform4fv("camera_position",_camera_position);
	}

	switch(type)
	{
	case CUBE:
		_cube->draw(shader);
		break;
	case SPHERE:
		_sphere->draw(shader);
		break;
    case MESH:
		drawMesh(shader);
		break;
	default:
		break;
	}

	if(_useSilhouettes)
	{
		_fbo.unbind();
		shader.unbind();
        extractSilhouettes();
	}

	if(_useHDR)
	{
		_fbo.unbind();
		shader.unbind();
		toneMap();
	}

    if(_warp)
    {
        t += 0.01;
        if(t > 1.0){
            t = 0;
        }
        _fbo.unbind();
        shader.unbind();
        warp();
        shader = GQShaderManager::bindProgram("finalColor");
        glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
        shader.setUniform1f("width", _finalFbo.colorTexture(0)->width()/4);
        shader.setUniform1f("height", _finalFbo.colorTexture(0)->height()/4);
        glActiveTexture(GL_TEXTURE0);
        glEnable(_finalFbo.colorTexture(0)->target());
        _finalFbo.colorTexture(0)->bind();
        shader.setUniform1i("colorMap",0);
        _quad->draw(shader);
        shader.unbind();
    }
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

}

void Scene::setupMesh()
{
	_trimesh->need_bsphere();
	_trimesh->need_bbox();
	_trimesh->need_normals();
	_trimesh->need_tstrips();
	_trimesh->need_faces();
	_trimesh->need_uv_dirs();
}

void Scene::setupVertexBufferSet()
{
	// Trimesh stores triangle strips as length followed by indices.
	// Grab them and store the offsets and lengths.
	_tristrips.clear();
	const int *t = &_trimesh->tstrips[0];
	const int *end = t + _trimesh->tstrips.size();
	while (t < end) {
		int striplen = *t++;
		_tristrips.push_back(striplen);
		t += striplen;
	}

	_vertex_buffer_set.clear();
	_vertex_buffer_set.add(GQ_VERTEX, _trimesh->vertices);
	_vertex_buffer_set.add(GQ_NORMAL, _trimesh->normals);
	if(!_trimesh->colors.empty())
		_vertex_buffer_set.add(GQ_COLOR,  _trimesh->colors);
	//if(!_trimesh->udirs.empty())
	//	_vertex_buffer_set.add(GQ_TANGENT,  _trimesh->udirs);
    if(!_trimesh->texcoords.empty())
		_vertex_buffer_set.add(GQ_TEXCOORD, _trimesh->texcoords);
	_vertex_buffer_set.add(GQ_INDEX, 1, _trimesh->tstrips);

	_vertex_buffer_set.copyToVBOs();
}	

void Scene::drawMesh(GQShaderRef& shader)
{
	if (_vertex_buffer_set.numBuffers() == 0)
		setupVertexBufferSet();

	assert(_trimesh->tstrips.size() > 0);

	_vertex_buffer_set.bind(shader);

	int offset = 1;
	for (int i = 0; i < _tristrips.size(); i++) {
		drawElements(_vertex_buffer_set, GL_TRIANGLE_STRIP, offset, _tristrips[i]); 
		offset += _tristrips[i] + 1; // +1 to skip the length stored in the tristrip array.
	}

	_vertex_buffer_set.unbind();
}

void Scene::warp()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    GQShaderRef shader = GQShaderManager::bindProgram("16_warp");
    setupLighting(shader);

    _finalFbo.initFullScreen(1,GQ_ATTACH_DEPTH_TEXTURE,GQ_COORDS_NORMALIZED,GQ_FORMAT_RGBA_FLOAT);
    glClearColor(0,0,0,0);
    _finalFbo.bind(GQ_CLEAR_BUFFER);

    glActiveTexture(GL_TEXTURE0);
    glEnable(_fbo.colorTexture(0)->target());
    _fbo.colorTexture(0)->bind();
    shader.setUniform1i("colorMap",0);

    glActiveTexture(GL_TEXTURE1);
    glEnable(_fbo.colorTexture(1)->target());
    _fbo.colorTexture(1)->bind();
    shader.setUniform1i("normalMap",1);

    glActiveTexture(GL_TEXTURE2);
    glEnable(_fbo.colorTexture(2)->target());
    _fbo.colorTexture(2)->bind();
    shader.setUniform1i("posMap",2);

    glActiveTexture(GL_TEXTURE3);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,_fbo.depthMapId());
    shader.setUniform1i("depthMap",3);

    glActiveTexture(GL_TEXTURE4);
    glEnable(_warpFbo.colorTexture(0)->target());
    _warpFbo.colorTexture(0)->bind();
    shader.setUniform1i("offsetMap",4);

//    qCritical("Width: %d, Height: %d, VP_WIDTH: %d, VP_HEIGHT: %d",_fbo.colorTexture(0)->width(), _fbo.colorTexture(0)->height(), _finalFbo.colorTexture(0)->width(),_finalFbo.colorTexture(0)->height());

    shader.setUniform1f("width", _fbo.colorTexture(0)->width());
    shader.setUniform1f("height", _fbo.colorTexture(0)->height());
    shader.setUniform1f("depthThreshold", _warpDepthThres);
    shader.setUniform1f("normThreshold",_warpNormalThres);

    _quad->draw(shader);

    glActiveTexture( GL_TEXTURE0 );
    glDisable(_fbo.colorTexture(0)->target());

    glActiveTexture( GL_TEXTURE1 );
    glDisable(_fbo.colorTexture(1)->target());

    glActiveTexture( GL_TEXTURE2 );
    glDisable(_fbo.colorTexture(2)->target());

    glActiveTexture( GL_TEXTURE3 );
    glDisable(GL_TEXTURE_2D);

    glActiveTexture( GL_TEXTURE4 );
    glDisable(_warpFbo.colorTexture(0)->target());

    _finalFbo.unbind();
    shader.unbind();

}

void Scene::extractSilhouettes()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Activation du shader d'extraction des silhouettes
    GQShaderRef shader = GQShaderManager::bindProgram("11_quad");

    glActiveTexture(GL_TEXTURE0);
    glEnable(_fbo.colorTexture(0)->target());
    _fbo.colorTexture(0)->bind();
    shader.setUniform1i("colorMap",0);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_fbo.depthMapId());
	shader.setUniform1i("depthMap",1);

    shader.setUniform1f("width", _fbo.colorTexture(0)->width());
	shader.setUniform1f("height", _fbo.colorTexture(0)->height());
	shader.setUniform1f("threshold", _seuil);

	_quad->draw(shader);

	glActiveTexture( GL_TEXTURE0 );
	glDisable(_fbo.colorTexture(0)->target());

	glActiveTexture( GL_TEXTURE1 );
	glDisable(GL_TEXTURE_2D);

	shader.unbind();
}

void Scene::drawEnvMap()
{
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	GQShaderRef shader = GQShaderManager::bindProgram("12_background");

	// Envoie des variables uniformes (matrices de transformation)
	shader.setUniformXform("projection_matrix",_projection_matrix);
	shader.setUniformXform("model_view_matrix", _modelView_matrix);

	shader.bindNamedTexture("cubemap",_hdrTexReflection);

	if(shader.uniformLocation("normal_matrix")>=0)
	{
		xform normal_matrix(_modelView_matrix);
		invert(normal_matrix);
		normal_matrix = transpose(normal_matrix);
		shader.setUniformMatrixUpper3x3("normal_matrix", normal_matrix);
	}

	_sphereBackground->draw(shader);

	shader.unbind();
}

void Scene::toneMap()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Activation du shader de tone mapping
	GQShaderRef shader = GQShaderManager::bindProgram("14_tonemapping");

	glActiveTexture(GL_TEXTURE0);
	glEnable(_fbo.colorTexture(0)->target());
	_fbo.colorTexture(0)->bind();
	shader.setUniform1i("colorMap",0);

	shader.setUniform1f("width", _fbo.colorTexture(0)->width());
	shader.setUniform1f("height", _fbo.colorTexture(0)->height());
	shader.setUniform1f("gamma", _gamma);
	shader.setUniform1f("exposure", _exposure);

	_quad->draw(shader);

	glActiveTexture( GL_TEXTURE0 );
	glDisable(_fbo.colorTexture(0)->target());

	shader.unbind();
}

static QStringList lightPresetNames = (QStringList() << "Headlight" <<
									   "North" << "North-Northeast" << "Northeast" << "East-Northeast" <<
									   "East" << "East-Southeast" << "Southeast" << "South-Southeast" <<
									   "South" << "South-Southwest" << "Southwest" << "West-Southwest" <<
									   "West" << "West-Northwest" << "Northwest" << "North-Northwest" );
static dkStringList light_preset("Light->Direction", lightPresetNames);
static dkFloat light_depth("Light->Depth", 1.0f, 0.0f, 2.0f, 0.1f);

void Scene::setupLighting(GQShaderRef& shader)
{
	const QString& preset = light_preset.value();
	vec camera_light;

	if (preset == "Headlight")
		camera_light = vec(0.0f, 0.0f, 1.0f);
	else if (preset == "North")
		camera_light = vec(0.0f, 1.0f, light_depth);
	else if (preset == "North-Northeast")
		camera_light = vec(0.374f, 1.0f, light_depth);
	else if (preset == "Northeast")
		camera_light = vec(1.0f, 1.0f, light_depth);
	else if (preset == "East-Northeast")
		camera_light = vec(1.0f, 0.374f, light_depth);
	else if (preset == "East")
		camera_light = vec(1.0f, 0.0f, light_depth);
	else if (preset == "East-Southeast")
		camera_light = vec(1.0f, -0.374f, light_depth);
	else if (preset == "Southeast")
		camera_light = vec(1.0f, -1.0f, light_depth);
	else if (preset == "South-Southeast")
		camera_light = vec(0.374f, -1.0f, light_depth);
	else if (preset == "South")
		camera_light = vec(0.0f, -1.0f, light_depth);
	else if (preset == "South-Southwest")
		camera_light = vec(-0.374f, -1.0f, light_depth);
	else if (preset == "Southwest")
		camera_light = vec(-1.0f, -1.0f, light_depth);
	else if (preset == "West-Southwest")
		camera_light = vec(-1.0f, -0.374f, light_depth);
	else if (preset == "West")
		camera_light = vec(-1.0f, 0.0f, light_depth);
	else if (preset == "West-Northwest")
		camera_light = vec(-1.0f, 0.374f, light_depth);
	else if (preset == "Northwest")
		camera_light = vec(-1.0f, 1.0f, light_depth);
	else if (preset == "North-Northwest")
		camera_light = vec(-0.374f, 1.0f, light_depth);

	normalize(camera_light);
	xform mv_xf = rot_only( _camera_transform );
	_light_direction = mv_xf * camera_light;

	if(shader.uniformLocation("light_dir_world")>=0)
		shader.setUniform3fv("light_dir_world", _light_direction);
}

void Scene::recordStats(Stats& stats)
{
	stats.beginConstantGroup("Mesh");
	stats.setConstant("Num Vertices", _trimesh->vertices.size());
	stats.setConstant("Num Faces", _trimesh->faces.size());
	stats.endConstantGroup();
}

