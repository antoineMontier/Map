#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "graph.h"
#define IN 1
#define OUT -1
#define NO_ZOOM 1

int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    SDL_Color *palette = initialiseColors();

    double center_x = WIDTH/2, center_y = HEIGHT/2, zoom =1;
    int mouse_scroll = 0;
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
    createCoordinatesSystem("./coordinates.txt", "e", &g);
    printf("%f\n", g.vertexs[0].distance);
    while (program_launched)
    {
        if(mouse_scroll == IN){
            zoom *= 1.05;
            mouse_scroll = 0;
        }else if(mouse_scroll == OUT){
            zoom *= 0.95;
            mouse_scroll = 0;
        }
        background(r, 255, 255, 255, WIDTH, HEIGHT);//while bg
        
        displayGraph(r, f, &g, tmp, palette, center_x, center_y, zoom);
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
                mouse_scroll = evt.wheel.y;
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