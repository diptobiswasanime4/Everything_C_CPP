#include <vector>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_CreateWindowAndRenderer(2000, 2000, 0, &window, &renderer);

    SDL_Event event;
    SDL_Point current;
    vector<SDL_Point> vector_of_points;

    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            SDL_Quit();
            exit(0);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int x, y;
            auto button = SDL_GetMouseState(&x, &y);
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                vector_of_points.emplace_back(SDL_Point{x, y});
            }
        }
    }
    SDL_Delay(3000);

    return 0;
}