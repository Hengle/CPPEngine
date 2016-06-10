#include <spine/spine-sfml.h>
#include <spine/extension.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
void _spAtlasPage_createTexture (spAtlasPage* self, const char* path);
#ifdef __cplusplus
}
#endif

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
	//_spAtlasPage_createTexture` Loads a texture and stores it and its size in the void* rendererObject

	TextureID id=-1;
	#if 0
	id=IRenderer::GetRendererInstance()->addImageLibTexture(
		path
		,false,
		//IRenderer::GetRendererInstance()->GetbilinearClamp());
		IRenderer::GetRendererInstance()->Getlinear());
	#else
	Image3 img;
	img.loadImageLibImage(path, false);
	img.flipY();
	id=IRenderer::GetRendererInstance()->addTexture(&img);
	#endif
	//printf("%s:\n%s:%d\n", __FILE__,__FUNCTION__, __LINE__);
	//printf("_spAtlasPage_createTexture: %x %s\n", id, path);

	self->width=IRenderer::GetRendererInstance()->GetTextureWidth (id);
	self->height=IRenderer::GetRendererInstance()->GetTextureHeight(id);

	//printf("self->width =%d\n", self->width);
	//printf("self->height=%d\n", self->height);

	spine::spineTexture* tex=new spine::spineTexture();
	tex->id=id;
	tex->width=self->width;
	tex->height=self->height;
	self->rendererObject=tex;
}

