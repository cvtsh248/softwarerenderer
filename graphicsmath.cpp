#include "graphicsmath.h"

std::vector<float> Multiply(struct Matrix_44 &transform, std::vector<float> &coord){
    return std::vector<float> {coord[0]*transform.a[0] + coord[1]*transform.a[1] + coord[2]*transform.a[2] + coord[3]*transform.a[3],
                               coord[0]*transform.b[0] + coord[1]*transform.b[1] + coord[2]*transform.b[2] + coord[3]*transform.b[3],
                               coord[0]*transform.c[0] + coord[1]*transform.c[1] + coord[2]*transform.c[2] + coord[3]*transform.c[3],
                               coord[0]*transform.d[0] + coord[1]*transform.d[1] + coord[2]*transform.d[2] + coord[3]*transform.d[3]};
}

std::vector<float> Normalise(std::vector<float> &a){
    return std::vector<float> {a[0]/(std::sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2])), a[1]/(std::sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2])), a[2]/(std::sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]))};
}

std::vector<float> Cross(std::vector<float> &a, std::vector<float> &b){ // 3D vector ONLY
    return std::vector<float> {a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]};
}

float Dot(std::vector<float> &a, std::vector<float> &b){ // 3D vector ONLY
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

std::vector<float> SubtractVec(std::vector<float> &a, std::vector<float> &b){
    return std::vector<float> {a[0]-b[0], a[1]-b[1], a[2]-b[2]};
}

std::vector<float> AddVec(std::vector<float> &a, std::vector<float> &b){
    return std::vector<float> {a[0]+b[0], a[1]+b[1], a[2]+b[2]};
}

std::vector<float> P3DTo4DVec(struct Point3D &a){
    return std::vector<float> {a.x, a.y, a.z, 1};
}

std::vector<int> lerpInt(int &a, int &b){
    std::vector<int> buffer;
    // float a = (float)a_;
    // float b = (float)b_;
    for (float i = 0; i < b-a+1; i++){
        //buffer.push_back((int)(a*((b-a-i)/(b-a))+(b*i)/(b-a)));
        buffer.push_back((int)(a+i));
    }
    return buffer;
}

std::vector<std::vector<int>> lerpBilinear(std::vector<int> &a, std::vector<int> &b){
    std::vector<std::vector<int>> buffer;
    float gradient = ((float)(b[1]-a[1])/(float)(b[0]-a[0]));
    float constant = (float)a[1]-(float)a[0]*(gradient);

    float x = (float)a[0];

    if ((b[0]-a[0]) > 0){
        while (x != b[0]){
            float y = gradient*x + constant;
            std::vector<int> buf {(int)x,(int)y};
            buffer.push_back(buf);
            x++;
        }
    } else if ((b[0]-a[0]) < 0) {
        while (x != b[0]){
            float y = gradient*x + constant;
            std::vector<int> buf {(int)std::round(x),(int)std::round(y)};
            buffer.push_back(buf);
            x--;
        }
    }

    return buffer;
}