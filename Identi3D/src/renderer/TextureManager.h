//
// File: TextureManager.h
// ======================
// Manage textures.
//

#ifndef IDENTI3D_SRC_RENDERER_TEXTUREMANAGER_H
#define IDENTI3D_SRC_RENDERER_TEXTUREMANAGER_H

#include <src/identi3d/General.h>

#include "Texture.h"

namespace Identi3D
{

	class TextureManager
	{
	public:
		TextureManager(void) {} ;
		virtual ~TextureManager(void) {} ;

		/*
		 * Create texture from a resource.
		 */
		Texture *createTexture(const Resource &res) = 0;
		
		/*
		 * Create texture from a file in memory.
		 */
		Texture *createTexture(const void *ptr) = 0;
		
		/*
		 * Release specified texture.
		 */
		void releaseTexture(Texture &tex) = 0;
		
		// TODO: Add declarations.
	
	};

}

#endif // IDENTI3D_SRC_RENDERER_TEXTUREMANAGER_H