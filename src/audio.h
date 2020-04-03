#pragma once
// std
#include <vector>
// libs
#include <SDL.h>
#include <SDL_mixer.h>
namespace poeng
{
class Audio
{
public:
	Audio();
	~Audio();
	Audio(const Audio&)=delete;
	void operator=(Audio&&)=delete;
	void operator=(const Audio&)=delete;
	Audio(Audio&&)=delete;
	bool Load();
	//>panning in range [0,640] for left ro right.
	void Play(int channel,size_t sample,int panning);
private:
	std::vector<Mix_Chunk*> _chunks{};
};
}
