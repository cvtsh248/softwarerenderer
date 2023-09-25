if [[ "$OSTYPE" == "darwin"* ]]; then
    g++ main.cpp 3DLib/Render3D.cpp 3DLib/graphicsmath.cpp -std=c++17 -F/Library/Frameworks -framework SDL2
    ./a.out
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then 
    g++ main.cpp 3DLib/Render3D.cpp 3DLib/graphicsmath.cpp -w -lSDL2
    ./a.out
fi