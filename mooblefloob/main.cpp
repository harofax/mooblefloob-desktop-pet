// SDL window with transparent background v1.2
#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_image.h>
#include <Windows.h>

#define SCALE 4;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* creature = nullptr;

// Initializes SDL and creates a window
bool init();

// Loads media files
bool loadMedia();

//Frees media and shuts down SDL
void close();

// Makes a window transparent by setting a transparency color.
bool MakeWindowTransparent(SDL_Window* window, COLORREF colorKey) {
    // Get window handle (https://stackoverflow.com/a/24118145/3357935)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hWnd = wmInfo.info.win.window;

    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // Set transparency color
    return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
}

    // Get resolution of primary monitor
const int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
const int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

int main(int argc, char** argv) {



    if (!init())
    {
        printf("Failed to initialize!\n");
        return 1;
    }
    else
    {
	    if (!loadMedia())
	    {
            printf("Failed to load media files!\n");
            return 1;
	    }
    }

    // Set background color to magenta and clear screen
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
    SDL_RenderClear(gRenderer);

    SDL_Rect dest;
    dest.x = 600;
    dest.y = 600;

    SDL_QueryTexture(creature, nullptr, nullptr, &dest.w, &dest.h);

    dest.w *= SCALE;
    dest.h *= SCALE;
    

    SDL_RenderCopy(gRenderer, creature, nullptr, &dest);

    // Add window transparency (Magenta will be see-through)
    MakeWindowTransparent(gWindow, RGB(255, 0, 255));

    // Render the square to the screen
    SDL_RenderPresent(gRenderer);

    int move_x = 1;
    int move_y = 1;

    // Loop until user quits
    bool quit = false;

	SDL_Event event;

    while (!quit) {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, creature, nullptr, &dest);

        // Add window transparency (Magenta will be see-through)
        MakeWindowTransparent(gWindow, RGB(255, 0, 255));

        

        // Render the square to the screen
        SDL_RenderPresent(gRenderer);

    	while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            if (event.type == SDL_KEYDOWN)
            {
	            switch (event.key.keysym.sym)
	            {
	            case SDLK_SPACE:
                    break;
	            case SDLK_ESCAPE:
                    quit = true;
                    break;
	            }
            }
        }
    }

    close();
    return 0;
}


bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
	else
    {
	    gWindow = SDL_CreateWindow("SDL Transparent Window",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            desktopWidth, desktopHeight, SDL_WINDOW_BORDERLESS);

        if (gWindow == nullptr)
        {
            printf("Window could not get created! SDL_Error: %s\n", SDL_GetError());

            success = false;
        }
		else
        {
	        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    SDL_Surface* load_surf = SDL_LoadBMP("res\\blorpo.bmp");

    if (load_surf == nullptr)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "res\\blorpo.bmp", SDL_GetError());
        success = false;
    }

    creature = SDL_CreateTextureFromSurface(gRenderer, load_surf);

	SDL_FreeSurface(load_surf);

    return success;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

	SDL_DestroyTexture(creature);
    creature = nullptr;

    SDL_Quit();
}

