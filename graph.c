#include "graph.h"
#include <string.h>

void createGraph(Graph *g, int v, int a)
{
    g->v = v;
    g->a = a;
    g->vertexs = malloc(v * sizeof(Vertex));
    g->aretes = malloc(a * sizeof(Arete));
    g->nb_arete = 0;
    g->nb_vertex = 0;
}

void destructGraph(Graph *g)
{
    g->v = 0;
    g->a = 0;
    free(g->vertexs);
    free(g->aretes);
    g->nb_arete = 0;
    g->nb_vertex = 0;
}

void addVertex(Graph *g, int id, double distance, double angle, int color)
{
    if (g->v <= g->nb_vertex)
    {
        printf("you reached the max amount of vertex\n");
        return;
    }
    Vertex v;
    v.id = id;
    v.distance = distance;
    v.angle = angle;
    v.card = 0;
    if (color == -2)
        v.color = rand() % NB_COLOR;
    else
        v.color = color;
    (g->vertexs)[g->nb_vertex] = v;
    (g->nb_vertex)++;
}

void addArete(Graph *g, int start_id, int end_id, double weight)
{
    if (g->a <= g->nb_arete)
    {
        printf("you reached the max amount of arete\n");
        return;
    }
    Arete a;
    a.start = start_id;
    a.end = end_id;
    a.weight = weight;
    (g->aretes)[g->nb_arete] = a;
    (g->nb_arete)++;

    (g->vertexs)[start_id].card++;
    (g->vertexs)[end_id].card++;
}

void printGraph(Graph *g)
{
    printf("graph : %d vertex, %d arete\n", g->nb_vertex, g->nb_arete);
    for (int i = 0; i < g->nb_vertex; i++)
        printVertex((g->vertexs) + i);
    for (int i = 0; i < g->nb_arete; i++)
        printArete((g->aretes) + i);
    ;
    printf("\n");
}

void printVertex(Vertex *v)
{
    printf("===================== Vertex\n----Identifier----\n%d \n---dist angle---\n(%f ; %f rad)\n-----Cardinal-----\n%d\n------Color-------\n%d\n", v->id, v->distance, v->angle, v->card, v->color);
}

void printArete(Arete *a)
{
    printf("=====================  Arete\n---------weight\n          %f\n  %d -----------> %d\n", a->weight, a->start, a->end);
}

void createCompleteGraph(Graph *g, int n)
{
    if (n <= 0)
        return;
    for (int i = 0; i < n; i++)
    {
        g->vertexs[i].distance= fmin(HEIGHT, WIDTH)*0.48;
        g->vertexs[i].angle = 3.1415*2*i/n;
        g->vertexs[i].id = i; // add vertexs and create a circle with their coordinates
        g->vertexs[i].color = i % NB_COLOR;
        g->vertexs[i].card = n; // is the card increased if a connexion exists or if an arrow is on the vertex ???
        g->nb_vertex = i + 1;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            g->aretes[i * n + j].start = i;
            g->aretes[i * n + j].end = j;
            g->nb_arete = i * n + j + 1;
        }
}

void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp, SDL_Color *c, int artificial_center_x, int artificial_center_y, double zoom_power)
{
    // display vertexs
    for (int i = 0; i < g->nb_vertex; i++)
    {
        // color gestion
        if (g->vertexs[i].color != NO_COLOR)
        {
            color(r, c[g->vertexs[i].color].r, c[g->vertexs[i].color].g, c[g->vertexs[i].color].b, c[g->vertexs[i].color].a);
            circle(r, artificial_center_x + cos(g->vertexs[i].angle) * zoom_power * g->vertexs[i].distance, artificial_center_y + sin(g->vertexs[i].angle) * zoom_power * g->vertexs[i].distance, VERTEX_SIZE, 1);
        }
        color(r, 0, 0, 0, 1); // black border
        circle(r, artificial_center_x + cos(g->vertexs[i].angle) * zoom_power * g->vertexs[i].distance, artificial_center_y + sin(g->vertexs[i].angle) * zoom_power * g->vertexs[i].distance, VERTEX_SIZE, 0);
        toChar(tmp, g->vertexs[i].id);
        // printf("%d\n", g->vertexs[i].id);
        text(r, artificial_center_x + cos(g->vertexs[i].angle) * zoom_power * g->vertexs[i].distance, artificial_center_y + sin(g->vertexs[i].angle) * zoom_power * g->vertexs[i].distance, tmp, f, 0, 0, 0);
    }

    // display aretes
   double sx, sy, ex, ey;
    for (int i = 0; i < g->nb_arete; i++)
    {
        sx = artificial_center_x + (cos(g->vertexs[g->aretes[i].start].angle) * g->vertexs[g->aretes[i].start].distance * zoom_power);
        sy = artificial_center_y + (sin(g->vertexs[g->aretes[i].start].angle) * g->vertexs[g->aretes[i].start].distance * zoom_power);
        ex = artificial_center_x + (cos(g->vertexs[g->aretes[i].end].angle)   * g->vertexs[g->aretes[i].end].distance   * zoom_power);
        ey = artificial_center_y + (sin(g->vertexs[g->aretes[i].end].angle)   * g->vertexs[g->aretes[i].end].distance   * zoom_power);
        color(r, 0, 0, 0, 0);

        double theta = atan2(ey - sy, ex - sx);
        double theta2 = atan2(sy - ey, sx - ex);
        if (g->aretes[i].start != g->aretes[i].end) // exception
        {
            line(r, sx + VERTEX_SIZE * cos(theta), sy + VERTEX_SIZE * sin(theta), ex + VERTEX_SIZE * cos(theta2), ey + VERTEX_SIZE * sin(theta2));
            // arrow end
            triangle(r, ex + (VERTEX_SIZE * 2) * cos(theta2) + VERTEX_SIZE * 0.2 * cos(theta2 + 3.1415 / 2), ey + (VERTEX_SIZE * 2) * sin(theta2) + VERTEX_SIZE * 0.2 * sin(theta2 + 3.1415 / 2),
                     ex + (VERTEX_SIZE * 2) * cos(theta2) + VERTEX_SIZE * 0.2 * cos(theta2 - 3.1415 / 2), ey + (VERTEX_SIZE * 2) * sin(theta2) + VERTEX_SIZE * 0.2 * sin(theta2 - 3.1415 / 2),
                     ex + VERTEX_SIZE * cos(theta2), ey + VERTEX_SIZE * sin(theta2), 1); // end of arrow
        }
        else
        { // reflexive arete exception
            // let's make an artificial circle :
            double precision = 0.5;
            for (int a = sx - VERTEX_SIZE; a < sx + VERTEX_SIZE * 2; a++)
            {
                for (int b = sy; b < sy + VERTEX_SIZE * 2; b++)
                {
                    if (dist(sx, sy, a, b) > VERTEX_SIZE && fabs(dist(sx, sy + VERTEX_SIZE * 1.2, a, b) - VERTEX_SIZE * 0.7) < precision)
                        point(r, a, b);
                }
            }
            triangle(r, sx + VERTEX_SIZE * 0.6, sy + VERTEX_SIZE * 0.8,  // end of arrow
                     sx + VERTEX_SIZE * 0.9, sy + VERTEX_SIZE * 1.1,     // right
                     sx + VERTEX_SIZE * 0.5, sy + VERTEX_SIZE * 1.3, 1); // left
        }
    }
}

SDL_Color *initialiseColors()
{
    SDL_Color *palette = malloc(NB_COLOR * sizeof(SDL_Color));

    palette[0].r = 252; // yellow
    palette[0].g = 186;
    palette[0].b = 1;

    palette[1].r = 252; // red
    palette[1].g = 3;
    palette[1].b = 3;

    palette[2].r = 147; // lime
    palette[2].g = 252;
    palette[2].b = 0;

    palette[5].r = 3; // soft blue
    palette[5].g = 215;
    palette[5].b = 252;

    palette[3].r = 252; // soft pink
    palette[3].g = 134;
    palette[3].b = 219;

    palette[4].r = 163; // light brown
    palette[4].g = 144;
    palette[4].b = 3;

    palette[10].r = 250; // beige
    palette[10].g = 237;
    palette[10].b = 137;

    palette[6].r = 193; // soft green
    palette[6].g = 255;
    palette[6].b = 143;

    palette[7].r = 86; // kaki
    palette[7].g = 125;
    palette[7].b = 55;

    palette[8].r = 39; // hard green
    palette[8].g = 87;
    palette[8].b = 1;

    palette[9].r = 5; // azur
    palette[9].g = 245;
    palette[9].b = 145;

    palette[11].r = 5; // deep blue
    palette[11].g = 25;
    palette[11].b = 250;

    palette[12].r = 90; // hard magenta
    palette[12].g = 7;
    palette[12].b = 245;

    palette[13].r = 175; // soft magenta
    palette[13].g = 134;
    palette[13].b = 252;

    palette[14].r = 66; // hard brown
    palette[14].g = 58;
    palette[14].b = 1;

    for (int i = 0; i < NB_COLOR; i++)
        palette[i].a = 1;

    return palette;
}

void weightAsDistance(Graph *g)
{
    for (int i = 0; i < g->nb_arete; i++)
    {
        g->aretes[i].weight = dist(cos(g->vertexs[g->aretes[i].start].angle) * g->vertexs[g->aretes[i].start].distance,
                                   sin(g->vertexs[g->aretes[i].start].angle) * g->vertexs[g->aretes[i].start].distance,
                                    cos(g->vertexs[g->aretes[i].end].angle)  * g->vertexs[g->aretes[i].end].distance,
                                    sin(g->vertexs[g->aretes[i].end].angle)  * g->vertexs[g->aretes[i].end].distance);
    }
}

void createCoordinatesSystem(const char *file_coord, const char *file_links, Graph *g, double cx, double cy)
{
    FILE *fc = fopen(file_coord, "r");
    if (fc == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(fc);
        return;
    }

    // first know the min/max value
    double xmin, xmax, ymin, ymax;
    char buffer[128];
    char bin[16];
    double x, y;
    int a, b;

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MIN") != 0)
        fprintf(stderr, "no min value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmin, bin, &ymin);

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MAX") != 0)
        fprintf(stderr, "no max value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmax, bin, &ymax);

    x = fmin(xmin, xmax);
    y = fmax(xmin, xmax);
    xmin = x;
    xmax = y;
    x = fmin(ymin, ymax);
    y = fmax(ymin, ymax);
    ymin = x;
    ymax = y;
    int links_reading = 0;

    // here, xmin ymin xmax ymax are initialized !
    int i = 0;
    while (!feof(fc))
    { // read while end of file isn't reached
        fgets(buffer, sizeof(buffer), fc);

        if (!links_reading)
        {
            sscanf(buffer, "%6s", bin);
            sscanf(buffer, "%lf%3s%lf", &x, bin, &y);

            g->vertexs[i].angle = -atan2(x - (xmin + xmax)/2, y - (ymin + ymax)/2);
            g->vertexs[i].distance = dist((xmin + xmax)/2, (ymin + ymax)/2, x, y);
            g->vertexs[i].id = i;
            g->vertexs[i].color = NO_COLOR;
            g->nb_vertex = i + 1;
            i++;
            if (i >= g->v)
            {
                fprintf(stderr, "graph vertex array is too small\n");
                fclose(fc);
                return;
            }
        }
    }

    fclose(fc);

    // read links....

    FILE *fl = fopen(file_links, "r");
    if (fl == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(fl);
        return;
    }
    while (!feof(fl))
    {
        fgets(buffer, sizeof(buffer), fl);

        sscanf(buffer, "%c", &bin[0]);
        if (bin[0] != '.')
        {
            sscanf(buffer, "%d%c%d", &a, &bin[0], &b);
            g->aretes[i].start = a - 3;
            g->aretes[i].end = b - 3; //-3 to count after max and min
            g->aretes[i].weight = NO_WEIGHT;
            g->nb_arete = i + 1;
            i++;
            if (i >= g->a)
            {
                fprintf(stderr, "graph arete array is too small\n");
                fclose(fl);
                return;
            }
        }
        else
        { // bin[0] == 'd' ; create a double arete
            sscanf(buffer, "%c%d%c%d", &bin[0], &a, &bin[0], &b);
            g->aretes[i].start = a - 3;
            g->aretes[i].end = b - 3; //-3 to count after max and min
            g->aretes[i].weight = NO_WEIGHT;
            g->nb_arete = i + 1;
            i++;
            if (i >= g->a)
            {
                fprintf(stderr, "graph arete array is too small\n");
                fclose(fl);
                return;
            }
            g->aretes[i].start = b - 3;
            g->aretes[i].end = a - 3; //-3 to count after max and min
            g->aretes[i].weight = NO_WEIGHT;
            g->nb_arete = i + 1;
            i++;
            if (i >= g->a)
            {
                fprintf(stderr, "graph arete array is too small\n");
                fclose(fl);
                return;
            }
        }
    }
    fclose(fl);
    // printf("%f\n", ymax-ymin);
}

void changeCenter(double zoom, Graph*g, double old_center_x, double old_center_y, double new_center_x, double new_center_y){
    double x, y;
    for(int i = 0 ; i < g->nb_vertex;i++){
        x = cos(g->vertexs[i].angle) * g->vertexs[i].distance*zoom;
        y = sin(g->vertexs[i].angle) * g->vertexs[i].distance*zoom;
        g->vertexs[i].distance = dist(new_center_x, new_center_y ,old_center_x + x, old_center_y + y)/zoom;
        g->vertexs[i].angle = atan2(new_center_y - (old_center_y + y), new_center_x - (old_center_x + x)) + 3.1415;

    }

}





/*
int linkByClick(const char *file_links, Graph *g, double x1, double y1, double x2, double y2, int doublelink, int edge_x, int edge_y, int width, int height)
{
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
        return 0;
    int s = -1, e = -1;

    for (int i = 0; i < g->nb_vertex && s == -1; i++)
        if (dist(x1, y1, edge_x + (g->vertexs[i].x) * width / WIDTH, edge_y + (g->vertexs[i].y) * height / HEIGHT) < VERTEX_SIZE)
            s = i;

    for (int i = 0; i < g->nb_vertex && s != -1 && e == -1; i++)
        if (dist(x2, y2, edge_x + (g->vertexs[i].x) * width / WIDTH, edge_y + (g->vertexs[i].y) * height / HEIGHT) < VERTEX_SIZE)
            e = i;

    if (s == -1 || e == -1) // no clicks in thee vertexs
        return 0;

    FILE *f = fopen(file_links, "a");
    if (f == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(f);

        return 0;
    }

    printf("adding arete between %d and %d \n", s, e);
    addArete(g, g->nb_arete, s, e, NO_WEIGHT);

    if (doublelink)
    {
        addArete(g, g->nb_arete, e, s, NO_WEIGHT);

        fprintf(f, "%s", "."); // double way arrow signature
    }
    fprintf(f, "%d %d\n", s + 3, e + 3);
    fclose(f);
    return 1; // sucess
}*/
/*
int createVertex(const char* file_coordinates, Graph*g, double cx, double cy, double _w, double _h, double x, double y){
    //first let's know the min and max value :
    FILE *fc = fopen(file_coordinates, "r");
    if (fc == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(fc);
        return 0;
    }

    // first know the min/max value
    double xmin, xmax, ymin, ymax;
    char buffer[128];
    char bin[16];
    double _x, _y;
    int a, b;

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MIN") != 0)
        fprintf(stderr, "no min value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmin, bin, &ymin);

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MAX") != 0)
        fprintf(stderr, "no max value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmax, bin, &ymax);

    _x = fmin(xmin, xmax);
    _y = fmax(xmin, xmax);
    xmin = _x;
    xmax = _y;
    _x = fmin(ymin, ymax);
    _y = fmax(ymin, ymax);
    ymin = _x;
    ymax = _y;
    printf("(%f, %f)\t(%f %f)\n",xmin, ymin, xmax, ymax);
    fclose(fc);


    //add vertex on the actual graph :

    //transform x and y into coordinates
    double coordx, coordy, aa, bb;

    aa= (x - cx)/(double)_w;
    bb = (y - cy)/(double)_h;
    addVertex(g, g->nb_vertex - 1, aa*WIDTH, bb*HEIGHT, NO_COLOR);

    coordx = xmin + aa*(xmax - xmin) * (_h/WIDTH);
    coordy = ymin + bb*(ymax - ymin) * (_w/HEIGHT);

    //write the coordinates in the file :
    FILE*fr = fopen(file_coordinates, "a");
    fprintf(fr, "%lf , %lf\n", coordx, coordy);



    fclose(fr);
    return 1;
}*/