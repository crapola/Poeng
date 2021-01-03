#pragma once
// std
#include <memory>
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
	void Load();
	//>panning in range [0,640] for left ro right.
	void Play(size_t sample,int panning);
private:
	typedef std::unique_ptr<Mix_Chunk,void (*)(Mix_Chunk*)> Mix_Chunk_Ptr;
	std::vector<Mix_Chunk_Ptr> _chunks{};
};
}
