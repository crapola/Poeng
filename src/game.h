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
// How many levels can be travelled backwards before losing.
const int kRecaptureLimit=3;
// Bonus rarity.
const int kBonusRarity=3;
// Conversion functions.
int PixelToGridX(int p_x);
int PixelToGridY(int p_y);
int GridToPixelX(int p_x);
int GridToPixelY(int p_y);
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
	POWER_BOMB,
	POWER_GRAVITY,
	END
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
		COLLECT_POWER, // value=BrickType x,y=pixel position
		LASER_SHOOT,
		LOSE,
		GAME_OVER,
		WIN,
		LOSE_ONE_BALL,
		DESTROY_POWER, // value=BrickType x,y=pixel position
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
	float acc_y{};
	bool glued{false};
};
struct Level
{
	Cell at(size_t x,size_t y) const;
	Cell& at(size_t x,size_t y);
	Cell atPixels(int x,int y) const;
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
	void CheatBallSpawn();
	void CheatPower(BrickTypes pow);
	void EditBrick(int x_pixels,int y_pixels,BrickTypes brick);
	void Fire();
	const Object& Laser() const;
	const Level& LevelCurrent() const;
	size_t LevelGet() const;
	bool LevelsLoad(const char* p_path);
	void LevelsReset();
	void LevelsShuffle();
	void LevelsSave(const char* path) const;
	void LevelSet(size_t lvl);
	void LevelsValidate();
	int Lives() const;
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
	int _recapture{};
	int _player_y{kMapCoordY};
	uint8_t _player_power{};
	int _player_push{};
	Object _laser{};
	int _lives{};
	int _score{};
};
}
