#include "bitmap.h"
Bitmap::Bitmap()
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"Bitmap created.");
}
Bitmap::Bitmap(Bitmap&& other)
{
	_surface=other._surface;
	other._surface=NULL;
#ifndef NDEBUG
	_name=other._name;
	other._name="MOVED";
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,"Bitmap moved: %s->%s.",other._name.c_str(),_name.c_str());
#endif
}
Bitmap::~Bitmap()
{
#ifndef NDEBUG
	if (_surface==NULL)
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"Freeing null surface of %s.",_name.c_str());
#endif
	SDL_FreeSurface(_surface);
}
Bitmap::operator SDL_Surface* () const
{
	return _surface;
}
bool Bitmap::Load(const char* filename)
{
	_surface=SDL_LoadBMP(filename);
	if (_surface==NULL)
	{
		return false;
	}
	SDL_SetColorKey(_surface,SDL_TRUE,0);
#ifndef NDEBUG
	_name=filename;
#endif
	return true;
}
