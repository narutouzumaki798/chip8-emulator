#include "stdio.h"
#include "SDL2/SDL.h"


#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define RES 10
#define FPS 60     // frames per second
#define InstPS 200 // instructions per second

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;

struct color{ int red; int blue; int green; };
struct color screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];



#include "chip8.c"




void keypress(int key)
{
    switch(key)
    {
    case SDLK_UP:
	break;

    case SDLK_DOWN:
	break;

    case SDLK_LEFT:
	break;

    case SDLK_RIGHT:
	break;

    case SDLK_w:
	break;

    case SDLK_s:
	break;

    case SDLK_a:
	break;

    case SDLK_d:
	break;
    }
}


void draw()
{
    SDL_Rect r;
    int N = SCREEN_HEIGHT, M = SCREEN_WIDTH;
    for(int i=0; i<N; i++)
    {
	for(int j=0; j<M; j++)
	{
	    r.x = j*RES;
	    r.y = i*RES;
	    r.w = RES;
	    r.h = RES;

	    int c = rand()%256; // screen_buffer[i][j].red;
	    SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, c, c, c));
	}
    }
    SDL_UpdateWindowSurface(window);
}


void game_loop()
{
    const long instDelay = 1000/InstPS; // correct no. of ticks for each instruction for maintaning the
                                        // instructions per second (there are 1000 ticks per second)

    const long frameDelay = 1000/FPS;   // correct no. of ticks for each frame for maintaining FPS

    long instTime, instStart; // instruction duration in ticks and instruction start tick
    long lastFrame = 0, frame; // each frame is 'frameDelay' ticks

    emu_start(); // emulator init
    short stop = 0;
    SDL_Event e;
    while(!stop)
    {
        // instruction start tick
	instStart = SDL_GetTicks(); 

        // handle input
	while(SDL_PollEvent(&e) != 0) 
	{
	    if(e.type == SDL_QUIT)
		stop = 1;
	    else if(e.type == SDL_KEYDOWN)
	    {
		keypress(e.key.keysym.sym);
	    }
	}

        // emulator update
	emu_update();

	// other updates
	long ticks = SDL_GetTicks(); // printf("ticks = %ld\n", SDL_GetTicks());
	frame = ticks/frameDelay;
	if(frame > lastFrame) // new frame occurs at 60 Hz
	{
	    draw();
	    if(delay_timer) delay_timer--;
	    if(sound_timer) sound_timer--;
	}
	lastFrame = frame;

	// delay to maintain emu_update rate ,i.e, instruction rate
	instTime = SDL_GetTicks() - instStart; // instruction duration in ticks (1 tick = 1/1000 second) (for this frame)
	if(instTime < instDelay)
	    SDL_Delay(instDelay - instTime); // delay to maintain instruction rate
    }
}


int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("chip8 emulator", 100, 100, SCREEN_WIDTH*RES, SCREEN_HEIGHT*RES, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);


    game_loop();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



