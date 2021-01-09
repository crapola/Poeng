#pragma once
#include "scene.h"
// std
#include <random>
#include <vector>
namespace poeng
{
class Editor : public Scene
{
public:
	Editor();
	virtual ~Editor();
	virtual void Enter(ENTER_PARAMS) override;
	virtual void Event(EVENT_PARAMS) override;
	virtual void Exit() override;
	virtual void Render(RENDER_PARAMS) override;
	virtual void Update(UP_PARAMS) override;
private:
	std::minstd_rand _rng{};
	int _mouse_x{},_mouse_y{};
	int _mouse_button{};
	BrickTypes _brick{BrickTypes::NONE};
};
}