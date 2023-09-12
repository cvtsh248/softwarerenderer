#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <SDL2/SDL.h>
#include "Render3D.h"

float SCREEN_WIDTH = 640*1.5;
float SCREEN_HEIGHT = 480*1.5;

int main(){
    float near = 0.1;
    float far = 100;
    float fov = 0.1;
    float fps = 30;
    // Object3D Cube = {std::vector<Point3D> {Point3D{0,0,0},Point3D{1,0,0},Point3D{1,1,0},Point3D{0,1,0},Point3D{0,0,1},Point3D{1,0,1},Point3D{1,1,1},Point3D{0,1,1}},{},{}};
    Object3D Teapot = loadObj("teapot.obj");
    Object3D Cube = loadObj("cube.obj");
    Object3D Thingy = loadObj("weird.obj");
    std::vector<Object3D> objects = {Cube};
    SDL_Window* window = SDL_CreateWindow("3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Init(SDL_INIT_VIDEO);

    Point3D camPos = {0,0,0};

    Render3D R1(fps, (SCREEN_WIDTH), (SCREEN_HEIGHT), near, far, fov, camPos, objects);
    std::vector<float> tr = {0,-2,-2,0};
    std::vector<float> tr1 = {0.0,0.01,0.01,0};
    std::vector<float> tr2 = {0,-1,-6,0};
    std::vector<float> tr3 = {0,1,-8,0};

    Point3D neworig = {0,-1,-6};
    Point3D neworige = {0,-1,-8};
    R1.Translate(0, tr2);
    //R1.Translate(1, tr3);
    //R1.Translate(1, tr3);
    // for (int i = 0; i < 30; i++){
    //     R1.Translate(0,tr1);
    // }
    int quit = 0;
    while(!quit){
        SDL_Event event;
        //SDL_Delay(100);
        R1.RotateX(0, 0.01, neworig);
        R1.RotateY(0, 0.01, neworig);
        R1.RotateZ(0, 0.01, neworig);

        // R1.Translate(-1,tr1);
        
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);
        // R1.RenderVert(window, renderer);
        R1.RenderTris(window, renderer);
        //SDL_RenderPresent(renderer);  
        //SDL_RenderPresent(renderer);  
        while (SDL_PollEvent(&event)) {
            //SDL_Delay(1000);
            if (event.type == SDL_QUIT) {
                quit = 1;
                puts("QUIT!");
                break;
            }
        }
    }
    
}
