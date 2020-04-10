#pragma once
/*******************************************************************************
Game
*******************************************************************************/
// std
#include <array>
#include <queue>
#include <memory>
#include <optional>
#include <vector>
namespace poeng
{
// Maximum number of balls.
const int kBallNum=32;
// Ball pixel dimensions.
const int kBallSize=16;
// Brick pixel dimensions.
const int kBrickW=16;
const int kBrickH=32;
// Number of levels.
const int kLevelCount=32;
// Number of columns and rows for one level tilemap.
const int kMapW=40;
const int kMapH=11;
// Tilemap pixel coordinates on screen.
const int kMapCoordY=64;
enum BrickTypes:uint8_t
{
	NONE=0,
	RED=1, // 1 to 7 are colored bricks.
	SOLID=8,
	METAL1=9,
	METAL2=10,
	POWER_SIZE=11,
	POWER_GLUE,
	POWER_LASER,
	POWER_EXTRA1,
	POWER_EXTRA3,
	POWER_BOMB
};
typedef uint8_t Cell;
struct GameEvent
{
	enum Event
	{
		NONE=0,
		HIT_BRICK,
		HIT_WALL,
		HIT_BAT,
		HIT_BORDERS,
		COLLECT_POWER,
		LASER_SHOOT,
		LOSE,
		GAME_OVER,
		WIN
	};
	Event event{NONE};
	int value{},x{},y{};
	operator bool()
	{
		return event!=NONE;
	}
};
struct Object
{
	float x,y,vx,vy;
	bool glued{false};
};
struct Level
{
	Cell at(size_t x,size_t y) const
	{
		return tiles.at(y*40+x);
	}
	Cell& at(size_t x,size_t y)
	{
		return tiles.at(y*40+x);
	}
	Cell atPixels(int x,int y) const // TODO: move conversion elsewhere.
	{
		int tx=x/kBrickW;
		int ty=(y-kMapCoordY)/kBrickH;
		if (tx<0 || tx>=kMapW || ty<0 || ty>=kMapH) return 0;
		return at(tx,ty);
	}
	std::array<Cell,kMapW*kMapH> tiles{};
	int bricks{};
	int wall_strength{10};
};
class Game
{
public:
	Game();
	~Game();
	const std::vector<Object>& Balls() const;
	void BallSpawnCheat();
	void Fire();
	const Object& Laser() const;
	const Level& LevelCurrent() const;
	size_t LevelGet() const;
	void LevelsReset();
	void LevelSet(size_t lvl);
	int Lives() const;
	void LoadLevels(const char* file);
	void PlayerMove(int x,int y);
	int PlayerPos() const;
	uint8_t PlayerPower() const;
	int PlayerPush() const;
	int PlayerSize() const;
	const GameEvent PollEvent();
	int Score() const;
	void Start();
	void Tick();
private:
	struct CollisionInfo
	{
		int x{},y{};
	};
	void BallGlue(Object&);
	void BallSpawn(Object);
	int BonusCollect(Cell,Object& instigator,int grid_x,int grid_y);
	void BonusSpawn(Cell*);
	Cell* Break(int x,int y);
	std::optional<CollisionInfo> Collide(Object&);
	// Break 3x3 around tile coordinates.
	void Explode(int x,int y);
	void LaserUpdate();
	float PushImpulse(float vx);
	void TravelBackwards();
	void TravelForwards();
	void Win();
	std::queue<GameEvent> _events{};
	std::vector<Object> _balls{}; // Active balls.
	std::vector<Object> _balls_next{}; // Balls in reserve.
	std::unique_ptr<std::array<Level,kLevelCount>> _levels=std::make_unique<std::array<Level,kLevelCount>>();
	std::unique_ptr<std::array<Level,kLevelCount>> _levels_copy=std::make_unique<std::array<Level,kLevelCount>>();
	size_t _level_current{};
	int _player_y{kMapCoordY};
	uint8_t _player_power{};
	int _player_push{};
	Object _laser{};
	int _lives{};
	int _score{};
};
}
