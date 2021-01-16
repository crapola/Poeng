#include "font.h"
// local
#include "bitmap.h"
namespace poeng
{
const int kCharW=8;
const int kCharH=8;
const int kAtlasCols=16;
Font::Font()
{
}
Font::~Font()
{
}
void Font::Draw(const char* p_txt, int p_x,int p_y) const
{
	p_x-=kCharW;
	for (auto c:std::string(p_txt))
	{
		DrawGlyph(c,p_x+=kCharW,p_y);
	}
}
void Font::Draw(const char* p_txt, int p_x, int p_y, Uint8 p_r, Uint8 p_g, Uint8 p_b) const
{
	// TODO: Just set the color and leave as it is?
	Uint8 r,g,b;
	if (SDL_GetTextureColorMod(*_texture,&r,&g,&b)!=0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Can't get texture color mod.");
	}
	if (SDL_SetTextureColorMod(*_texture,p_r,p_g,p_b)!=0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Can't set texture color mod.");
	}
	Draw(p_txt,p_x,p_y);
	if (SDL_SetTextureColorMod(*_texture,r,g,b)!=0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Can't set texture color mod.");
	}
}
void Font::TextureSet(Texture* p_tx)
{
	_texture=p_tx;
}
void poeng::Font::DrawGlyph(char p_c, int p_x, int p_y) const
{
	int c_x=((p_c-' ')%kAtlasCols)*kCharW;
	int c_y=((p_c-' ')/kAtlasCols)*kCharH;
	_texture->Draw(p_x,p_y,c_x,c_y,kCharW,kCharH);
}
}
