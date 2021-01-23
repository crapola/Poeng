#pragma once
// std
#include <random>
// local
#include "scene.h"
// Drawing functions common to Playing and Editor.
namespace poeng
{
void DrawBackground(RENDER_PARAMS,std::minstd_rand&,uint8_t timer);
void DrawBrick(const std::vector<Texture>& p_tex,Cell brick,int x_grid,int y_grid,uint8_t timer);
}