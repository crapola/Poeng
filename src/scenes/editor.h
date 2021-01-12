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
	virtual void Exit(EXIT_PARAMS) override;
	virtual void Render(RENDER_PARAMS) override;
	virtual void Update(UP_PARAMS) override;
private:
	Cell SelectionAsBrick();
	std::minstd_rand _rng{};
	int _mouse_x{320},_mouse_y{240};
	int _mouse_button{};
	const int kSelectionMax=BrickTypes::END-2-7;
	int _selection{};
};
}