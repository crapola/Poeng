#include "gameview.h"
// std
#include <algorithm>
// local
#include "audio.h"
namespace poeng
{
GameView::GameView()
{
	// Load levels.
	if (!_game.LevelsLoad("userlevels.bin"))
	{
		SDL_Log("User levels not found, loading default.");
		if (!_game.LevelsLoad("data/levels.bin"))
		{
			throw std::runtime_error("Levels file not found.");
		}
	}
	// Register SDL user events.
	Uint32 user_event=SDL_RegisterEvents(5);
	if (user_event==(Uint32)-1)
	{
		const auto err="SDL_RegisterEvents() failed.";
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,err);
		throw std::runtime_error(err);
	}
}
GameView::~GameView()
{
}
void GameView::AudioInit()
{
	_audio.Load();
}
void GameView::CreateTextures(SDL_Renderer* p_renderer)
{
	for (auto& b:_bitmaps)
	{
		_textures.push_back(Texture(p_renderer,b));
	}
}
void GameView::FontInit()
{
	_font.TextureSet(&_textures[26]);
}
bool GameView::Event(const SDL_Event& p_event,int p_mx,int p_my)
{
	switch (p_event.type)
	{
	case MySdlEvents::EXIT:
		return false;
		break;
	case MySdlEvents::SCENE_SWITCH:
		_scenes.at(_scene_current)->Exit(_game);
		_scene_current=p_event.user.code;
		_scenes.at(_scene_current)->Enter(_game);
		break;
	default:
		break;
	}
	_scenes[_scene_current]->Event(p_event,_game,p_mx,p_my);
	return true;
}
void GameView::LoadImages()
{
	// Load bitmaps.
	const std::array arr=
	{
		"data/images/backgrounds.bmp",//0
		"data/images/ball.bmp",
		"data/images/ball2.bmp",
		"data/images/ball3.bmp",
		"data/images/bat.bmp",
		"data/images/bat2.bmp",//5
		"data/images/bat3.bmp",
		"data/images/bat4.bmp",
		"data/images/beam.bmp",
		"data/images/bonus1.bmp",
		"data/images/bonus2.bmp",//10
		"data/images/bonus3.bmp",
		"data/images/bonus4.bmp",
		"data/images/bonus5.bmp",
		"data/images/bonus6.bmp",
		"data/images/bonus6b.bmp",//15
		"data/images/borders.bmp",
		"data/images/bricks.bmp",
		"data/images/explo0.bmp",
		"data/images/explo1.bmp",
		"data/images/explo2.bmp",//20
		"data/images/explo3.bmp",
		"data/images/explo4.bmp",
		"data/images/explo5.bmp",
		"data/images/explo6.bmp",
		"data/images/explo7.bmp",//25
		"data/images/font.bmp",
		"data/images/life.bmp",
		"data/images/logo.bmp",
		"data/images/wall.bmp",
		"data/images/new/bonus_spring.bmp",//30
		"data/images/new/bricksres.bmp"
	};
	auto loader=[](const char* str)->Bitmap
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Loading %s.",str);
		Bitmap bmp;
		if (!bmp.Load(str))
		{
			const auto err=SDL_GetError();
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"SDL_LoadBMP() failed: %s\n",err);
			throw std::runtime_error(std::string("File not found.\n")+std::string(err));
		}
		return bmp;
	};
	std::transform(arr.begin(),arr.end(),std::back_inserter(_bitmaps),loader);
}
void GameView::Render(SDL_Renderer* p_renderer,float p_lerp)
{
	_scenes[_scene_current]->Render(p_lerp,p_renderer,_textures,_font,_game);
}
void GameView::Update()
{
	_scenes[_scene_current]->Update(_game,_audio);
}
}
