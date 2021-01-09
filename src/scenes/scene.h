#pragma once
// std
#include <vector>
// libs
#include <SDL.h>
// local
#include "../audio.h"
#include "../font.h"
#include "../game.h"
#include "../texture.h"
namespace poeng
{
// Give user events SDL_USEREVENT+n a more readable name.
enum MySdlEvents
{
	SCENE_SWITCH=SDL_USEREVENT,
	EXIT
};
void SceneChange(int scene_id);
#define ENTER_PARAMS Game& p_game
#define EVENT_PARAMS const SDL_Event& p_event,Game& p_game,int p_mx,int p_my
#define RENDER_PARAMS SDL_Renderer* p_renderer,const std::vector<Texture>& p_tex,const Font& p_font,const Game& p_game
#define UP_PARAMS Game& p_game,Audio& p_audio
class Scene
{
public:
	virtual ~Scene()=0;
	virtual void Enter(ENTER_PARAMS)=0;
	virtual void Event(EVENT_PARAMS)=0;
	virtual void Exit()=0;
	virtual void Render(RENDER_PARAMS)=0;
	virtual void Update(UP_PARAMS)=0;
protected:
};
}