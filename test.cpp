#include <iostream>
#include <vector>
#include <iterator>


struct Point3D{
    float x;
    float y;
    float z;
};

struct Object3D{
    std::vector<Point3D> vert;
};

class Test{
    public:
        std::vector<Point3D> vertices;
        Test(std::vector<Point3D> *vert){
            vertices = *vert;
        }
};

void egg(float &a){
    a=a+0.4;
}

int main(){
    std::vector<Point3D> vertices {Point3D{3,2,1}, Point3D{1,9,14}, Point3D{0.1,12,7}};
    Object3D ob;
    ob.vert = vertices;
    //std::vector<Point3D> *vertices = new std::vector<Point3D>();


    // Test test(&vertices);
    // std::cout<<test.vertices[1].z<<std::endl;
    //std::cout<<ob.vert<<std::endl;
    float b = 1;
    std::printf("%.6f", b);
    //std::printf("%.6f", ob.vert[1].x);

}