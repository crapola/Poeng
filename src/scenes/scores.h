#pragma once
#include "scene.h"
// local
#include "../highscores.h"
namespace poeng
{
class Scores : public Scene
{
public:
	Scores();
	virtual ~Scores();
	virtual void Enter(ENTER_PARAMS) override;
	virtual void Event(EVENT_PARAMS) override;
	virtual void Exit(EXIT_PARAMS) override;
	virtual void Render(RENDER_PARAMS) override;
	virtual void Update(UP_PARAMS) override;
	bool Typing() const;
private:
	HighScores _scores{};
	bool _entry_mode{false};
	std::string _entry{};
	long _score_latest{};
	int _rank{};
	uint8_t _timer{};
};
}