/*****************************************************************************\

GQTexture.h
Author: Forrester Cole (fcole@cs.princeton.edu)
Copyright (c) 2009 Forrester Cole

Class to manage OpenGL textures. I/O is handled by GQimage.

libgq is distributed under the terms of the GNU General Public License.
See the COPYING file for details.

\*****************************************************************************/

#ifndef _GQ_TEXTURE_H_
#define _GQ_TEXTURE_H_

#include "GQImage.h"
#include "GQHDRloader.h"

#include "GQInclude.h"

#include <QString>

class GQTexture
{
    public:
        GQTexture();
        virtual ~GQTexture();

        virtual bool load( const QString& filename ) = 0;

        virtual bool bind() const = 0;
        virtual void unbind() const = 0;     
        
        virtual unsigned int width() const = 0;
        virtual unsigned int height() const = 0;
        virtual unsigned int depth() const = 0;
        
        int id() const { return _id; }
        virtual int target() const = 0;

        bool isInitialized() const { return _id >= 0; }
        
        void clear();

    protected:
        int _id;
};

class GQTexture2D : public GQTexture
{
    public:
        bool load( const QString& filename );
        bool create( const GQImage& image, int target = GL_TEXTURE_2D );
        bool create( const GQFloatImage& image, int target = GL_TEXTURE_2D );
		bool create(int width, int height, int internal_format, 
					int format, int type, const void *data, int target);
	
        bool bind() const;
        void unbind() const;
        
		void setMipmapping(bool enable) const;
		void setAnisotropicFiltering(bool enable) const;
	
		void generateMipmaps();
        
        unsigned int width() const { return _width; }
        unsigned int height() const { return _height; }
        unsigned int depth() const { return 1; }

        int target() const;

        void setWarpMode(int wrap_mode);
		    
    protected:
        int _target;
        int _width;
        int _height;
};

class GQTexture3D : public GQTexture
{
    public:
        bool load( const QString& filename );
        bool create(const GQImage& image);
        bool create(int width, int height, int depth, int internal_format, 
                    int format, int type, const void *data);
        bool bind() const;
        void unbind() const;
	
		void setMipmapping(bool enable) const;
		void setAnisotropicFiltering(bool enable) const;
	
		void generateMipmaps();
        
        unsigned int width() const { return _width; }
        unsigned int height() const { return _height; }
        unsigned int depth() const { return _depth; }

        int target() const { return GL_TEXTURE_3D; }

    protected:
        bool load3dt( const QString& filename );
        bool genTexture(int internal_format, int format, int type, 
						const void *data);
    
    protected:
        int _width;
        int _height;
        int _depth;
};

class GQCubeMap : public GQTexture
{
	public:
		GQCubeMap();
		~GQCubeMap();

		bool load( const QString& filename );

        bool bind() const;
        void unbind() const;

		unsigned int width()  const { return _res.width; }
        unsigned int height() const { return _res.height; }
		unsigned int depth()  const { return 1; };
	
		int target() const { return GL_TEXTURE_CUBE_MAP_ARB; };

	protected:
		bool loadhdr( const QString& filename );
		void	createCubeMap( GLenum format, GQImage& img );
		void	createCubeMapHDR( GLenum format );
		float*  getPixel(int x, int y);

	protected:
        GQHDRLoaderResult _res;
		GLenum			  _m_target;
		GQImage*		  _img;
};

#endif /*_GQ_TEXTURE_H_*/
