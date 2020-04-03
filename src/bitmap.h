#pragma once
// std
#include <string>
// libs
#include <SDL.h>
class Bitmap
{
public:
	Bitmap();
	Bitmap(const Bitmap&)=delete;
	void operator=(const Bitmap&)=delete;
	Bitmap(Bitmap&&);
	~Bitmap();
	operator SDL_Surface* () const;
	bool Load(const char* filename);
private:
	SDL_Surface* _surface=NULL;
#ifndef NDEBUG
public:
	std::string _name="NEW"; // Debugging only.
#endif
};
