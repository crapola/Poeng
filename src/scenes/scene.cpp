#include "scene.h"
namespace poeng
{
void SceneChange(int p_scene_id)
{
		SDL_Event sent;
		SDL_zero(sent);
		sent.type=SCENE_SWITCH;
		sent.user.code=p_scene_id;
		SDL_PushEvent(&sent);
}
Scene::~Scene()
{
}
}