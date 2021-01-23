// std
#include <ctime>
#include <iostream>
// libs
#include "emscripten.h"
// local
#include "../gameview.h"
#include "../window.h"
SDL_Renderer* renderer {};
poeng::GameView* game_view_ptr{};
int delta{};
int acc{};
int last_tick{};
void loop()
{
	int mouse_x,mouse_y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
		{
			mouse_x=event.motion.x;
			mouse_y=event.motion.y;
		}
		break;
		default:
			break;
		}
		game_view_ptr->Event(event,mouse_x,mouse_y);
	}
	const int now=SDL_GetTicks();
	delta=now-last_tick;
	last_tick=now;
	acc+=delta;
	const int kUpdatePeriod=16;
	const int kUpdatesPerRenderMax=10;
	for (int i=0; i<kUpdatesPerRenderMax&&acc>=kUpdatePeriod; ++i)
	{
		acc-=kUpdatePeriod;
		game_view_ptr->Update();
	}
	float lerp=acc/static_cast<float>(kUpdatePeriod);
	game_view_ptr->Render(renderer,lerp);
	SDL_RenderPresent(renderer);
}
int main(int, char**) try
{
	SDL_Log("Poeng for the web.");
	Window window("Poeng", 640, 480, SDL_WINDOW_RESIZABLE);
	renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer creation failed: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	srand(std::time(nullptr));
	poeng::GameView game_view;
	game_view_ptr=&game_view;
	game_view.LoadImages();
	game_view.CreateTextures(renderer);
	game_view.FontInit();
	game_view.AudioInit();
	// Loop.
	bool running = true;
	bool minimized = false;
	last_tick=SDL_GetTicks();
	emscripten_set_main_loop(loop,0,true);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,"Exit.");
	// Cleanup.
	SDL_DestroyRenderer(renderer);
	// Exit.
	return EXIT_SUCCESS;
}
catch (const std::runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error'd!", e.what(), 0);
	return EXIT_FAILURE;
}
