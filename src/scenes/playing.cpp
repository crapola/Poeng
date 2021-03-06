#include "playing.h"
// std
#include <algorithm>
#include <array>
#include <string>
// local
#include "drawing.h"
namespace poeng
{
Playing::Playing()
{
}
Playing::~Playing()
{
}
void Playing::Enter(ENTER_PARAMS)
{
	p_game.Start();
	_vfx.clear();
	// Capture pointer inside window.
	SDL_SetRelativeMouseMode(SDL_TRUE);
	_mouse_x=_mouse_x_prev=0;
}
void Playing::Exit(EXIT_PARAMS)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}
void Playing::Event(EVENT_PARAMS)
{
	switch (p_event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (p_event.button.button && SDL_BUTTON_LEFT)
		{
			_mouse_button=true;
			if (_paused)
			{
				_paused=false;
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (p_event.button.button && SDL_BUTTON_LEFT)
		{
			_mouse_button=false;
		}
		break;
	case SDL_MOUSEMOTION:
		_mouse_x=p_mx;
		_mouse_y=p_my;
		break;
	case SDL_KEYUP:
		switch (p_event.key.keysym.sym)
		{
		case SDLK_l:
			if (!(p_event.key.keysym.mod&KMOD_CTRL)) return;
			SDL_LogInfo(0,"Cheat laser.");
			p_game.CheatPower(BrickTypes::POWER_LASER);
			break;
		case SDLK_n:
			if (!(p_event.key.keysym.mod&KMOD_CTRL)) return;
			SDL_LogInfo(0,"Cheat next level.");
			p_game.LevelSet(p_game.LevelGet()+1);
			break;
		case SDLK_b:
			if (!(p_event.key.keysym.mod&KMOD_CTRL)) return;
			SDL_LogInfo(0,"Cheat previous level.");
			p_game.LevelSet(p_game.LevelGet()-1);
			break;
		case SDLK_s:
			if (!(p_event.key.keysym.mod&KMOD_CTRL)) return;
			SDL_LogInfo(0,"Cheat spawn ball.");
			p_game.CheatBallSpawn();
			break;
		case SDLK_p:
			_paused=!_paused;
			SDL_SetRelativeMouseMode(_paused?SDL_FALSE:SDL_TRUE);
			break;
		case SDLK_ESCAPE:
		{
			if (_paused)
			{
				SDL_Event sent;
				SDL_zero(sent);
				sent.type=SCENE_SWITCH;
				sent.user.code=0;
				SDL_PushEvent(&sent);
			}
			_paused=!_paused;
			SDL_SetRelativeMouseMode(_paused?SDL_FALSE:SDL_TRUE);
		}
		break;
		default:
			_win=false;
			// Unpause.
			if (_paused)
			{
				_paused=false;
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
			break;
		}
		break;
	default:
		break;
	}
}
void Playing::Render(RENDER_PARAMS)
{
	// Clear.
	SDL_SetRenderDrawColor(p_renderer,0,0,0,255);
	SDL_RenderClear(p_renderer);
	// Score.
	std::string str_score("SCORE "+std::to_string(p_game.Score()));
	p_font.Draw(str_score.c_str(),16,8);
	// Push bar.
	{
		p_font.Draw("PUSH",16,450,0xe0,0xe0,0xe0);
		SDL_SetRenderDrawColor(p_renderer,255,0,0,255);
		SDL_Rect r{52,450,p_game.PlayerPush(),8};
		SDL_RenderFillRect(p_renderer,&r);
	}
	// Lives.
	for (int i=0; i<p_game.Lives(); ++i)
	{
		p_tex[27].Draw(16+i*5,18);
	}
	DrawBackground(p_lerp,p_renderer,p_tex,p_font,p_game,_rng,_timer);
	// Player.
	{
		size_t tx=4;
		size_t pw=p_game.PlayerPower();
		if (pw!=0)
		{
			tx=pw-6;
		}
		p_tex[tx].Draw(8,p_game.PlayerPos());
	}
	// Laser.
	{
		auto o=p_game.Laser();
		if (o.glued)
		{
			p_tex[8].Draw(o.x,o.y);
		}
	}
	// Balls.
	auto balls=p_game.Balls();
	auto draw_ball=[&](const Object& o)
	{
		float lerp=(o.glued||_paused)?0.0f:p_lerp;
		p_tex[1].Draw(o.x+o.vx*lerp,o.y+o.vy*lerp);
	};
	std::for_each(balls.begin(),balls.end(),draw_ball);
	auto lvl=p_game.LevelCurrent();
	// Wall.
	if (lvl.wall_strength>0)
	{
		// Draw strength.
		const int text_x=640-16-(sizeof("WALL")-1)*8;
		const int space=4;
		p_font.Draw("WALL",text_x,450,0xe0,0xe0,0xe0);
		SDL_SetRenderDrawColor(p_renderer,255,0,0,255);
		SDL_Rect r{text_x-space,450,-lvl.wall_strength,8};
		SDL_RenderFillRect(p_renderer,&r);
		// Draw wall and wiggle.
		const float d=(624-_wall_x)*1.2f;
		_wall_x+=(624-_wall_x)/10.f+d;
		for (int y=0; y<11*kBrickH; y+=kBrickH)
			p_tex[29].Draw(_wall_x,64+y);
	}
	// Pause.
	if (_paused)
	{
		p_font.Draw("** PAUSED **",500,450+8*2,0xC2,0,0);
		// Box.
		SDL_Rect r_black{205,220,430-205,258-220};
		SDL_Rect r_border{206,221,429-206,257-221};
		SDL_SetRenderDrawColor(p_renderer,0,0,0,128);
		SDL_RenderFillRect(p_renderer,&r_black);
		SDL_SetRenderDrawColor(p_renderer,255,255,255,255);
		SDL_RenderDrawRect(p_renderer,&r_border);
		p_font.Draw("Press [ESC] again to exit,",320-26*4,230,0xe0,0xe0,0xe0);
		p_font.Draw("any other key to continue.",320-26*4,240,0xe0,0xe0,0xe0);
	}
	// Winnar.
	if (_win)
	{
		SDL_Rect r_black{180,190,458-180,288-190};
		SDL_Rect r_border{181,191,457-181,287-191};
		SDL_SetRenderDrawColor(p_renderer,0,0,0,128);
		SDL_RenderFillRect(p_renderer,&r_black);
		SDL_SetRenderDrawColor(p_renderer,255,255,255,255);
		SDL_RenderDrawRect(p_renderer,&r_border);
		p_font.Draw("Congraturations!",320-15*4,200,0xff,0,0);
		p_font.Draw("You broke all the bricks.",320-25*4,230,255,255,255);
		p_font.Draw("Now do it again!",320-16*4,240,255,255,255);
		p_font.Draw("Press any key to continue...",320-28*4,270,0xc8,0xc8,0xc8);
	}
	// Effects.
	SDL_SetRenderDrawColor(p_renderer,255,255,255,255);
	if (!_vfx.empty())
	{
		for (auto& e:_vfx)
		{
			SDL_Rect r{e.x,e.y,kBrickW,kBrickH};
			switch (e.type)
			{
			case Effect::BLINK:
				SDL_RenderFillRect(p_renderer,&r);
				break;
			case Effect::METAL:
				p_tex[17].Draw(r.x,r.y, (11-(e.life/2))*16,0,kBrickW,kBrickH);
				break;
			case Effect::BOOM:
			{
				int animpic=18+7-(e.life/2);
				p_tex[animpic].Draw(e.x,e.y);
			}
			break;
			}
		}
	}
}
void Playing::Update(UP_PARAMS)
{
	if (_paused || _win) return;
	++_timer;
	p_game.PlayerMove(_mouse_x-_mouse_x_prev,_mouse_y);
	_mouse_x_prev=_mouse_x;
	if (_mouse_button) p_game.Fire();
	p_game.Tick();
	GameEvent ge;
	while ((ge=p_game.PollEvent()))
	{
		switch (ge.event)
		{
		case GameEvent::Event::HIT_BRICK:
		{
			int sound=6;
			if (ge.value!=BrickTypes::SOLID)
			{
				if (ge.value==BrickTypes::METAL2)
				{
					_vfx.push_back({ge.x*kBrickW, ge.y*kBrickH+kMapCoordY,6,Effect::METAL});
					sound=5;
				}
				else
				{
					_vfx.push_back({ge.x*kBrickW, ge.y*kBrickH+kMapCoordY,3,Effect::BLINK});
					sound=4;
				}
			}
			p_audio.Play(sound,ge.x*kBrickW);
		}
		break;
		case GameEvent::Event::HIT_WALL:
		{
			_wall_x=600;
			// Play hit or break sound.
			p_audio.Play(ge.value>0?11:10,600);
		}
		break;
		case GameEvent::Event::HIT_BAT:
			p_audio.Play(p_game.PlayerPower()==BrickTypes::POWER_GLUE?17:1,16);
			break;
		case GameEvent::Event::HIT_BORDERS:
			p_audio.Play(2,ge.x);
			break;
		case GameEvent::Event::COLLECT_POWER:
		{
			// Play sound.
			// 11 > 7 broad
			// 12 > 14 glue
			// 13 > 9 laser
			// 14,15 > 13 extra balls
			// 16 > 3 boom
			// 17 > 15 gravity
			std::array<int,7> sound_lut({7,14,9,13,13,3,15});
			const int sound=sound_lut.at(ge.value-11);
			const int pan=(sound==7||sound==9||sound==14)?16:ge.x;
			p_audio.Play(sound,pan);
			// Explosion.
			if (ge.value==16)
			{
				const int kExplosionPixelSize=64;
				const int ex=ge.x-kExplosionPixelSize/2+kBrickW/2;
				const int ey=ge.y-kExplosionPixelSize/2+kBrickH/2;
				_vfx.push_back({ex,ey,14,Effect::BOOM});
			}
		}
		break;
		case GameEvent::Event::LASER_SHOOT:
			p_audio.Play(8,16);
			break;
		case GameEvent::Event::LOSE:
			p_audio.Play(10,16);
			_mouse_button=false;
			break;
		case GameEvent::Event::GAME_OVER:
		{
			SDL_Event sent;
			// Go to title.
			SDL_zero(sent);
			sent.type=SCENE_SWITCH;
			sent.user.code=0;
			SDL_PushEvent(&sent);
		}
		break;
		case GameEvent::Event::WIN:
			_win=true;
			break;
		case GameEvent::Event::LOSE_ONE_BALL:
			p_audio.Play(12,0);
			break;
		case GameEvent::Event::DESTROY_POWER:
			p_audio.Play(16,ge.x);
			break;
		case GameEvent::Event::RESBRICKS:
			p_audio.Play(18,ge.x*kBrickW);
		default:
			break;
		}
	}
	// Effects logic.
	if (!_vfx.empty())
	{
		for (auto& e:_vfx)
		{
			e.life-=1;
		}
		_vfx.erase(std::remove_if(_vfx.begin(),_vfx.end(),[](Effect& e)
		{
			return e.life<0;
		}),_vfx.end());
	}
}
}