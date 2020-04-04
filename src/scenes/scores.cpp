#include "scores.h"
// std
#include <string>
#include <iostream>
namespace poeng
{
Scores::Scores()
{
	if (!_scores.Load())
		_scores.Populate();
}
Scores::~Scores()
{
#ifdef NDEBUG
	_scores.Save();
#endif
}
void Scores::Enter(ENTER_PARAMS)
{
	_entry.clear();
	_score_latest=p_game.Score();
	if (_scores.Qualifies(_score_latest))
	{
		_rank=_scores.Insert({_score_latest,"Player"});
		_entry_mode=true;
	}
}
void Scores::Exit()
{}
void Scores::Event(EVENT_PARAMS)
{
	switch (p_event.type)
	{
	case SDL_TEXTINPUT:
		if (_entry.size()<16)
			_entry+=p_event.text.text;
		break;
	case SDL_KEYDOWN:
		switch (p_event.key.keysym.sym)
		{
		case SDLK_RETURN:
			if (_entry_mode)
			{
				_entry_mode=false;
				_scores.Replace(_rank, {_score_latest,_entry});
			}
			break;
		case SDLK_BACKSPACE:
			if (!_entry.empty())
			{
				_entry.pop_back();
			}
			break;
		default:
			break;
		}
	default:
		break;
	}
}
void Scores::Render(RENDER_PARAMS)
{
	const int x=248;
	const int y=222;
	for (int i=0; i<10; ++i)
	{
		int line=12*i+y;
		HighScores::Entry e=_scores.Get(i);
		// Rank.
		std::string srank(std::to_string(i+1)+'.');
		p_font.Draw(srank.c_str(),x-srank.length()*8,line,255,0,0);
		// Name.
		p_font.Draw(e.second.c_str(),x+82,line,0,0xe1,0);
		// Score.
		std::string sscore(std::to_string(e.first));
		p_font.Draw(sscore.c_str(),x+80-sscore.length()*8,line+1,255,0,0);
		p_font.Draw(sscore.c_str(),x+80-sscore.length()*8,line);
		// Line.
		SDL_SetRenderDrawColor(p_renderer,0,16,64,255);
		SDL_RenderDrawLine(p_renderer,x-2,line+8,x+78,line+8);
		// Caption.
		if (_entry_mode)
		{
			p_font.Draw("ENTER YOUR NAME",x+30,y-20,255,0,0);
		}
		else
		{
			p_font.Draw("** TOP TEN **",x+34,y-20,255,0,0);
		}
	}
	if (_entry_mode)
	{
		SDL_Rect r{x+82,y+_rank*12,16*8,8};
		SDL_SetRenderDrawColor(p_renderer,0,0,64,255);
		SDL_RenderFillRect(p_renderer,&r);
		p_font.Draw(_entry.c_str(),x+82,y +_rank*12,255,255,255);
		if (_timer%16>=8)
			p_font.Draw("<",x+82+_entry.length()*8,y+_rank*12,255,255,255);
	}
}
void Scores::Update(UP_PARAMS)
{
	++_timer;
}
bool Scores::Typing() const
{
	return _entry_mode;
}
}