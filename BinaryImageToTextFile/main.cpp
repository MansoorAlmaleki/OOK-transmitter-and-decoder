#include "raylib.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int clamp(int value, int Min, int Max){
    if(value > Max){
        value = Max;
    }
    else if(value < Min){
        value = Min;
    }
    return value;
}

int main() {
    const int ImageSize = 32;
    const int screenWidth = 640;
    const int screenHeight = 640;
    const int PixelScaleFactor = screenWidth/ImageSize;

    bool ImageWritten = false;

    ofstream BinaryImageTextFile("output/BinaryImageText.txt");

    InitWindow(screenWidth, screenHeight, "Binary Image to text");

    SetTargetFPS(400);

    // creates a 32x32 vector where a vector with 32 rows is created and in each row a vector with a size of 32 is put inside which is the column
    // ImageGrid[x][y]
    // vector(elements, Value) the function takes two arguments, the elements needed and the value of which must be the vectors type (eg, int, float, vectors, objects, etc)
    vector<vector<int>> ImageGrid(ImageSize, vector<int>(ImageSize,0));

    while(!WindowShouldClose()){

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 MousePosition = GetMousePosition();
            ImageGrid[clamp(int(MousePosition.x/PixelScaleFactor), 0, ImageSize-1)][clamp(int(MousePosition.y/PixelScaleFactor), 0, ImageSize-1)] = 1;
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
            Vector2 MousePosition = GetMousePosition();
            ImageGrid[clamp(int(MousePosition.x/PixelScaleFactor), 0, ImageSize-1)][clamp(int(MousePosition.y/PixelScaleFactor), 0, ImageSize-1)] = 0;
        }

        if(IsKeyPressed(KEY_Q)){
            ImageGrid.assign(ImageSize, vector<int>(ImageSize,0));
        }

        //when done creating the image we output a file of the image data to be used in an MCU or similar that uses it in an array
        //to be encoded for the data transmission
        if(IsKeyPressed(KEY_SPACE) && ImageWritten == false){
            ImageWritten == true;
            int index = 0;
            for(int y = 0; y < ImageSize; y++){
                for(int x = 0; x < ImageSize; x++){
                    BinaryImageTextFile << ImageGrid[x][y];

                    if(index < (ImageSize*ImageSize)-1){
                       BinaryImageTextFile << ",";
                    }
                    index++;
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);
        Vector2 MousePosition = GetMousePosition();
        DrawRectangle(clamp(int(MousePosition.x/PixelScaleFactor), 0, ImageSize-1)*PixelScaleFactor, clamp(int(MousePosition.y/PixelScaleFactor), 0, ImageSize-1)*PixelScaleFactor, PixelScaleFactor, PixelScaleFactor, Color{255,255,255,120});

        for(int y = 0; y < ImageSize; y++){
            for(int x = 0; x < ImageSize; x++){
                if(ImageGrid[x][y] == 1){
                    DrawRectangle(x*PixelScaleFactor, y*PixelScaleFactor, PixelScaleFactor, PixelScaleFactor, WHITE);
                }
            }
        }


        EndDrawing();
    }
    CloseWindow();
    return 0;
}
