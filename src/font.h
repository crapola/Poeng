#pragma once
// libs
#include <SDL.h>
// local
#include "texture.h"
namespace poeng
{
class Font
{
public:
	Font();
	~Font();
	Font(Font&&) = delete;
	Font(const Font&) = delete;
	Font& operator=(Font&&) = delete;
	Font& operator=(const Font&) = delete;
	void Draw(const char* txt,int x,int y) const;
	void Draw(const char* txt,int x,int y,Uint8 r,Uint8 g,Uint8 b) const;
	void TextureSet(Texture*);
private:
	void DrawGlyph(char,int x,int y) const;
	Texture* _texture{};
};
}
