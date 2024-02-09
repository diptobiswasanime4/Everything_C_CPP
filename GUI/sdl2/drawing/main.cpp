#include <SDL2/SDL.h>
#include <string>

using namespace std;

SDL_Rect draw_rect(SDL_Renderer *renderer, int w, int h, int x, int y, string query)
{
    SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
    if (query == "stroke")
    {
        SDL_RenderDrawRect(renderer, &rect);
    }
    else if (query == "fill")
    {
        SDL_RenderFillRect(renderer, &rect);
    }
    return rect;
}

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(640 * 2, 480 * 2, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 2, 2);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect1 = draw_rect(renderer, 100, 100, 0, 0, "stroke");

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect2 = draw_rect(renderer, 150, 150, 50, 50, "fill");

    SDL_Rect intersection;
    SDL_IntersectRect(&rect1, &rect2, &intersection);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &intersection);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 640 / 2, 480 / 2);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    return 0;
}
