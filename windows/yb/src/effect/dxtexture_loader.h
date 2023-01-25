#ifndef __DXTEXTURE_LOADER_H__
#define __DXTEXTURE_LOADER_H__

#include "effect.h"
#include <string>

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>

namespace nn {
	namespace yb {

		//-----------------------------------------------------------------------------

		namespace main {

			class CDxTextureLoader : public ::Effekseer::TextureLoader {
			private:
				::EffekseerRenderer::Renderer*	m_renderer;

			public:
				CDxTextureLoader( ::EffekseerRenderer::Renderer* renderer );
				virtual ~CDxTextureLoader();
				void* Load( const EFK_CHAR* path );
				void Unload( void* data );

				// ì«Ç›çûÇ›éûÇÃÉãÅ[Ég
				std::wstring RootPath;
			};

			//-----------------------------------------------------------------------------
		} //namespace main

	} //namespece yb
} //namespace nn

#endif __DXTEXTURE_LOADER_H__