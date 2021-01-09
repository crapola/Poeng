#pragma once
// std
#include <array>
#include <memory>
#include <vector>
// libs
#include <SDL.h>
// local
#include "audio.h"
#include "bitmap.h"
#include "font.h"
#include "game.h"
#include "texture.h"
#include "scenes/editor.h"
#include "scenes/playing.h"
#include "scenes/title.h"
namespace poeng
{
class GameView
{
public:
	GameView();
	~GameView();
	void AudioInit();
	void CreateTextures(SDL_Renderer*);
	void FontInit();
	bool Event(const SDL_Event& event,int mx,int my);
	void LoadImages();
	void Render(SDL_Renderer* renderer);
	void Update();
private:
	Font _font{};
	Game _game{};
	std::vector<Bitmap> _bitmaps{};
	std::vector<Texture> _textures{};
	Audio _audio{};
	// Scenes.
	Title _title{};
	Playing _playing{};
	Editor _editor{};
	std::array<Scene*,3> _scenes{&_title,&_playing,&_editor};
	size_t _scene_current{};
};
}
