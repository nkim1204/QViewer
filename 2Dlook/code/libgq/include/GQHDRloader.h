
/***********************************************************************************
	Created:	17:9:2002
	FileName: 	GQHDRLoader.h
	Author:		Igor Kravtchenko
	
	Info:		Load HDR image and convert to a set of float32 RGB triplet.
************************************************************************************/

#ifndef GQHDRLOADERRESULT_H
#define GQHDRLOADERRESULT_H

#include "GQInclude.h"

class GQHDRLoaderResult
{
public:
	int width, height;
	// each pixel takes 3 float32, each component can be of any value...
	float *cols;
};

#endif
#ifndef GQHDRLOADER_H
#define GQHDRLOADER_H
class GQHDRLoader
{
public:
	static bool load(const QString& fileName, GQHDRLoaderResult &res);
};

#endif
