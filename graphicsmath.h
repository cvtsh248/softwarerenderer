#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>

#ifndef GRAPHICSMATH_H
#define GRAPHICSMATH_H

struct Matrix_44{
    float a[4];
    float b[4];
    float c[4];
    float d[4];
};

struct Point3D{ 
    float x; 
    float y; 
    float z;
};

std::vector<float> Multiply(struct Matrix_44 &transform, std::vector<float> &coord);

std::vector<float> P3DTo4DVec(struct Point3D &a);

std::vector<float> Normalise(std::vector<float> &a);

float Dot(std::vector<float> &a, std::vector<float> &b);

std::vector<float> Cross(std::vector<float> &a, std::vector<float> &b);

std::vector<float> SubtractVec(std::vector<float> &a, std::vector<float> &b);

std::vector<float> AddVec(std::vector<float> &a, std::vector<float> &b);

float Dot(std::vector<float> &a, std::vector<float> &b);

#endif