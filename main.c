#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "graph.h"
#define IN 1
#define OUT -1
#define NO_ZOOM 0
#define ZOOM_POWER 0.05

int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    SDL_Color *palette = initialiseColors();

    double center_x = WIDTH/2, center_y = HEIGHT/2;

    srand(time(0));
    SDL_Window *w;
    SDL_Renderer *r;

    SDL_Event evt;
    SDL_bool program_launched = SDL_TRUE;
    openSDL(WIDTH, HEIGHT, 0, &w, &r);
    TTF_Font *f;
    setFont(&f, "Roboto-Black.ttf", VERTEX_SIZE * 0.8);

    char *tmp = malloc(10);
    Graph g;
    createGraph(&g, 500, 2000);
    addVertex(&g, 0, 50, 3.1415/4, 3);
    addVertex(&g, 1, 150, 3.1415, 2);
    addArete(&g, 0, 1, 5);
    addArete(&g, 1, 1, 0);
    while (program_launched)
    {
        background(r, 255, 255, 255, WIDTH, HEIGHT);//while bg
        
        
        displayGraph(r, f, &g, tmp, palette, center_x, center_y, 1);
        color(r, 255, 0, 0, 1);
        mark(r, center_x, center_y, 2);
        
     

        while (SDL_PollEvent(&evt))
        { // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (evt.type)
            {

            case SDL_QUIT:
                program_launched = SDL_FALSE; // quit the program if the user closes the window
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (evt.key.keysym.sym)
                { // returns the key ('0' ; 'e' ; 'SPACE'...)

                case SDLK_ESCAPE:
                    program_launched = SDL_FALSE; // escape the program by pressing esc
                    break;

                case SDLK_l:
                    break;

                case SDLK_v:
                    break;

                case SDLK_CAPSLOCK:
                    break;

                default:
                    break;
                }
            case SDL_MOUSEWHEEL:
                //zoom = evt.wheel.y;
                break;

            case SDL_MOUSEMOTION:
                break;

            case SDL_MOUSEBUTTONDOWN:
                break;

            case SDL_MOUSEBUTTONUP:
                break;

            default:
                break;
            }
        }
        SDL_RenderPresent(r); // refresh the render
        SDL_Delay(33);
    }
    free(palette);
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}