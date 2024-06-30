#include <iostream>
#include <cstdint>
#include "engine.hpp"
#include "sprite.hpp"
#include "physics.hpp"
// specify screen dimensions
#define ScreenWidth 1920
#define ScreenHeight 1080
#define ScrollSpeed 1
#define ScrollSpeed2 2

// clear Save_data.txt to reset your progress in game !!

int main()
{

    Engine *game = new Engine("Game", ScreenWidth, ScreenHeight);
    Sprite *sprite = new Sprite("./test_assets/Otter_Sprite_Sheet.png", game->renderer, 3, 12);

    // user, please insert layer (starting with with the most "back" layer first) (Please insert 5 layers)
    game->addLayer("./test_assets/1st_layer.png");
    game->addLayer("./test_assets/2nd_layer.png");
    game->addLayer("./test_assets/3rd_layer.png");
    game->addLayer("./test_assets/4th_layer.png");
    game->addLayer("./test_assets/5th_layer.png");

    Image *image2 = game->getLayer(1); // Gets the first layer from the add layer vector

    // user, please insert your "tile" images.
    game->addTiles("./test_assets/Dirt.png");
    game->addTiles("./test_assets/deepDirt.png");
    game->addTiles("./test_assets/Grass.png");
    game->changeTile(1);
    game->setGridSize(32);
    int gridSize = game->getGridSize();
    game->initializeTileMap(gridSize, ScreenWidth, ScreenHeight);

    Uint64 lastFrameTime = SDL_GetPerformanceCounter();

    SDL_Event event;
    while (SDL_PollEvent(&event) >= 0)
    {
        Uint64 currentFrameTime = SDL_GetPerformanceCounter();
        game->setDeltaTime((currentFrameTime - lastFrameTime) / static_cast<double>(SDL_GetPerformanceFrequency()));
        lastFrameTime = currentFrameTime;
        switch (event.type)
        {
        case SDL_QUIT:
            game->save();
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                game->tilemap(gridSize, game->get_Screen_width(), game->get_Screen_height(), mouseX, mouseY);
            }
            break;

        case SDL_MOUSEMOTION:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                game->tilemap(gridSize, game->get_Screen_width(), game->get_Screen_height(), mouseX, mouseY);
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                // User, put as many statements for certain keybind as you wish for currently existing sprite functions
                //  add scrolling here so when sprite moves forward the backround will scroll with it...
            case SDLK_d:
                game->moveRight(sprite, 10);
                game->set_backroundLocation(game->get_backroundLocation() + ScrollSpeed);
                game->set_backroundLocation2(game->get_backroundLocation2() + ScrollSpeed);
                game->set_backroundLocation3(game->get_backroundLocation3() + 3);
                game->set_backroundLocation4(game->get_backroundLocation4() + 4);
                game->set_backroundLocation5(game->get_backroundLocation5() + 5);
                break;
                // same logic except for backwards...
            case SDLK_a:
                game->moveLeft(sprite, 10);
                game->set_backroundLocation(game->get_backroundLocation() - ScrollSpeed);
                game->set_backroundLocation2(game->get_backroundLocation2() - ScrollSpeed2);
                game->set_backroundLocation3(game->get_backroundLocation3() - 3);
                game->set_backroundLocation4(game->get_backroundLocation4() - 4);
                game->set_backroundLocation5(game->get_backroundLocation5() - 5);
                break;

            case SDLK_SPACE:
                game->jump(sprite, 10);
                break;

            case SDLK_4:
                game->respawn(sprite);
                break;
            case SDLK_r:
                game->respawn_x(sprite);
                break;
            case SDLK_f:
                game->spawn(sprite);
                break;
            }
        }
        // 320,180
        //  Clears the renderer, then copies the background and background copy to the render target, and then the foreground is copied.
        SDL_RenderClear(game->renderer);
        game->setRenderCopy(game->backgrounds, 0, 0, 1920, 1080, game->get_Screen_width(), game->get_Screen_height());
        // game->grid(gridSize, ScreenWidth, ScreenHeight);
        game->renderTileMap();
        sprite->drawSelectedSprite(game->renderer);
        game->resolveCollisions(sprite);
        game->applyGravity(sprite);
        sprite->deccelerate();
        sprite->setState();
        sprite->animate(0, 1, 2, 12);
        SDL_RenderPresent(game->renderer);
    }

    // Cleaning up all the variables once SDL_Quit() is called. Destroying the textures and surfaces, and then assigning all pointers used to nullptr
    game->window = nullptr;
    game->renderer = nullptr;
    image2 = nullptr;

    game->~Engine();
    return 0;
}