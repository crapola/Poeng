#include "title.h"
#include <iostream>
namespace poeng
{
Title::Title()
{
}
Title::~Title()
{
}
void Title::Enter(ENTER_PARAMS)
{
	_scores.Enter(p_game);
}
void Title::Exit()
{
}
void Title::Event(EVENT_PARAMS)
{
	_scores.Event(p_event,p_game);
	if (_scores.Typing()) return;
	switch (p_event.type)
	{
	case SDL_MOUSEBUTTONUP:
	{
		SDL_Event sent;
		SDL_zero(sent);
		sent.type=SCENE_SWITCH;
		sent.user.code=1;
		SDL_PushEvent(&sent);
	}
	break;
	case SDL_KEYUP:
		switch (p_event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		{
			SDL_Event sent;
			SDL_zero(sent);
			sent.type=EXIT;
			sent.user.code=0;
			SDL_PushEvent(&sent);
		}
		break;
		default:
			break;
		}
	default:
		break;
	}
}
void Title::Render(RENDER_PARAMS)
{
	// Background effect.
	Uint8 blue=std::abs(_timer-128);
	SDL_SetRenderDrawColor(p_renderer,0,0,blue,255);
	SDL_RenderClear(p_renderer);
	// Logo.
	p_tex[28].Draw(200,100);
	p_font.Draw("2.0",442,140,0x68,0x68,0x68);
	p_font.Draw("Click to start",320-7*8,400);
	// Scores.
	_scores.Render(p_renderer,p_tex,p_font,p_game);
}
void Title::Update(UP_PARAMS)
{
	_timer++;
	_scores.Update(p_game,p_audio);
}
}