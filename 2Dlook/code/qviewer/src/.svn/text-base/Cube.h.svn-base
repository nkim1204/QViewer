#ifndef _CUBE_H
#define _CUBE_H

#include "GQShaderManager.h"

class Cube {
public:
	Cube();

	void draw (GQShaderRef& shader);

	float radius() const { return _radius; }

protected:
	void createVAO(GQShaderRef& shader);

private:
	bool   _initVAO;
        int _cubeBuffers[5];
        int _vertexArrayObject;
        int _vertexPositionIndex;
        int _vertexColorIndex;
        int _vertexNormalIndex;
        int _vertexTexcoordIndex;

	float _radius;
};

#endif
