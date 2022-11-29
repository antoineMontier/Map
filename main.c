#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "graph.h"
#define IN 1
#define OUT -1
#define FRAMES_PER_SECOND 60

int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    SDL_Color *palette = initialiseColors();
    double mouseX = 0, mouseY = 0, pmouseX = 0, pmouseY = 0, zmouseX, zmouseY, clickX = -1, clickY = -1;
    double center_x = WIDTH / 2, center_y = HEIGHT / 2, zoom = 20000;
    int mouse_scroll = 0, click = 0, capsLock = 0, link_mode = 0;
    srand(time(0));
    SDL_Window *w;
    SDL_Renderer *r;

    SDL_Event evt;
    SDL_bool program_launched = SDL_TRUE;
    openSDL(WIDTH, HEIGHT, 0, &w, &r);
    TTF_Font *f;
    setFont(&f, "Roboto-Black.ttf", VERTEX_SIZE);
    TTF_Font *controls;
    setFont(&controls, "Roboto-Light.ttf", 20);

    char *tmp = malloc(10);
    modify_osm_file("./map.txt");
    Graph g;
    createGraph(&g, 20000, 10000);
    createCoordinatesSystem("./new_coord.txt", "./links.txt", &g, center_x, center_y);
    //colorate_welsh_and_powell(&g);

    //shortestPath(&g, 9, 17);

    //printf("%d\n", g.vertexs[633].card);

    //printGraph(&g);

/*
    int*temporary = malloc(g.nb_vertex*sizeof(int));

    neightbours(&g, 0, temporary);

    printf("[");
    for(int i = 0 ; i < g.nb_vertex ; i++)
        printf("%d ", temporary[i]);

    printf("]\n");
    free(temporary);
*/


    while (program_launched)
    {

        //=======MOTION OF MAP============
        if (click && link_mode == 0)
        {
            center_x -= (pmouseX - mouseX) * 8;
            center_y -= (pmouseY - mouseY) * 8;
        }
        //===END MOTION OF MAP============

        //======== ZOOM ============
        if (mouse_scroll == IN)
        {
            if (mouseX != zmouseX || mouseY != zmouseY)
            {
                changeCenter(zoom, &g, center_x, center_y, mouseX, mouseY);
                center_x = mouseX;
                center_y = mouseY;
                zmouseX = mouseX;
                zmouseY = mouseY;
            }
            zoom *= 1.08;
            mouse_scroll = 0;
        }
        else if (mouse_scroll == OUT)
        {
            if (mouseX != zmouseX || mouseY != zmouseY)
            {
                changeCenter(zoom, &g, center_x, center_y, mouseX, mouseY);
                center_x = mouseX;
                center_y = mouseY;
                zmouseX = mouseX;
                zmouseY = mouseY;
            }
            zoom /= 1.08;
            mouse_scroll = 0;
        }
        //========END ZOOM=============

        background(r, 255, 255, 255, WIDTH, HEIGHT); // white bg
        displayGraph(r, f, &g, tmp, palette, center_x, center_y, zoom);
        color(r, 255, 0, 0, 1);
        if (link_mode && clickX != -1 && clickY != -1)
            line(r, clickX, clickY, mouseX, mouseY);
        mark(r, center_x, center_y, 2);

        //==== display control board :
        text(r, 0, 0, "Zoom :          |  link mode :", controls, 0, 0, 0);
        toChar(tmp, (int)(log10(zoom) * 100));
        text(r, 65, 0, tmp, controls, 0, 0, 0);
        if (link_mode)
        {
            color(r, 250, 150, 220, 1);
            roundRect(r, 225, 1, 40, 24, 1, 10, 10, 10, 10);

            if (clickX != -1 && clickY != -1)
            {
                color(r, 150, 30, 70, 1);
                roundRect(r, 225, 1, 20, 24, 1, 10, 0, 10, 0);
            }

            // arrow body :
            color(r, 255, 255, 255, 1);
            roundRect(r, 233, 11, 24, 4, 1, 2, 2, 2, 2);
            // arrow right spike :
            triangle(r, 255, 8,       // top
                     255, 18,         // bottom
                     260, 13, 1);     // middle
            if (capsLock)             // arrow left spike (optionnal) :
                triangle(r, 235, 8,   // top
                         235, 18,     // bottom
                         230, 13, 1); // middle
        }
        color(r, 255, 0, 0, 1);
        roundRect(r, 225, 1, 40, 24, 0, 10, 10, 10, 10);
        line(r, 245, 1, 245, 25);

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
                    link_mode = !link_mode;
                    break;

                case SDLK_v:
                    break;

                case SDLK_r:
                    printf("reloading file...");
                    createCoordinatesSystem("./coordinates.txt", "./links.txt", &g, WIDTH/2, HEIGHT/2);
                    changeCenter(zoom, &g, WIDTH/(2*zoom), HEIGHT/(2*zoom), center_x, center_y);
                    center_x = mouseX;
                    center_y = mouseY;
                    zmouseX = mouseX;
                    zmouseY = mouseY;
                    printf("done !\n");
                    break;

                case SDLK_CAPSLOCK:
                    capsLock = !capsLock;
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
                if (link_mode)
                {
                    if (linkByClick(r, "./links.txt", &g, center_x, center_y, zoom, clickX, clickY, mouseX, mouseY, capsLock))
                    {
                        color(r, 150, 30, 70, 1);
                        roundRect(r, 245, 1, 20, 24, 1, 0, 10, 0, 10);
                        clickX = -1;
                        clickY = -1; // avoid repetitions
                    }
                    else
                    {
                        clickX = mouseX;
                        clickY = mouseY;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                click = 0;
                break;

            default:
                break;
            }
        }
        SDL_RenderPresent(r); // refresh the render
        SDL_Delay(1000 / FRAMES_PER_SECOND);
    }
    free(palette);
    free(tmp);
    TTF_CloseFont(controls);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}
