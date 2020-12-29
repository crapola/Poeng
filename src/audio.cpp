#include "audio.h"
// std
#include <array>
namespace poeng
{
constexpr const char* kSoundFiles[]=
{
	"data/sounds/1up.wav",
	"data/sounds/bat.wav",
	"data/sounds/border.wav",
	"data/sounds/boum.wav",
	"data/sounds/brick1.wav",
	"data/sounds/brick2.wav", // 5
	"data/sounds/brickhard.wav",
	"data/sounds/broad.wav",
	"data/sounds/lazerbeam.wav",
	"data/sounds/lazerbonus.wav",
	"data/sounds/wallbreak.wav", // 10
	"data/sounds/wallhit.wav",
};
Audio::Audio()
{
	if (SDL_Init(SDL_INIT_AUDIO)==-1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"SDL_Init: %s\n", SDL_GetError());
	}
	// Open 44.1KHz, signed 16bit, system byte order, stereo audio, using 1024 byte chunks.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
	{
		SDL_LogError(0,"Mix_OpenAudio: %s\n", Mix_GetError());
	}
	SDL_LogInfo(0,"Audio initalized.");
}
Audio::~Audio()
{
	for (Mix_Chunk* c:_chunks)
	{
		Mix_FreeChunk(c);
	}
	Mix_CloseAudio();
}
bool Audio::Load()
{
	for (auto& s:kSoundFiles)
	{
		Mix_Chunk *sample;
		sample=Mix_LoadWAV(s);
		if (!sample)
		{
			SDL_LogError(0,"Failed to load %s. Mix_LoadWAV: %s\n",s,Mix_GetError());
			continue;
		}
		_chunks.push_back(sample);
	}
	return true;
}
void Audio::Play(size_t p_sample,int p_pan)
{
	static int chan=0;
	int pos=((p_pan*180)/640 + 360-90)%360;
	chan=(chan+1)%8;
	Mix_SetPosition(chan,pos,0);
	Mix_PlayChannel(chan,_chunks.at(p_sample),0);
}
}