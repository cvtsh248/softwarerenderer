#include "Render3D.h"

// Methods

Object3D loadObj(std::string filename){

    std::string mtl = filename+".mtl";
    filename = filename+".obj";

    Object3D Out;
    std::ifstream objfile;
    objfile.open(filename);

    std::printf("Loading ");
    std::printf("%s\n",filename.c_str());
    if (objfile.is_open()){
        std::string buffer;
        std::vector<std::string> buf;
        while (std::getline(objfile, buffer)){
            std::stringstream ss(buffer);
            std::string s;
            int i = 0;
            int letter_slash = buffer.find('/');
            if (buffer[0] == *"v" && buffer[1] != *"t" && buffer[1] != *"n"){ // Appending vertices
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        pointBuffer.push_back(std::stof(s));
                    }
                    i++;
                }
                Out.vertices.push_back(Point3D{pointBuffer[0], pointBuffer[1], pointBuffer[2]});
            }
            else if (buffer[0] == *"f" && buffer[letter_slash] != '/'){ // Appending faces
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        pointBuffer.push_back(std::stoi(s));
                    }
                    i++;
                }
                Out.tris.push_back(Point3D{pointBuffer[0], pointBuffer[1], pointBuffer[2]});
            }
            else if (buffer[0] == *"f" && buffer[letter_slash] == '/'){
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        //pointBuffer.push_back(std::stof(s));
                        std::stringstream ss_(s);
                        std::string s_;
                        int n = 0;
                        while (std::getline(ss_, s_, '/')){
                            if (n == 0){
                                pointBuffer.push_back(std::stoi(s_));
                            }
                            n++;
                        }
                    }
                    i++;
                }
                Out.tris.push_back(Point3D{pointBuffer[0], pointBuffer[1], pointBuffer[2]});
            }
            else if(buffer[0] == *"v" && buffer[1] == *"n"){
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        pointBuffer.push_back(std::stoi(s));
                    }
                    i++;
                }
                Out.normals.push_back(pointBuffer);
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

    std::ifstream objfile_;
    objfile_.open(mtl);
    if (objfile_.is_open()){
        std::string buffer;
        std::vector<std::string> buf;
        while (std::getline(objfile_, buffer)){
            std::stringstream ss(buffer);
            std::string s;
            int i = 0;
            if (buffer[0] == *"K" && buffer[1] == *"d"){ // Appending diffuse colour
                std::vector<float> pointBuffer;
                while (std::getline(ss, s, ' ')) {
                    if (i > 0){
                        pointBuffer.push_back(std::stof(s));
                    }
                    i++;
                }
                Out.DifCol = pointBuffer;
            }
        }
    }
    return Out;

}

Render3D::Render3D(float &fps, float &width, float &height, float &near, float &far, float &fov, Point3D &cpos, std::vector<Object3D> &obj){
    //objects[0].vertices;
    imgw = width;
    imgh = height;

    FPS = fps;

    cameraPos = cpos;

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
        }

    }
    
    SDL_RenderPresent(renderer); 

}

void Render3D::RenderTris(SDL_Window *window, SDL_Renderer *renderer){

    int tick = SDL_GetTicks();
    std::vector<std::vector<float>> projected;
    std::vector<std::vector<int>> pxy;
    std::vector<Point3D> buffer;
    std::vector<float> buffer_;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Project the vertices
    for (int i = 0; i < objects.size(); i++){
        buffer = {};
        buffer_ = {};
        projected = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
        pxy = {{0,0},{0,0},{0,0}};
        //Sort vertices first
        //objects[i].camdist = {};
        std::vector<float> camDist;
        for (int j = 0; j < objects[i].tri_v.size(); j+=3){
            float centroidX = (objects[i].tri_v[j].x + objects[i].tri_v[j+1].x + objects[i].tri_v[j+2].x)/3;
            float centroidY = (objects[i].tri_v[j].y + objects[i].tri_v[j+1].y + objects[i].tri_v[j+2].y)/3;
            float centroidZ = (objects[i].tri_v[j].z + objects[i].tri_v[j+1].z + objects[i].tri_v[j+2].z)/3;
            camDist.push_back((centroidX-cameraPos.x)*(centroidX-cameraPos.x) + (centroidY-cameraPos.y)*(centroidY-cameraPos.y) + (centroidZ-cameraPos.z)*(centroidZ-cameraPos.z));
        }

        objects[i].tri_v = sortTris(objects[i].tri_v, camDist);

        for (int j = 0; j < objects[i].tri_v.size(); j+=3){
            buffer = {objects[i].tri_v[j], objects[i].tri_v[j+1],objects[i].tri_v[j+2]};
            std::vector<float> vecA = {buffer[0].x-buffer[1].x, buffer[0].y-buffer[1].y, buffer[0].z-buffer[1].z};
            std::vector<float> vecB = {buffer[0].x-buffer[2].x, buffer[0].y-buffer[2].y, buffer[0].z-buffer[2].z};
            vecA = Normalise(vecA);
            vecB = Normalise(vecB);
            std::vector<float> crs = Cross(vecA, vecB);
            std::vector<float> camN = {buffer[0].x - cameraPos.x, buffer[0].y - cameraPos.y, buffer[0].z - cameraPos.z};
            std::vector<std::vector<int>> a;
            std::vector<std::vector<int>> b;

            std::vector<float> dirLight = {0.0f,0.0f,-1.0f};
            camN = Normalise(camN);
            dirLight = Normalise(dirLight);
            crs = Normalise(crs);
            float dt = Dot(crs, camN);
            float dl = Dot(crs, dirLight);
            if (-dt >= 0){
                for (int n = 0; n < 3; n++){
                        buffer_ = P3DTo4DVec(buffer[n]);
                        translation = {{1,0,0,-cameraPos.x},
                                    {0,1,0,-cameraPos.y},
                                    {0,0,1,-cameraPos.z},
                                    {0,0,0,1}};
                        projected[n] = Multiply(translation, buffer_);
                        projected[n] = Multiply(projection, projected[n]);
                        if (projected[n][3] != 0 && projected[n][3] != 1){
                            pxy[n][0] = std::round(((projected[n][0]/projected[n][3] + 1)*0.5*imgw));
                            pxy[n][1] = std::round(((1-(projected[n][1]/projected[n][3] + 1)*0.5)*imgh)); 
                        } else {
                            pxy[n][0] = std::round(((projected[n][0]+ 1)*imgw*0.5)); 
                            pxy[n][1] = std::round(((1-(projected[n][1]+1))*0.5*imgh));
                        }
                    }
                std::vector<float> col;
                if (objects[i].DifCol.size() > 0){
                    col = objects[i].DifCol;
                } else {
                    col = {1,1,1};
                }
                Uint8 shade = (Uint8)(std::abs(dl*255));
                Uint8 shadeR = (Uint8)((std::abs(dl*255))*col[0]);
                Uint8 shadeG = (Uint8)((std::abs(dl*255))*col[1]);
                Uint8 shadeB = (Uint8)((std::abs(dl*255))*col[2]);
                SDL_Vertex triangleVertex[3]=
                    {
                        {
                            { (float)pxy[0][0],(float)pxy[0][1]}, /* first point location */ 
                            { shadeR, shadeG, shadeB, 0xFF }, /* first color */ 
                            { 0.f, 0.f }
                        },
                        {
                            { (float)pxy[1][0], (float)pxy[1][1] }, /* second point location */ 
                            {shadeR, shadeG, shadeB, 0xFF }, /* second color */
                            { 0.f, 0.f }
                        },
                        {
                            { (float)pxy[2][0], (float)pxy[2][1] }, /* third point location */ 
                            { shadeR, shadeG, shadeB, 0xFF }, /* third color */
                            { 0.f, 0.f }
                        }
                    };
                SDL_RenderGeometry(renderer, NULL, triangleVertex, 3, NULL, 0);

                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                // SDL_RenderDrawLine(renderer, pxy[0][0],pxy[0][1],pxy[1][0],pxy[1][1]);
                // SDL_RenderDrawLine(renderer, pxy[1][0],pxy[1][1],pxy[2][0],pxy[2][1]);
                // SDL_RenderDrawLine(renderer, pxy[2][0],pxy[2][1],pxy[0][0],pxy[0][1]);
            }
        }

    }

    while (SDL_GetTicks() < tick + (1/FPS)*1000){
        //wait
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

    if (id > -1){
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
    } else if (id < 0){
        buffer = P3DTo4DVec(cameraPos);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        cameraPos = buf;
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

    if (id > -1){
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
    } else if (id < 0){
        buffer = P3DTo4DVec(cameraPos);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        cameraPos = buf;
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

    if (id > -1){
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
    } else if (id < 0){
        buffer = P3DTo4DVec(cameraPos);
        buffer = Multiply(n_translation, buffer);
        buffer = Multiply(rotation, buffer);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        cameraPos = buf;
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

    if (id > -1){
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
    } else if (id < 0){
        buffer = P3DTo4DVec(cameraPos);
        buffer = Multiply(translation, buffer);
        buf = {buffer[0], buffer[1], buffer[2]};
        cameraPos = buf;
    }
}