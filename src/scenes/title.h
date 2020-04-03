#pragma once
#include "scene.h"
// local
#include "scores.h"
namespace poeng
{
class Title : public Scene
{
public:
	Title();
	virtual ~Title();
	virtual void Enter(ENTER_PARAMS) override;
	virtual void Event(EVENT_PARAMS) override;
	virtual void Exit() override;
	virtual void Render(RENDER_PARAMS) override;
	virtual void Update(UP_PARAMS) override;
private:
	uint8_t _timer{};
	Scores _scores{};
};
}