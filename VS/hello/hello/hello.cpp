#include <SDL3/SDL.h>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <ctime>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Dungeon grid dimensions
const int GRID_WIDTH = 80;
const int GRID_HEIGHT = 60;
const int CELL_SIZE = 10;

// Button dimensions
const SDL_FRect RANDOMIZE_BUTTON = { WINDOW_WIDTH - 150.0f, 20.0f, 120.0f, 40.0f };

// Room parameters
const int MIN_ROOM_SIZE = 4;
const int MAX_ROOM_SIZE = 10;
const int MAX_ROOMS = 15;

struct Room {
    int x, y;
    int width, height;

    Room(int _x, int _y, int _w, int _h) : x(_x), y(_y), width(_w), height(_h) {}

    bool intersects(const Room& other) const {
        return (x <= other.x + other.width && x + width >= other.x &&
            y <= other.y + other.height && y + height >= other.y);
    }

    SDL_Point center() const {
        return { x + width / 2, y + height / 2 };
    }
};

class Dungeon {
private:
    std::vector<std::vector<bool>> grid; // true = floor, false = wall
    std::vector<Room> rooms;
    std::mt19937 rng;

public:
    Dungeon() : grid(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false)), rng(std::time(nullptr)) {
        generateDungeon();
    }

    void generateDungeon() {
        // Clear existing dungeon
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), false);
        }
        rooms.clear();

        // Generate rooms
        for (int i = 0; i < MAX_ROOMS; i++) {
            int width = MIN_ROOM_SIZE + rng() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1);
            int height = MIN_ROOM_SIZE + rng() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1);
            int x = 1 + rng() % (GRID_WIDTH - width - 2);
            int y = 1 + rng() % (GRID_HEIGHT - height - 2);

            Room newRoom(x, y, width, height);

            // Check if this room intersects with any existing room
            bool overlaps = false;
            for (const auto& room : rooms) {
                if (newRoom.intersects(room)) {
                    overlaps = true;
                    break;
                }
            }

            if (!overlaps) {
                // Add room to the dungeon
                createRoom(newRoom);
                rooms.push_back(newRoom);

                // Connect with previous room
                if (!rooms.empty() && rooms.size() > 1) {
                    SDL_Point newCenter = newRoom.center();
                    SDL_Point prevCenter = rooms[rooms.size() - 2].center();
                    createCorridor(prevCenter.x, prevCenter.y, newCenter.x, newCenter.y);
                }
            }
        }
    }

    void createRoom(const Room& room) {
        for (int y = room.y; y < room.y + room.height; y++) {
            for (int x = room.x; x < room.x + room.width; x++) {
                if (y >= 0 && y < GRID_HEIGHT && x >= 0 && x < GRID_WIDTH) {
                    grid[y][x] = true; // Set as floor
                }
            }
        }
    }

    void createCorridor(int x1, int y1, int x2, int y2) {
        // Horizontal corridor
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            if (x >= 0 && x < GRID_WIDTH && y1 >= 0 && y1 < GRID_HEIGHT) {
                grid[y1][x] = true;
            }
        }

        // Vertical corridor
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            if (x2 >= 0 && x2 < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                grid[y][x2] = true;
            }
        }
    }

    bool isFloor(int x, int y) const {
        if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
            return false;
        }
        return grid[y][x];
    }
};

class DungeonRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Dungeon dungeon;
    bool quit;

public:
    DungeonRenderer() : window(nullptr), renderer(nullptr), quit(false) {
        initialize();
    }

    ~DungeonRenderer() {
        cleanup();
    }

    bool initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        window = SDL_CreateWindow("Procedural Dungeon Generator", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
        if (!window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // In SDL3, SDL_RENDERER_ACCELERATED is no longer needed as a flag
        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        return true;
    }

    void cleanup() {
        if (renderer) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        SDL_Quit();
    }

    void render() {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        // Draw dungeon
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                SDL_FRect cellRect = { static_cast<float>(x * CELL_SIZE),
                                     static_cast<float>(y * CELL_SIZE),
                                     static_cast<float>(CELL_SIZE),
                                     static_cast<float>(CELL_SIZE) };
                if (dungeon.isFloor(x, y)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White floor
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black wall
                }
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }

        // Draw randomize button
        SDL_SetRenderDrawColor(renderer, 50, 150, 200, 255);
        SDL_RenderFillRect(renderer, &RANDOMIZE_BUTTON);

        // Draw button text (simplified as a button outline)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &RANDOMIZE_BUTTON);

        // Present the rendered content
        SDL_RenderPresent(renderer);
    }

    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= RANDOMIZE_BUTTON.x && mouseX <= RANDOMIZE_BUTTON.x + RANDOMIZE_BUTTON.w &&
                    mouseY >= RANDOMIZE_BUTTON.y && mouseY <= RANDOMIZE_BUTTON.y + RANDOMIZE_BUTTON.h) {
                    // Randomize button clicked
                    dungeon.generateDungeon();
                }
            }
        }
    }

    void run() {
        while (!quit) {
            handleEvents();
            render();
            SDL_Delay(16); // ~60 FPS
        }
    }
};

int main(int argc, char* args[]) {
    DungeonRenderer app;
    app.run();
    return 0;
}