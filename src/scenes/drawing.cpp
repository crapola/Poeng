#include "drawing.h"
namespace poeng
{
void DrawBackground(RENDER_PARAMS,std::minstd_rand& p_rng)
{
	static uint8_t timer{};
	// Draw background.
	{
		const int kBackgroundTop=64;
		const int kBackgroundRows=11;
		int style=32*(p_game.LevelGet()%4);
		for (int x=0; x<20; ++x)
		{
			p_tex[0].Draw(x*32,kBackgroundTop,32,style,32,32);
			for (int y=32; y<32*kBackgroundRows; y+=32)
			{
				p_tex[0].Draw(x*32,kBackgroundTop+y,0,style,32,32);
			}
		}
	}
	// Draw bricks.
	timer++;
	auto lvl=p_game.LevelCurrent();
	SDL_SetRenderDrawColor(p_renderer,0,0,0,255);
	for (size_t y=0; y<11; ++y)
	{
		for (size_t x=0; x<40; ++x)
		{
			auto b=lvl.at(x,y);
			if (b==0)
				continue;
			// Shadow.
			if (b!=BrickTypes::POWER_BOMB && b<BrickTypes::RES_RED_DOWN)
			{
				const auto px=GridToPixelX(x);
				const auto py=GridToPixelY(y);
				SDL_Rect r{px+8,py+8,kBrickW,kBrickH};
				SDL_RenderFillRect(p_renderer,&r);
			}
			DrawBrick(p_tex,b,x,y,timer);
			/*
			const int px=x*kBrickW;
			const int py=64+y*kBrickH;
			// Shadow.
			if (b!=BrickTypes::POWER_BOMB)
			{
			SDL_Rect r{px+8,py+8,kBrickW,kBrickH};
			SDL_RenderFillRect(p_renderer,&r);
			}
			// Brick or power-up.
			if (b==BrickTypes::METAL2) b=9;
			if (b>=BrickTypes::POWER_SIZE)
			{
			size_t tex=9+b-BrickTypes::POWER_SIZE;
			if (b==BrickTypes::POWER_BOMB) tex+=(timer%4)/2;
			if (b==BrickTypes::POWER_GRAVITY) tex=30;
			p_tex[tex].Draw(px,py);
			continue;
			}
			b=b-1;
			p_tex[17].Draw(px,py, b*kBrickW,0, kBrickW,kBrickH);
			*/
		}
	}
	SDL_SetRenderDrawColor(p_renderer,255,255,255,255);
	// Draw borders.
	const int kBorderTop=32;
	const int kBorderBottom=416;
	int seed=p_game.LevelGet();
	p_rng.seed(seed+1);
	for (int x=0; x<20; ++x)
	{
		int r=p_rng()%3;
		p_tex[16].Draw(x*32,kBorderTop,r*32,0,32,32);
		p_tex[16].Draw(x*32,kBorderBottom,r*32,32,32,32);
	}
	// Stage number.
	std::string str_stage("STAGE "+std::to_string(p_game.LevelGet()+1));
	p_font.Draw(str_stage.c_str(),300,450);
}
void DrawBrick(const std::vector<Texture>& p_tex,Cell p_brick,int p_x,int p_y,uint8_t p_timer)
{
	const int x=GridToPixelX(p_x);
	const int y=GridToPixelY(p_y);
	if (p_brick>=BrickTypes::POWER_SIZE)
	{
		if (p_brick>=BrickTypes::RES_RED)
		{
			if (p_brick>=BrickTypes::RES_RED_DOWN)
			{
				p_tex[31].Draw(x,y,(p_brick-BrickTypes::RES_RED_DOWN)*kBrickW,kBrickH,kBrickW,kBrickH);
			}
			else
			{
				p_tex[31].Draw(x,y,(p_brick-BrickTypes::RES_RED)*kBrickW,0,kBrickW,kBrickH);
			}
		}
		else
		{
			// Power-up.
			size_t tex=9+p_brick-BrickTypes::POWER_SIZE;
			if (p_brick==BrickTypes::POWER_BOMB) tex+=(p_timer%4)/2;
			if (p_brick==BrickTypes::POWER_GRAVITY) tex=30;
			p_tex[tex].Draw(x,y);
		}
	}
	else
	{
		// Brick from bricks.bmp.
		if (p_brick==BrickTypes::METAL2) p_brick=BrickTypes::METAL1;
		p_tex[17].Draw(x,y,(p_brick-1)*kBrickW,0,kBrickW,kBrickH);
	}
}
}
