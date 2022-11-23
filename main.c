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
    double mouseX = 0, mouseY = 0, pmouseX = 0, pmouseY = 0, mapValueX, mapValueY;
    double center_x = WIDTH/2, center_y = HEIGHT/2, zoom =20000;
    int mouse_scroll = 0, click = 0;
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
    createCoordinatesSystem("./coordinates.txt", "./links.txt", &g, center_x, center_y, &mapValueX, &mapValueY);
    int i = 0;
    while (program_launched)
    {

        //=======MOTION OF MAP============
        if(click){
            center_x -= (pmouseX - mouseX)*5;
            center_y -= (pmouseY - mouseY)*5;
        }   
        //===END MOTION OF MAP============

        //======== ZOOM ============
        if(mouse_scroll == IN){
            center_x = mouseX;
            center_y = mouseY;
            //changeCenter(&g, center_x, center_y, WIDTH/2, HEIGHT/2);
            //center_x = WIDTH/2;
            //center_y = HEIGHT/2;
            //center_x +=  (mouseX - WIDTH/2)*1.05;
            //center_y += (mouseY - HEIGHT/2)*1.05;
            zoom *= 1.05;
            mouse_scroll = 0;
        }else if(mouse_scroll == OUT){
            center_x = mouseX;
            center_y = mouseY;
            //changeCenter(&g, center_x, center_y, WIDTH/2, HEIGHT/2);
            //center_x = WIDTH/2;
            //center_y = HEIGHT/2;
            //center_x +=  (mouseX - WIDTH/2)*1.05;
            //center_y += (mouseY - HEIGHT/2)*1.05;
            //center_x /= 1.05;
            //center_y /= 1.05;
            zoom /= 1.05;
            mouse_scroll = 0;
        }
        //========END ZOOM=============




        background(r, 255, 255, 255, WIDTH, HEIGHT);//white bg
        printf("bg dsiplayed\n");
        displayGraph(r, f, &g, tmp, palette, center_x, center_y, zoom);
        printf("graph displayed\n");
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
                    changeCenter(r, zoom, &g, center_x, center_y, mouseX, mouseY, mapValueX, mapValueY);
                    printf("finished\n");
                    /*center_x = mouseX;
                    center_y = mouseY;*/
                    break;

                case SDLK_v:
                    break;

                case SDLK_CAPSLOCK:
                    break;

                case SDLK_KP_PLUS:
                    zoom *= 1.5;
                    break;

                case SDLK_KP_MINUS:
                    zoom /= 1.5;
                    break;

                default:
                    break;
                }
            case SDL_MOUSEWHEEL:
                mouse_scroll = evt.wheel.y;
                break;

            case SDL_MOUSEMOTION:
                pmouseX = mouseX;
                pmouseY = mouseY;
                mouseX = evt.button.x;
                mouseY = evt.button.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                click = 1;
                break;

            case SDL_MOUSEBUTTONUP:
                click = 0;
                break;

            default:
                break;
            }
        }
        printf("%d\n", i++);
        SDL_RenderPresent(r); // refresh the render
        SDL_Delay(330);
    }
    free(palette);
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}