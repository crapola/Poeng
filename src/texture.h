#pragma once
// libs
#include <SDL.h>
class Texture
{
public:
	Texture(SDL_Renderer*,SDL_Surface*);
	~Texture();
	Texture(const Texture&)=delete;
	void operator=(const Texture&)=delete;
	Texture(Texture&&);
	operator SDL_Texture* ();
	void Draw(int x,int y) const;
	void Draw(int x,int y,int src_x,int src_y,int src_w,int src_h) const;
private:
	// A texture is associated to a renderer.
	SDL_Texture* _texture=NULL;
	SDL_Renderer* _renderer=NULL;
	int _width=0,_height=0;
};
