#include "renderer.hpp"
#include "config.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "core.hpp"
#include "hero_controller.hpp"

#include <SDL.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    Renderer renderer;
    renderer.placeCamera({5.0, 5.0}, {12.0, 9.0});

    Core core;
    core.placeHero({5.0, 5.0});
    core.spawnEnemy({2.0, 8.0});

    HeroControlScheme controlScheme;
    controlScheme.mapAction(Action::MoveLeft, SDLK_a);
    controlScheme.mapAction(Action::MoveRight, SDLK_d);
    controlScheme.mapAction(Action::MoveUp, SDLK_w);
    controlScheme.mapAction(Action::MoveDown, SDLK_s);

    HeroController heroController(controlScheme, core.heroControls());

    FrameTimer timer(config::GameFps);
    for (;;) {
        bool done = false;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
                break;
            }
            heroController.processEvent(event);
        }
        if (done) {
            break;
        }

        int framesPassed = timer.framesPassed();
        for (int i = 0; i < framesPassed; i++) {
            core.update(timer.delta());
            renderer.update(timer.delta());
        }

        if (framesPassed > 0) {
            renderer.clear();
            renderer.renderScene(core.scene());
            renderer.present();
        }
    }

    SDL_Quit();
}
