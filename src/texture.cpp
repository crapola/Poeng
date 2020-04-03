#include "texture.h"
Texture::Texture(SDL_Renderer* p_renderer,SDL_Surface* p_surface)
{
	_texture=SDL_CreateTextureFromSurface(p_renderer,p_surface);
	_width=p_surface->w;
	_height=p_surface->h;
	_renderer=p_renderer;
	if (!_texture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Texture creation failed: %s\n",SDL_GetError());
	}
}
Texture::Texture(Texture&& other)
{
	_texture=other._texture;
	_renderer=other._renderer;
	_width=other._width;
	_height=other._height;
	other._texture=NULL;
	other._renderer=NULL;
	other._width=0;
	other._height=0;
}
Texture::~Texture()
{
	SDL_DestroyTexture(_texture);
}
Texture::operator SDL_Texture* ()
{
	return _texture;
}
void Texture::Draw(int p_x,int p_y) const
{
	SDL_Rect r{p_x,p_y,_width,_height};
	SDL_RenderCopy(_renderer,_texture,NULL,&r);
}
void Texture::Draw(int p_x,int p_y,int p_src_x,int p_src_y,int p_src_w,int p_src_h) const
{
	SDL_Rect src{p_src_x,p_src_y,p_src_w,p_src_h};
	SDL_Rect dst{p_x,p_y,p_src_w,p_src_h};
	SDL_RenderCopy(_renderer,_texture,&src,&dst);
}