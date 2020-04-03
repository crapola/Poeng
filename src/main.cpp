// std
#include <iostream>
// local
#include "gameview.h"
#include "window.h"
int main(int, char**) try
{
	std::cout << "Poeng v2\n";
#ifdef NDEBUG
	std::cout << "Release build.\n";
#else
	std::cout << "Debug build.\n";
#endif
	Window window("Poeng", 640, 480, SDL_WINDOW_RESIZABLE);
	//SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer creation failed: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	poeng::GameView game_view;
	game_view.LoadImages();
	game_view.CreateTextures(renderer);
	game_view.FontInit();
	game_view.AudioInit();
	// Loop.
	bool running = true;
	bool minimized = false;
	SDL_Event event;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
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
						// TODO: Resizing.
						// The window surface gets destroyed on resize.
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
			running &= game_view.Event(event);
		}
		if (!minimized)
		{
			game_view.Update();
			game_view.Render(renderer);
			SDL_RenderPresent(renderer);
		}
		SDL_Delay(15);
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
