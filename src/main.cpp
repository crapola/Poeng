// std
#include <ctime>
#include <iostream>
// local
#include "gameview.h"
#include "window.h"
int main(int, char**) try
{
	SDL_Log("Poeng v2.");
#ifdef NDEBUG
	SDL_Log("Release build.");
#else
	SDL_Log("Debug build.");
#endif
	Window window("Poeng", 640, 480, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer creation failed: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	srand(std::time(nullptr));
	poeng::GameView game_view;
	game_view.LoadImages();
	game_view.CreateTextures(renderer);
	game_view.FontInit();
	game_view.AudioInit();
	// Loop.
	int delta=0;
	int acc=0;
	int last_tick=SDL_GetTicks();
	bool running = true;
	bool minimized = false;
	SDL_Event event;
	while (running)
	{
		int mouse_x,mouse_y;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
			{
				// Compute scaled mouse position.
				const int x=event.motion.x;
				const int y=event.motion.y;
				float sx,sy;
				SDL_RenderGetScale(renderer,&sx,&sy);
				int w,h;
				SDL_GetWindowSize(window,&w,&h);
				const int ox=std::max(0,w-640*static_cast<int>(sx))/2;
				const int oy=std::max(0,h-480*static_cast<int>(sy))/2;
				mouse_x=(x-ox)/sx;
				mouse_y=(y-oy)/sy;
			}
			break;
			case SDL_QUIT:
				running = false;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_MAXIMIZED:
					SDL_Log("Window %d maximized", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					SDL_Log("Window %d minimized", event.window.windowID);
					minimized = true;
					break;
				case SDL_WINDOWEVENT_SHOWN:
					SDL_Log("Window %d shown", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					SDL_Log("Window %d resized", event.window.windowID);
					{
						// Letterboxing and resizing.
						// Reset scale first for consistant behaviour.
						SDL_RenderSetScale(renderer,1,1);
						const int w=event.window.data1;
						const int h=event.window.data2;
						const int size=(w>=1280 && h>=960)?2:1;
						const int kW=640*size;
						const int kH=480*size;
						const int x=std::max(0,w-kW)/2;
						const int y=std::max(0,h-kH)/2;
						const SDL_Rect rect{x,y,kW,kH};
						SDL_RenderSetViewport(renderer,&rect);
						SDL_RenderSetScale(renderer,size,size);
					}
					break;
				case SDL_WINDOWEVENT_RESTORED:
					SDL_Log("Window %d restored", event.window.windowID);
					minimized = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			running &= game_view.Event(event,mouse_x,mouse_y);
		}
		if (!minimized)
		{
			const int now=SDL_GetTicks();
			delta=now-last_tick;
			last_tick=now;
			acc+=delta;
			const int kUpdatePeriod=16;
			const int kUpdatesPerRenderMax=10;
			for(int i=0;i<kUpdatesPerRenderMax&&acc>=kUpdatePeriod;++i)
			{
				acc-=kUpdatePeriod;
				game_view.Update();
			}
			float lerp=acc/static_cast<float>(kUpdatePeriod);
			game_view.Render(renderer,lerp);
			SDL_RenderPresent(renderer);
		}
		SDL_Delay(0);
	}
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
