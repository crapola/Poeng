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
void Title::Exit(EXIT_PARAMS)
{
}
void Title::Event(EVENT_PARAMS)
{
	_scores.Event(p_event,p_game,p_mx,p_my);
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
		case SDLK_e:
		{
			SDL_Event sent;
			SDL_zero(sent);
			sent.type=SCENE_SWITCH;
			sent.user.code=2;
			SDL_PushEvent(&sent);
		}
		break;
		case SDLK_s:
			p_game.LevelsShuffleOnStart(!p_game.LevelsShuffleOnStart());
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
	// Edit.
	{
		const int x=320-7*8,y=400+8*4;
		p_font.Draw("[E]",x,y);
		p_font.Draw("    Editor",x,y,0x80,0x80,0x80);
	}
	// Shuffle.
	{
		const bool shuffle=p_game.LevelsShuffleOnStart();
		const int x=320-7*8,y=400+8*2;
		p_font.Draw("[S]",x,y);
		p_font.Draw("    Shuffle",x,y,0x80,0x80,0x80);
		const int r=shuffle?0x00:0xFF;
		const int g=shuffle?0xFF:0x00;
		const int b=0;
		std::array<const char*,2> onoff{"ON","OFF"};
		const char* str=onoff[shuffle?0:1];
		p_font.Draw(str,x+12*8,y,r,g,b);
	}
	// Scores.
	_scores.Render(p_lerp,p_renderer,p_tex,p_font,p_game);
}
void Title::Update(UP_PARAMS)
{
	_timer++;
	_scores.Update(p_game,p_audio);
}
}