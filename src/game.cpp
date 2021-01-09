#include "Game.h"
// std
#include <algorithm>
#include <fstream>
#include <iostream>
//#define BONUS_DEBUG 5
namespace poeng
{
int PixelToGridX(int p_x)
{
	return p_x/kBrickW;
}
int PixelToGridY(int p_y)
{
	return (p_y-kMapCoordY)/kBrickH;
}
int GridToPixelX(int p_x)
{
	return p_x*kBrickW;
}
int GridToPixelY(int p_y)
{
	return p_y*kBrickH+kMapCoordY;
}
Cell Level::at(size_t x,size_t y) const
{
	return tiles.at(y*40+x);
}
Cell& Level::at(size_t x,size_t y)
{
	return tiles.at(y*40+x);
}
Cell Level::atPixels(int x,int y) const
{
	int tx=PixelToGridX(x);
	int ty=PixelToGridY(y);
	if (tx<0 || tx>=kMapW || ty<0 || ty>=kMapH) return 0;
	return at(tx,ty);
}
/*
// Load the old level file.
void LevelsLegacyLoad(std::array<Level,32>* p_out_lvl)
{
	char compmap[20][11][32];
	std::ifstream lvl("data/levels_legacy.bin",std::ios::binary);
	lvl.read(reinterpret_cast<char*>(compmap),22*11*32);
	for (int level=0; level<32; level++)
	{
		for (int x=0; x<20; x++)
		{
			for (int y=0; y<11; y++)
			{
				(*p_out_lvl)[level].tiles[y*40+x*2+0]=(compmap[x][y][level]&0x0F);
				(*p_out_lvl)[level].tiles[y*40+x*2+1]=(compmap[x][y][level]&0xF0)>>4;
			}
		}
	}
}
// Save level tiles. New format.
void LevelsSave(const std::array<Level,32>& p_lvls)
{
	std::ofstream out("out_level.bin",std::ios::binary);
	for (auto& tm:p_lvls)
	{
		out.write((char*)tm.tiles.begin(),sizeof(tm.tiles));
	}
}
*/
Game::Game()
{
	_balls.reserve(kBallNum);
	_balls_next.reserve(kBallNum);
}
Game::~Game()
{
}
const std::vector<Object>& Game::Balls() const
{
	return _balls;
}
void Game::Fire()
{
	// Unglue balls.
	for (auto& b:_balls)
	{
		if (!b.glued) continue;
		b.glued=false;
		b.vx=PushImpulse(b.vx);
	}
	// Shoot laser.
	if (_player_power==BrickTypes::POWER_LASER && !_laser.glued)
	{
		_laser.x=16;
		_laser.vx=16;
		_laser.vy=0;
		_laser.y=_player_y+14;
		_laser.glued=true;
		_events.push({GameEvent::LASER_SHOOT,0,0,0});
	}
}
void Game::CheatBallSpawn()
{
	BallSpawn(_balls[0]);
}
void Game::CheatPower(BrickTypes p_pow)
{
	_player_power=p_pow;
}
void Game::EditBrick(int p_x,int p_y,BrickTypes p_brick)
{
	const int x=PixelToGridX(p_x);
	const int y=PixelToGridY(p_y);
	// Ignore out of bounds.
	if (x<0 || x>=kMapW || y<0 || y>=kMapH) return;
	auto& lvl=_levels.get()->at(_level_current);
	Cell& c=lvl.at(x,y);
	c=p_brick;
	_levels_copy.get()->at(_level_current).at(x,y)=p_brick;
}
const Object& Game::Laser() const
{
	return _laser;
}
const Level& Game::LevelCurrent() const
{
	return _levels->at(_level_current);
}
size_t Game::LevelGet() const
{
	return _level_current;
}
void Game::LevelsReset()
{
	*_levels=*_levels_copy;
}
void Game::LevelSet(size_t p_lvl)
{
	_level_current=p_lvl%kLevelCount;
}
void Game::LevelsShuffle()
{
	std::random_shuffle(_levels->begin(),_levels->end());
}

int Game::Lives() const
{
	return _lives;
}
void Game::LoadLevels(const char* p_file)
{
	std::ifstream file(p_file,std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error(std::string("Levels file not found: ")+std::string(p_file));
		return;
	}
	std::vector<char> file_content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	auto convertmetal=[](Cell c)
	{
		if (c==BrickTypes::METAL1) c=BrickTypes::METAL2;
		// Also remove out of range stuff.
		if (c>=BrickTypes::END) c=BrickTypes::NONE;
		return c;
	};
	std::transform(file_content.begin(),file_content.end(),file_content.begin(),convertmetal);
	const size_t kSize=kMapW*kMapH;
	size_t num=std::min(_levels->size(),file_content.size()/kSize);
	//std::cout<<"There are "<<num<<" levels\n";
	for (size_t i=0; i<num; ++i)
	{
		auto file_it=file_content.begin()+i*kSize;
		auto tiles_it=_levels->at(i).tiles.begin();
		std::copy_n(file_it,kSize,tiles_it);
		// Wall strength.
		_levels->at(i).wall_strength=10+i/2;
		// Count breakables.
		auto breakable=[](const Cell& c)
		{
			return c>BrickTypes::NONE&&c!=BrickTypes::SOLID;
		};
		_levels->at(i).bricks=std::count_if(_levels->at(i).tiles.begin(),_levels->at(i).tiles.end(),breakable);
	}
	// Keep copy to reset.
	*_levels_copy=*_levels;
}
void Game::PlayerMove(int p_x,int p_y)
{
	_player_y=p_y+(64-p_y)*(p_y<64)+((416-PlayerSize())-p_y)*(p_y>(416-PlayerSize()));
	_player_push=std::max(_player_push,p_x);
	_player_push=std::min(_player_push,128);
	_player_push=std::max(_player_push,0);
	_player_push--;
}
int Game::PlayerPos() const
{
	return _player_y;
}
uint8_t Game::PlayerPower() const
{
	return _player_power;
}
int Game::PlayerPush() const
{
	return _player_push;
}
int Game::PlayerSize() const
{
	return _player_power==BrickTypes::POWER_SIZE?64:32;
}
const GameEvent Game::PollEvent()
{
	GameEvent ret;
	if (_events.empty())
		return ret;
	ret=_events.front();
	_events.pop();
	return ret;
}
int Game::Score() const
{
	return _score;
}
void Game::Start()
{
	LevelsReset();
	_level_current=0;
	_recapture=0;
	_lives=3;
	_player_power=0;
	_laser.glued=false;
	_score=0;
	_balls.clear();
	_balls_next.clear();
	_balls.push_back({15,15,0.5,0.25});
	_balls[0].glued=true;
	//std::random_shuffle(_levels->begin(),_levels->end());

	//_player_power=BrickTypes::POWER_LASER;
	//test
	//_balls.push_back({550,150,-3.75,1.325});
	// Test lots of balls
	//for (int i=0; i<10; ++i) _balls.push_back({260,32,6,0.5325*i});
	/*
		_level_current=20;
		for (auto& tm:*_levels)
		{
			std::fill(tm.tiles.begin(),tm.tiles.end(),rand()%(BrickTypes::POWER_BOMB+1));
			tm.wall_strength=500000;
		}
	*/

}
void Game::Tick()
{
	auto movement=[](Object& o)
	{
		// Speed cap.
		const float speed_max=16;
		o.vx=std::max(-speed_max,std::min(speed_max,o.vx));
		o.vy=std::max(-speed_max,std::min(speed_max,o.vy));
		// Move.
		o.x+=o.vx;
		o.y+=o.vy;
		o.vy+=o.acc_y;
		o.acc_y=std::max(0.0f,o.acc_y-0.0005f);
	};
	auto borders=[&](Object& o)
	{
		const int border=kMapCoordY+kBrickH*kMapH-kBallSize;
		if (o.y>border)
		{
			o.y=border;
			o.vy=-o.vy;
			const int ox=o.x,oy=o.y; // floats to ints.
			_events.push({GameEvent::Event::HIT_BORDERS,0,ox,oy});
		}
		if (o.y<kMapCoordY)
		{
			o.y=kMapCoordY;
			o.vy=-o.vy;
			const int ox=o.x,oy=o.y;
			_events.push({GameEvent::Event::HIT_BORDERS,0,ox,oy});
		}
	};
	auto player=[&](Object& o)
	{
		// If glued, stick and return.
		if (o.glued)
		{
			BallGlue(o);
			return;
		}
		if (o.x<=16 && o.vx<0)
		{
			const auto hit=o.y+kBallSize-_player_y;
			if (hit>0 && hit<PlayerSize()+kBallSize && !o.glued)
			{
				_events.push({GameEvent::HIT_BAT,0,16,0});
				// Glue if player has that power.
				if (_player_power==BrickTypes::POWER_GLUE)
				{
					BallGlue(o);
				}
				else
				{
					// Bounce off player.
					o.x=std::max(5.0f,o.x);
					o.vx=PushImpulse(o.vx);
					o.vy=(hit-(PlayerSize()+kBallSize)/2)/3.0f;
					//_events.push({GameEvent::HIT_BAT,0,16,0});
				}
			}
		}
	};
	auto wall=[&](Object& o,const int ball_count)->int
	{
		if (_levels.get()->at(_level_current).wall_strength>0)
		{
			if (o.x>600)
			{
				o.x=600;
				o.vx=-std::abs(o.vx);
				_levels.get()->at(_level_current).wall_strength--;
				_events.push({GameEvent::Event::HIT_WALL,_levels.get()->at(_level_current).wall_strength,0,0});
			}
		}
		else
		{
			// Can go through.
			if (o.x>=622)
			{
				if (ball_count==1)
				{
					// It's the last one.
					TravelForwards();
					return 0;
				}
				else
				{
					// Copy it to other vector and remove it.
					_balls_next.push_back(o);
					return 1;
				}
			}
		}
		return 0;
	};
	auto loss=[&](Object& o,const int ball_count)->int
	{
		if (o.x<=4)
		{
			_events.push({GameEvent::Event::LOSE_ONE_BALL,0,0,0});
			// Delete this ball if there are more.
			if (ball_count>1)
			{
				return 1;
			}
			else
			{
				// Travel backwards.
				if (_level_current>0 && _recapture<kRecaptureLimit)
				{
					TravelBackwards();
					return 0;
				}
				else
				{
					// Lose a life.
					_events.push({GameEvent::Event::LOSE,0,0,0});
					_lives--;
					_player_power=0;
					_balls_next.clear();
					BallGlue(o);
					o.vx=4;
					o.acc_y=0.0f;
					// Game over.
					if (_lives<0)
					{
						_events.push({GameEvent::GAME_OVER,0,0,0});
					}
				}
			}
		}
		return 0;
	};
	// Return number of new balls.
	auto bricks=[&](Object& o)->int
	{
		// Repeat four times for four directions.
		for (int j=0; j<4; ++j)
		{
			const auto ci=Collide(o);
			if (!ci) continue;
			const int gx=PixelToGridX((*ci).x);
			const int gy=PixelToGridY((*ci).y);
			auto b=Break(gx,gy);
			if (b)
			{
				if (*b>=BrickTypes::POWER_SIZE)
				{
					const int new_balls=BonusCollect(*b,o,gx,gy);
					*b=0;
					return new_balls;
				}
				if (*b==0)
				{
					BonusSpawn(b);
				}
			}
		}
		return 0;
	};
	int passed=0;
	int lost=0;
	int new_balls=0;
	for (auto& b:_balls)
	{
		movement(b);
		borders(b);
		player(b);
		new_balls+=bricks(b);
		lost+=loss(b,_balls.size()-passed-lost);
		passed+=wall(b,_balls.size()-passed-lost);
	}
	// Delete lost and passed balls.
	auto deleteball=[](const Object& o)
	{
		return o.x<4|| o.x>622;
	};
	_balls.erase(std::remove_if(_balls.begin(),_balls.end(),deleteball),_balls.end());
	// Add extra balls.
	Object o=_balls.back();
	for (int i=0; i<new_balls; ++i)
	{
		BallSpawn(o);
	}
	// Update laser.
	LaserUpdate();
}
void Game::BallGlue(Object& p_o)
{
	p_o.glued=true;
	p_o.x=16;
	p_o.y=_player_y+(PlayerSize()/2)-8;
}
void Game::BallSpawn(Object o)
{
	o.vx=std::abs(o.vx+(rand()%10)/10.f);
	o.vy=std::abs(o.vy+(rand()%10)/10.f);
	o.acc_y+=(5-rand()%10)/10.f;
	if (_balls.size()<kBallNum)
		_balls.push_back(o);
}
//<Number of new balls.
int Game::BonusCollect(Cell p_c,Object& p_instigator,int p_gx,int p_gy)
{
	_events.push({GameEvent::COLLECT_POWER,p_c,GridToPixelX(p_gx),GridToPixelY(p_gy)});
	switch (p_c)
	{
	case BrickTypes::POWER_SIZE:
		_player_power=BrickTypes::POWER_SIZE;
		break;
	case BrickTypes::POWER_GLUE:
		_player_power=BrickTypes::POWER_GLUE;
		break;
	case BrickTypes::POWER_LASER:
		_player_power=BrickTypes::POWER_LASER;
		break;
	case BrickTypes::POWER_EXTRA1:
		return 1;
		break;
	case BrickTypes::POWER_EXTRA3:
		return 3;
		break;
	case BrickTypes::POWER_BOMB:
		Explode(p_gx,p_gy);
		break;
	case BrickTypes::POWER_GRAVITY:
		p_instigator.acc_y=1.0f;
		break;
	default:
		break;
	}
	return 0;
}
void Game::BonusSpawn(Cell* c)
{
	if (rand()%kBonusRarity==0)
	{
		int r=rand()%(BrickTypes::END-BrickTypes::POWER_SIZE);
#ifdef BONUS_DEBUG
		r=BONUS_DEBUG;
#endif
		*c=BrickTypes::POWER_SIZE+r;
	}
}
// Break regular brick.
// > x,y in grid.
// < Pointer to brick.
Cell* Game::Break(int p_x,int p_y)
{
	auto& lvl=_levels.get()->at(_level_current);
	// Ignore out of bounds.
	if (p_x<0 || p_x>=kMapW || p_y<0 || p_y>=kMapH) return nullptr;
	Cell& c=lvl.at(p_x,p_y);
	// Ignore empty.
	if (c==0) return nullptr;
	// Ignore power-ups.
	if (c>=BrickTypes::POWER_SIZE) return &c;
	// Regular bricks.
	_events.push({GameEvent::Event::HIT_BRICK,c,p_x,p_y});
	if (c==BrickTypes::METAL2)
	{
		c--;
		return &c;
	}
	if (c<=BrickTypes::METAL1 && c!=BrickTypes::SOLID)
	{
		// Score.
		_score+=50+c*10;
		// Delete.
		c=0;
		// Reduce brick count.
		if (--lvl.bricks==0)
		{
			lvl.wall_strength=0;
			_events.push({GameEvent::Event::HIT_WALL,0,0,0});
		}
	}
	return &c;
}
std::optional<Game::CollisionInfo> Game::Collide(Object& o)
{
	auto& lvl=_levels.get()->at(_level_current);
	const int cs=14;
	const int corners[8]= {1,1,cs,1,1,cs,cs,cs};
	int xb=0,yb=0;
	bool hit_x=false,hit_y=false;
	// Find hit in every corner.
	for (int i=0; i<8; i+=2)
	{
		auto xc=corners[i];
		auto yc=corners[i+1];
		if (lvl.atPixels(xc+o.x+o.vx, yc+o.y)>0)
		{
			xb=xc+o.x+o.vx;
			yb=yc+o.y;
			hit_x=true;
			break;
		}
		if (lvl.atPixels(xc+o.x, yc+o.y+o.vy)>0)
		{
			xb=xc+o.x;
			yb=yc+o.y+o.vy;
			hit_y=true;
			break;
		}
	}
	if (hit_x)
	{
		o.vx=-o.vx;
	}
	else if (hit_y)
	{
		o.vy=-o.vy;
	}
	if (hit_x || hit_y)
		return CollisionInfo{xb,yb};
	else
		return std::nullopt;
}
//>x,y in tiles.
void Game::Explode(int p_x,int p_y)
{
	const int first_x=p_x-1;
	const int first_y=p_y-1;
	for (int y=first_y; y<first_y+3; ++y)
		for (int x=first_x; x<first_x+3; ++x)
		{
			auto c=Break(x,y);
			if (c && *c>=POWER_SIZE)
			{
				if (*c!=POWER_BOMB)
				{
					const int rx=GridToPixelX(p_x);
					const int ry=GridToPixelY(p_y);
					_events.push({GameEvent::DESTROY_POWER,*c,rx,ry});
				}
				/*
				else
				{
					*c=0;
					Explode(x,y);
				}
				*/
				*c=0;
			};
		}
}
void Game::LaserUpdate()
{
	if (!_laser.glued) return;
	_laser.x+=_laser.vx;
	// Collision.
	const auto& lvl=_levels.get()->at(_level_current);
	if (lvl.atPixels(_laser.x,_laser.y)>0)
	{
		const int gx=PixelToGridX(_laser.x);
		const int gy=PixelToGridY(_laser.y);
		auto c=Break(gx,gy);
		// Handle bonus.
		if (c && *c>=POWER_SIZE)
		{
			const int rx=GridToPixelX(gx);
			const int ry=GridToPixelY(gy);
			if (*c==POWER_BOMB)
			{

				_events.push({GameEvent::COLLECT_POWER,*c,rx,ry});
				Explode(gx,gy);
			}
			else
			{
				_events.push({GameEvent::DESTROY_POWER,*c,rx,ry});
				*c=0;
			}
		}
		_laser.glued=false;
	}
	if (_laser.x>640) _laser.glued=false;
}
float Game::PushImpulse(float p_vx)
{
	p_vx=std::min(6.0f,std::abs(p_vx));
	return std::max(4.0f,p_vx)+_player_push/20.0f;;
}
void Game::TravelBackwards()
{
	// Ball.
	auto& o=_balls[0];
	o.x=620;
	o.vx=-16;// Was -8
	// Deduce score.
	int penalty=100+_level_current*10;
	_score=std::max(penalty,_score-penalty);
	LevelSet(--_level_current);
	_recapture++;
	// Lose balls that were passed.
	_balls_next.clear();
}
void Game::TravelForwards()
{
	if (!_balls_next.empty())
	{
		std::move(std::begin(_balls_next), std::end(_balls_next), std::back_inserter(_balls));
		_balls_next.clear();
	}
	for (auto& b:_balls)
	{
		b.x=17;
	}
	_recapture=0;
	// Check victory.
	if (_level_current==kLevelCount-1)
	{
		Win();
	}
	else
	{
		LevelSet(++_level_current);
	}

}
void Game::Win()
{
	_events.push({GameEvent::WIN,0,0,0});
	const auto score=_score;
	const auto li=_lives;
	Start();
	LevelsShuffle();
	_score=score+10000;
	_lives=li+1;
}
}
