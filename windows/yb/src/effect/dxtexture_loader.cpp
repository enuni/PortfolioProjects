
#include "nn_common.h"
#include "disp_common.h"

#include "dxtexture_loader.h"

#include "fs.h"
//---------------------------------------------------------------------------
nn::yb::main::CDxTextureLoader::CDxTextureLoader( ::EffekseerRenderer::Renderer* renderer ) {
	::nn_commonLog( 1, "CDxTextureLoader Create\n");
	this->m_renderer = renderer; 

}

//---------------------------------------------------------------------------
nn::yb::main::CDxTextureLoader::~CDxTextureLoader() {
	::nn_commonLog( 1, "CDxTextureLoader Delete\n");

}

//---------------------------------------------------------------------------
void* nn::yb::main::CDxTextureLoader::Load( const EFK_CHAR* path ) {

	std::wstring dstPath = std::wstring();
	dstPath.append(this->RootPath);
	dstPath.append(std::wstring((const wchar_t *)path));

	IDirect3DTexture9* texture = NULL;

	char *mbs = new char[dstPath.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, dstPath.c_str(), dstPath.length() * MB_CUR_MAX + 1); 

	// ファイルのサイズを得る
	int FileSize = fsGetFileSize( mbs ) ;

	if (FileSize > 0) 
	{
		// データ格納用のメモリ領域を確保
		void *Buffer = malloc( FileSize ) ;
		// ファイルを丸ごとメモリに読み込む 
		BOOL bRes = fsLoadFile( Buffer, mbs, FileSize );

		if (bRes)
		{
			D3DXCreateTextureFromFileInMemory( this->m_renderer->GetDevice(), Buffer, FileSize, &texture );
		}

		delete [] Buffer; 
		delete [] mbs;

		return (void*)texture;
	} 

	delete [] mbs; 
	return NULL;
}

//---------------------------------------------------------------------------
void nn::yb::main::CDxTextureLoader::Unload( void* data ) {
	if( data != NULL ) {
		IDirect3DTexture9* texture = (IDirect3DTexture9*)data;
		texture->Release();
	}
}

//---------------------------------------------------------------------------