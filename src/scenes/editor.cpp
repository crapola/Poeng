#include "editor.h"
// local
#include "drawing.h"
namespace poeng
{
Editor::Editor()
{}
Editor::~Editor()
{}
void Editor::Enter(ENTER_PARAMS)
{
	p_game.LevelsShuffleOnStart(false);
	p_game.Start();
}
void Editor::Exit(EXIT_PARAMS)
{
	p_game.LevelsValidate();
#ifdef NDEBUG
	p_game.LevelsSave("userlevels.bin");
#endif
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
		_mouse_y=std::min(kMapH*kBrickH+kMapCoordY-1,std::max(kMapCoordY,p_my));
		break;
	case SDL_MOUSEWHEEL:
	{
		// Assume p_event.wheel.y is 1 or -1.
		_selection=(kSelectionMax+_selection+p_event.wheel.y)%kSelectionMax;
	}
	break;
	default:
		break;
	}
}
void Editor::Render(RENDER_PARAMS)
{
	static uint8_t timer{};
	timer=(timer+1)%3;
	// Clear.
	SDL_SetRenderDrawColor(p_renderer,2,4,8,255);
	SDL_RenderClear(p_renderer);
	// Title.
	p_font.Draw("Editor",16,8);
	// Level.
	DrawBackground(p_renderer,p_tex,p_font,p_game,_rng);
	// Draw ghost.
	if (timer)
	{
		const int x=PixelToGridX(_mouse_x);
		const int y=PixelToGridY(_mouse_y);
		const Cell b=SelectionAsBrick();
		DrawBrick(p_tex,b,x,y,timer);
	}
	// Draw toolbar.
	{
		const int tool_x{640-23*kBrickW-1},tool_y{1};
		// bricks.bmp
		p_tex[17].Draw(tool_x,tool_y,0,0,kBrickW*9,kBrickH);
		// Power-ups.
		for (size_t p=9; p<15; ++p)
		{
			p_tex[p].Draw(tool_x+kBrickW*(9+(p-9)),tool_y);
		}
		p_tex[30].Draw(tool_x+kBrickW*(9+(15-9)),tool_y);
		// Respawning bricks.
		p_tex[31].Draw(tool_x+kBrickW*(9+(16-9)),tool_y,0,0,7*kBrickW,kBrickH);
		// Selection outline.
		SDL_Rect r{tool_x+_selection*kBrickW,tool_y-1,kBrickW+1,kBrickH+1};
		SDL_RenderDrawRect(p_renderer,&r);
	}
}
void Editor::Update(UP_PARAMS)
{
	if (_mouse_button)
	{
		p_game.EditBrick(_mouse_x,_mouse_y,BrickTypes(_mouse_button==1?SelectionAsBrick():0));
	}
}
Cell Editor::SelectionAsBrick()
{
	return _selection+1+(_selection>8)+(_selection==BrickTypes::METAL1-1);
}
}