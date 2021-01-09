#include "editor.h"
// local
#include "drawbackground.h"
namespace poeng
{
Editor::Editor()
{}
Editor::~Editor()
{}
void Editor::Enter(ENTER_PARAMS)
{
	p_game.Start();
}
void Editor::Exit(EXIT_PARAMS)
{
	p_game.LevelsSave("userlevels.bin");
}
void Editor::Event(EVENT_PARAMS)
{
	switch (p_event.type)
	{
	case SDL_KEYDOWN:
		switch (p_event.key.keysym.sym)
		{
		case SDLK_LEFT:
			p_game.LevelSet(p_game.LevelGet()-1);
			break;
		case SDLK_RIGHT:
			p_game.LevelSet(p_game.LevelGet()+1);
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (p_event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		{
			SDL_Event sent;
			SDL_zero(sent);
			sent.type=SCENE_SWITCH;
			sent.user.code=0;
			SDL_PushEvent(&sent);
		}
		break;
		default:
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		_mouse_button=p_event.button.button;
		break;
	case SDL_MOUSEBUTTONUP:
		_mouse_button=0;
		break;
	case SDL_MOUSEMOTION:
		_mouse_x=p_mx;
		_mouse_y=p_my;
		break;
	case SDL_MOUSEWHEEL:
	{
		// Assume p_event.wheel.y is 1 or -1.
		const int next=BrickTypes::END+static_cast<int>(_brick)+p_event.wheel.y;
		_brick=BrickTypes(next%BrickTypes::END);
	}
	break;
	default:
		break;
	}
}
void Editor::Render(RENDER_PARAMS)
{
	SDL_SetRenderDrawColor(p_renderer,0,8,16,255);
	SDL_RenderClear(p_renderer);
	DrawBackground(p_renderer,p_tex,p_font,p_game,_rng);
}
void Editor::Update(UP_PARAMS)
{
	if (_mouse_button)
	{
		p_game.EditBrick(_mouse_x,_mouse_y,_mouse_button==1?_brick:BrickTypes::NONE);
	}
}
}