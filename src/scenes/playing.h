#pragma once
#include "scene.h"
// std
#include <random>
#include <vector>
namespace poeng
{
class Playing : public Scene
{
public:
	Playing();
	virtual ~Playing();
	virtual void Enter(ENTER_PARAMS) override;
	virtual void Event(EVENT_PARAMS) override;
	virtual void Exit(EXIT_PARAMS) override;
	virtual void Render(RENDER_PARAMS) override;
	virtual void Update(UP_PARAMS) override;
private:
	std::minstd_rand _rng{};
	uint8_t _timer{};
	float _wall_x{624};
	struct Effect
	{
		int x{},y{},life{2};
		enum
		{
			BLINK=0,
			METAL,
			BOOM
		} type;
	};
	std::vector<Effect> _vfx{};
	//uint8_t _timer{};
	int _mouse_x{},_mouse_y{240};
	int _mouse_x_prev{}; // Previous frame mouse x.
	bool _mouse_button{false};
	bool _paused{false};
	bool _win{false};
};
}