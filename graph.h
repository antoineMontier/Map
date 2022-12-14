#define NO_COLOR -1
#define NO_WEIGHT -1
#define NB_COLOR 15
#define VERTEX_SIZE 2

#include "SDL_Basics.h"

typedef struct
{
    int id; // position in the array
    double distance;
    double angle;
    int card;
    int color;
} Vertex;

typedef struct
{
    int start;
    int end;
    double weight;
    int color;
} Arete;

typedef struct
{
    int nb_vertex;
    int nb_arete;
    int v, a;        // max size
    Vertex *vertexs; // size v
    Arete *aretes;   // size a
} Graph;

void createGraph(Graph *, int v, int a);
void destructGraph(Graph *);
void addVertex(Graph *g, int id, double distance, double angle, int color);
void addArete(Graph *g, int start_id, int end_id, double weight, int color);
void printGraph(Graph *g);
void printVertex(Vertex *v);
void printArete(Arete *a);
void createCompleteGraph(Graph *g, int n);
void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp, SDL_Color *c, int artificial_center_x, int artificial_center_y, double zoom_power);
SDL_Color* initialiseColors();
void weightAsDistance(Graph*g);//not sure on this one...
void createCoordinatesSystem(const char *file_coord, const char *file_links, Graph *g, double cx, double cy);
int linkByClick(SDL_Renderer *r, const char *file_links, Graph *g, double artificial_center_x, double artificial_center_y, double zoom, double x1, double y1, double x2, double y2, int doublelink);
int createVertex(const char* file_coordinates, Graph*g, double cx, double cy, double _w, double _h, double x, double y);
void changeCenter(double zoom, Graph*g, double old_center_x, double old_center_y, double new_center_x, double new_center_y);
void colorate_welsh_and_powell(Graph*g);
void neightbours(Graph *g, int vertex, int *vertexTable, int*areteTable);
int areteBetween(Graph*g, int start_id, int end_id);
void shortestPath(Graph*g, int vertex_start_id, int vertex_end_id);
void modify_osm_file(const char *filename);
