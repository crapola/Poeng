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
	"data/sounds/new/lostball.wav",
	"data/sounds/new/multiball.wav",
	"data/sounds/new/powerglue.wav",
	"data/sounds/new/spring.wav", // 15
	"data/sounds/new/destroy.wav",
	"data/sounds/new/glu.wav",
	"data/sounds/new/res.wav"
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
	Mix_CloseAudio();
}
void Audio::Load()
{
	for (auto& s:kSoundFiles)
	{
		Mix_Chunk *sample;
		sample=Mix_LoadWAV(s);
		if (!sample)
		{
			std::string msg("Failed to load ");
			msg+=s;
			msg+=". Mix_LoadWAV: ";
			msg+=Mix_GetError();
			msg+='\n';
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,msg.c_str());
			throw std::runtime_error(msg);
			continue;
		}
		_chunks.push_back(Mix_Chunk_Ptr(sample,Mix_FreeChunk));
	}
}
void Audio::Play(size_t p_sample,int p_pan)
{
	int pos=((p_pan*180)/640 + 360-90)%360;
	int chan=Mix_PlayChannel(-1,_chunks.at(p_sample).get(),0);
	Mix_SetPosition(chan,pos,0);
}
}