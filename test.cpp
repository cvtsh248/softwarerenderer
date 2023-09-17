#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>

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

bool compare_pair( const std::pair<float,std::vector<Point3D>> &pair1, 
                   const std::pair<float,std::vector<Point3D>> &pair2)
{
    if( pair1.first > pair2.first)
    {
        return true;
    } else {
        return false;
    }
}

std::vector<Point3D> sortTris(std::vector<Point3D> &triv, std::vector<double> &dist){
    std::vector<Point3D> output(triv.size());
    std::pair<double, std::vector<Point3D>> distPair[dist.size()];
    for (int i = 0; i < triv.size(); i+=3){
        std::vector<Point3D> buffer = {triv[i], triv[i+1], triv[i+2]};
        distPair[i/3].first = dist[i];
        distPair[i/3].second = buffer;
        //printf("%d\n", i/3);
    }
    // Sort pair

    size_t length = sizeof(distPair)/sizeof(distPair[0]);
    std::sort(distPair, distPair+length, compare_pair);
    for (int i = 0; i < triv.size(); i+=3){
        output[i] = distPair[i/3].second[0];
        output[i+1] = distPair[i/3].second[1];
        output[i+2] = distPair[i/3].second[2];
    }

    return output;

}

int main(){
    std::vector<Point3D> egg {Point3D{12,0,0},Point3D{11,0,0},Point3D{10,1,0},Point3D{9,1,0},Point3D{8,0,1},Point3D{7,0,1},Point3D{6,1,1},Point3D{5,1,1},Point3D{4,1,1}};
    std::vector<double> dist {1,5,2};

    egg = sortTris(egg, dist);
    for (int i = 0; i < egg.size(); i++){
        printf("%.6f\n", egg[i].x);
    }
    return 0;
}