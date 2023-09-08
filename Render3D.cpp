#include "Render3D.h"

// Methods

Object3D loadObj(std::string filename){
    Object3D Out;
    std::ifstream objfile;
    objfile.open(filename);

    if (objfile.is_open()){
        std::string buffer;
        std::vector<std::string> buf;
        while (std::getline(objfile, buffer)){
            std::stringstream ss(buffer);
            std::string s;
            int i = 0;
            if (buffer[0] == *"v" && buffer[1] != *"t" && buffer[1] != *"n"){ // Appending vertices
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        pointBuffer.push_back(std::stof(s));
                    }
                    i += 1;
                }
                Out.vertices.push_back(Point3D{pointBuffer[0], pointBuffer[1], pointBuffer[2]});
            }
            else if (buffer[0] == *"f" && !buffer.find('f')){ // Appending faces
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        pointBuffer.push_back(std::stoi(s));
                    }
                    i += 1;
                }
                Out.tris.push_back(Point3D{pointBuffer[0], pointBuffer[1], pointBuffer[2]});
            }
            else if (buffer[0] == *"f" && buffer.find('f')){
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i == 2){
                        //pointBuffer.push_back(std::stof(s));
                        pointBuffer = {};
                        std::stringstream ss_(s);
                        std::string s_;
                        while (std::getline(ss_, s_, '/')){
                            pointBuffer.push_back(std::stoi(s_));
                            //std::printf("%d\n", std::stoi(s_));
                        }
                    }
                    i += 1;
                }
                Out.tris.push_back(Point3D{pointBuffer[0], pointBuffer[1], pointBuffer[2]});
            }
        }
    }
    for (int j = 0; j < Out.tris.size(); j++){
        Point3D a = Out.vertices[Out.tris[j].x-1];
        Point3D b = Out.vertices[Out.tris[j].y-1];
        Point3D c = Out.vertices[Out.tris[j].z-1];

        Out.tri_v.push_back(a);
        Out.tri_v.push_back(b);
        Out.tri_v.push_back(c);

    }

    return Out;

}

Render3D::Render3D(float &width, float &height, float &near, float &far, float &fov, std::vector<Object3D> &obj){
    //objects[0].vertices;
    imgw = width;
    imgh = height;

    aspectRatio = width/height;
    top = std::tan(fov/2);
    bottom = -top;
    right = top*aspectRatio;
    left = bottom;

    objects = obj;

    projection = {{2*near/(right-left),0,(right+left)/(right-left),0},
                  {0,2*near/(top-bottom),(top+bottom)/(top-bottom),0},
                  {0,0,-((far+near)/(far-near)),-((2*near*far)/(far-near))},
                  {0,0,-1,0}};
    
}

void Render3D::RenderVert(SDL_Window *window, SDL_Renderer *renderer){
    //std::printf("egg");

    std::vector<float> buffer;
    std::vector<float> projected;
    int px, py;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Project the vertices
    for (int i = 0; i < objects.size(); i++){
        buffer = {};
        projected = {};
        for (int j = 0; j < objects[i].vertices.size(); j++){ 
            buffer = P3DTo4DVec(objects[i].vertices[j]);
            projected = Multiply(projection, buffer);
            if (projected[3] != 0 && projected[3] != 1){
                px = std::round(((projected[0]/projected[3] + 1)*0.5*imgw));
                py = std::round(((1-(projected[1]/projected[3] + 1)*0.5)*imgh)); 
            } else {
                px = std::round(((projected[0]+ 1)*imgw*0.5)); 
                py = std::round(((1-(projected[1]+1))*0.5*imgh));
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, px, py);
            // std::printf("%d ",j);
            // std::printf("%d ",px);
            // std::printf("%d\n",py);
        }

    }
    SDL_RenderPresent(renderer);  

}

void Render3D::RenderTris(SDL_Window *window, SDL_Renderer *renderer){

    
    std::vector<std::vector<float>> projected;
    std::vector<std::vector<int>> pxy;
    std::vector<Point3D> buffer;
    std::vector<float> buffer_;
    int px, py;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Project the vertices
    for (int i = 0; i < objects.size(); i++){
        buffer = {};
        buffer_ = {};
        projected = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
        pxy = {{0,0},{0,0},{0,0}};
        for (int j = 0; j < objects[i].tri_v.size(); j+=3){
            //buffer = P3DTo4DVec(objects[i].vertices[j]);
            //projected = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
            //pxy = {{0,0},{0,0},{0,0}};
            buffer = {objects[i].tri_v[j], objects[i].tri_v[j+1],objects[i].tri_v[j+2]};
            for (int n = 0; n < 3; n++){
                buffer_ = P3DTo4DVec(buffer[n]);
                projected[n] = Multiply(projection, buffer_);
                if (projected[n][3] != 0 && projected[n][3] != 1){
                    pxy[n][0] = std::round(((projected[n][0]/projected[n][3] + 1)*0.5*imgw));
                    pxy[n][1] = std::round(((1-(projected[n][1]/projected[n][3] + 1)*0.5)*imgh)); 
                } else {
                    pxy[n][0] = std::round(((projected[n][0]+ 1)*imgw*0.5)); 
                    pxy[n][1] = std::round(((1-(projected[n][1]+1))*0.5*imgh));
                }
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, pxy[0][0],pxy[0][1],pxy[1][0],pxy[1][1]);
            SDL_RenderDrawLine(renderer, pxy[1][0],pxy[1][1],pxy[2][0],pxy[2][1]);
            SDL_RenderDrawLine(renderer, pxy[2][0],pxy[2][1],pxy[0][0],pxy[0][1]);
        }

    }
    SDL_RenderPresent(renderer);  
}

void Render3D::RotateX(int id, float rad, Point3D &origin){
    rotation = {{1,0,0,0},
                {0,std::cos(rad),-std::sin(rad),0},
                {0,std::sin(rad),std::cos(rad),0},
                {0,0,0,1}};
    
    Point3D buf;
    std::vector<float> buffer;

    translation = {{1,0,0,origin.x},
                   {0,1,0,origin.y},
                   {0,0,1,origin.z},
                   {0,0,0,1}};
    
    Matrix_44 n_translation = {{1,0,0,-origin.x},
                               {0,1,0,-origin.y},
                               {0,0,1,-origin.z},
                               {0,0,0,1}};

    for (int i = 0; i < objects[id].vertices.size(); i++){
        buffer = P3DTo4DVec(objects[id].vertices[i]);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].vertices[i] = buf;
    }

    for (int i = 0; i < objects[id].tri_v.size(); i++){
        buffer = P3DTo4DVec(objects[id].tri_v[i]);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].tri_v[i] = buf;
    }

}

void Render3D::RotateY(int id, float rad, Point3D &origin){
    rotation = {{std::cos(rad),0,std::sin(rad),0},
                {0,1,0,0},
                {-std::sin(rad),0,std::cos(rad),0},
                {0,0,0,1}};
    Point3D buf;
    std::vector<float> buffer;

    translation = {{1,0,0,origin.x},
                   {0,1,0,origin.y},
                   {0,0,1,origin.z},
                   {0,0,0,1}};
    
    Matrix_44 n_translation = {{1,0,0,-origin.x},
                               {0,1,0,-origin.y},
                               {0,0,1,-origin.z},
                               {0,0,0,1}};

    for (int i = 0; i < objects[id].vertices.size(); i++){
        buffer = P3DTo4DVec(objects[id].vertices[i]);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].vertices[i] = buf;
    }


    for (int i = 0; i < objects[id].tri_v.size(); i++){
        buffer = P3DTo4DVec(objects[id].tri_v[i]);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].tri_v[i] = buf;
    }
}

void Render3D::RotateZ(int id, float rad, Point3D &origin){
    rotation = {{std::cos(rad),-std::sin(rad),0,0},
                {std::sin(rad),std::cos(rad),0,0},
                {0,0,1,0},
                {0,0,0,1}};
    Point3D buf;
    std::vector<float> buffer;

    translation = {{1,0,0,origin.x},
                   {0,1,0,origin.y},
                   {0,0,1,origin.z},
                   {0,0,0,1}};
    
    Matrix_44 n_translation = {{1,0,0,-origin.x},
                               {0,1,0,-origin.y},
                               {0,0,1,-origin.z},
                               {0,0,0,1}};

    for (int i = 0; i < objects[id].vertices.size(); i++){
        buffer = P3DTo4DVec(objects[id].vertices[i]);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].vertices[i] = buf;
    }


    for (int i = 0; i < objects[id].tri_v.size(); i++){
        buffer = P3DTo4DVec(objects[id].tri_v[i]);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].tri_v[i] = buf;
    }
}

void QuaternionRot(int id, std::vector<float> axis){

}

void Render3D::Translate(int id, std::vector<float> &tr_vec){

    translation = {{1,0,0,tr_vec[0]},
                   {0,1,0,tr_vec[1]},
                   {0,0,1,tr_vec[2]},
                   {0,0,0,1}};


    Point3D buf;
    std::vector<float> buffer;

    for (int i = 0; i < objects[id].vertices.size(); i++){
        buffer = P3DTo4DVec(objects[id].vertices[i]);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].vertices[i] = buf;
    }

    for (int i = 0; i < objects[id].tri_v.size(); i++){
        buffer = P3DTo4DVec(objects[id].tri_v[i]);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        objects[id].tri_v[i] = buf;
    }
}


