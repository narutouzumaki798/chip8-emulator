#include <unistd.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <ncurses.h>


#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define RES 16
#define FPS 60     // frames per second
#define InstPS 200 // instructions per second

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;

int screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];


#include "util.h"
#include "chip8.c"


void keypress(int key, int down)
{
    sem_wait(semaphore1);
    switch(key)
    {
    case '4':
	keys[0x1] = down;
	break;
    case '5':
	keys[0x2] = down;
	break;
    case '6':
	keys[0x3] = down;
	break;
    case '7':
	keys[0xc] = down;
	break;

    case 'r':
	keys[0x4] = down;
	break;
    case 't':
	keys[0x5] = down;
	break;
    case 'y':
	keys[0x6] = down;
	break;
    case 'u':
	keys[0xD] = down;
	break;

    case 'f':
	keys[0x7] = down;
	// fprintf(err_fp, "keypress: f down=%d pid=%d\n", down, pid); fflush(err_fp);
	break;
    case 'g':
	keys[0x8] = down;
	break;
    case 'h':
	keys[0x9] = down;
	break;
    case 'j':
	keys[0xE] = down;
	// fprintf(err_fp, "keypress: j down=%d pid=%d\n", down, pid); fflush(err_fp);
	break;

    case 'v':
	keys[0xA] = down;
	break;
    case 'b':
	keys[0x0] = down;
	break;
    case 'n':
	keys[0xB] = down;
	break;
    case 'm':
	keys[0xF] = down;
	break;

    case 'x':
	sem_post(semaphore1);
	stop();
	break;
    }
    sem_post(semaphore1);
}


void draw()
{
    SDL_Rect r;
    int N = SCREEN_HEIGHT, M = SCREEN_WIDTH;
    for(int i=0; i<N; i++)
    {
	for(int j=0; j<M; j++)
	{
	    r.x = j*RES+1;
	    r.y = i*RES+1;
	    r.w = RES-2;
	    r.h = RES-2;

	    int c = screen_buffer[i][j];
	    SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, c*74, c*87, c*34));
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
	int flag = 0;
	while(SDL_PollEvent(&e) != 0) 
	{
	    if(e.type == SDL_QUIT)
		stop = 1;
	    else if(e.type == SDL_KEYDOWN)
	    {
		keypress(e.key.keysym.sym, 1);
		flag = 1;
	    }
	    else if(e.type == SDL_KEYUP)
	    {
		keypress(e.key.keysym.sym, 0);
	    }
	}
	if(debug == 2 && flag == 0)
	{
	    // delay to maintain emu_update rate ,i.e, instruction rate
	    instTime = SDL_GetTicks() - instStart; // instruction duration in ticks (1 tick = 1/1000 second) (for this frame)
	    if(instTime < instDelay)
		SDL_Delay(instDelay - instTime); // delay to maintain instruction rate
	    continue;   
	}


        // emulator update
	emu_update();

	// other updates
	long ticks = SDL_GetTicks();
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
    window = SDL_CreateWindow("chip8 emulator", 820, 480, SCREEN_WIDTH*RES, SCREEN_HEIGHT*RES, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);

    game_loop();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



