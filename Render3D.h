#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <fstream>
#include <string>
#include <sstream> 
#include "graphicsmath.h"

#ifndef RENDER3D_H
#define RENDER3D_H

struct Object3D{
    std::vector<Point3D> vertices;
    std::vector<Point3D> tris;
    std::vector<Point3D> tri_v;
};

Object3D loadObj(std::string filename);

class Render3D{
    public:
        std::vector<Object3D> objects;

        Point3D cameraPos;
        float FPS;
        float fov; 
        float aspectRatio;
        float top;
        float bottom;
        float left;
        float right;
        float imgw;
        float imgh;

        Matrix_44 projection; // Projection matrix
        Matrix_44 rotation;
        Matrix_44 translation;

        Render3D(float &fps, float &width, float &height, float &near, float &far, float &fov, Point3D &cpos, std::vector<Object3D> &obj);

        void RenderVert(SDL_Window *window, SDL_Renderer *renderer);

        void RenderTris(SDL_Window *window, SDL_Renderer *renderer);

        void RotateX(int id, float rad, Point3D &origin);

        void RotateY(int id, float rad, Point3D &origin);

        void RotateZ(int id, float rad, Point3D &origin);

        void QuaternionRot(int id, std::vector<float> axis);

        void Translate(int id, std::vector<float> &tr_vec);

        
          
};

#endif