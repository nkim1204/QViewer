/*****************************************************************************\

Scene.h
Author: Forrester Cole (fcole@cs.princeton.edu)
Copyright (c) 2009 Forrester Cole

qviewer is distributed under the terms of the GNU General Public License.
See the COPYING file for details.

\*****************************************************************************/

#ifndef SCENE_H_
#define SCENE_H_

#include <QDomElement>
#include <QDir>

#include "GQVertexBufferSet.h"
#include "GQShaderManager.h"
#include "Stats.h"
#include "XForm.h"

#include "Sphere.h"
#include "Cube.h"
#include "Quad.h"

#include "DialsAndKnobs.h"

enum ModelType
{
	CUBE,
	SPHERE,
    MESH,
	NUM_MODELS
};
const QStringList modelChoices = QStringList() << "cube" << "sphere" << "lemming";

class GLViewer;
class TriMesh;
class dkFloat;
class dkEnum;

class Scene {
public:
	Scene();
	~Scene();

	void init();
	void clear();

	bool load( const QString& filename );
	bool load( const QDomElement& root, const QDir& path );
	bool save( const QString& filename, const GLViewer* viewer,
		const DialsAndKnobs* dials_and_knobs );
	bool save( QDomDocument& doc, QDomElement& root, const QDir& path );

	void loadEnvMap( const QString &filename );

	void recordStats(Stats& stats);

	void drawScene(const QString &programName, const ModelType type); 

	void boundingSphere(vec& center, float& radius, const ModelType type);

	void setCameraTransform( const xform& xf ) { _camera_transform = xf; }
	void setCameraPosition( const vec4f pos ) { _camera_position = pos; }
	void setModelViewMatix( const xform& xf )  { _modelView_matrix = xf; }
	void setProjectionMatrix( const xform& xf ){ _projection_matrix = xf; }

	const TriMesh* trimesh() const { return _trimesh; }
	const QDomElement& viewerState() { return _viewer_state; }
	const QDomElement& dialsAndKnobsState() { return _dials_and_knobs_state; }

	static QString fileExtension() { return QString("qvs"); }

protected:
	void setupMesh();
	void setupVertexBufferSet();

	void setupTextures(GQShaderRef& shader);
	void extractSilhouettes();
	void toneMap();
    void warp();

	void setupLighting(GQShaderRef& shader);
	void drawMesh(GQShaderRef& shader);

	void drawEnvMap();

protected:
	TriMesh*            _trimesh;
	GQVertexBufferSet   _vertex_buffer_set;
	QVector<int>		_tristrips;
	QString             _trimesh_filename;
	Sphere*				_sphere;
	Sphere*				_sphereBackground;
	Cube*				_cube;
	Quad*				_quad;
	Quad*				_ground;
	GQTexture*			_tex_earth_day;
	GQTexture*			_tex_earth_night;
	GQTexture*			_tex_earth_bump;
	GQTexture*			_tex_earth_normals;
    GQTexture2D*          _tex_gray_scale;

	GQCubeMap*			_hdrTexReflection;

	GQFramebufferObject _fbo;
    GQFramebufferObject _warpFbo;
    GQFramebufferObject _finalFbo;


	xform               _camera_transform;
	xform               _modelView_matrix;
	xform               _projection_matrix;
	vec                 _light_direction;
	vec4f               _camera_position;

	QDomElement         _viewer_state;
	QDomElement         _dials_and_knobs_state;

	int					_shadowTextureSize;
};

#endif // SCENE_H_
