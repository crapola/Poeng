// std
#include <ctime>
#include <iostream>
// libs
#include "emscripten.h"
// local
#include "../gameview.h"
#include "../window.h"

SDL_Renderer* renderer{};
poeng::GameView* game_view_ptr{};

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
			// Compute scaled mouse position.
			/*
			const int x=event.motion.x;
			const int y=event.motion.y;
			float sx,sy;
			SDL_RenderGetScale(renderer,&sx,&sy);
			int w,h;
			SDL_GetWindowSize(window,&w,&h);
			//SDL_Log("scale %f %f, size %d %d, mouse %d %d",sx,sy,w,h,mouse_x,mouse_y);
			const int ox=std::max(0,w-640*static_cast<int>(sx))/2;
			const int oy=std::max(0,h-480*static_cast<int>(sy))/2;
			mouse_x=(x-ox)/sx;
			mouse_y=(y-oy)/sy;
			*/
			mouse_x=event.motion.x;
			mouse_y=event.motion.y;
		}
		break;
		default:
			break;
		}
		game_view_ptr->Event(event,mouse_x,mouse_y);
	}
	game_view_ptr->Update();
	game_view_ptr->Render(renderer);
	SDL_RenderPresent(renderer);
}
int main(int, char**) try
{
	SDL_Log("Poeng for the web.");
	Window window("Poeng", 640, 480, SDL_WINDOW_RESIZABLE);
	renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
	emscripten_set_main_loop(loop,70,true);
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
