/****************************************************************************************************
*   [raylib-aseprite] example for Dreamcast - Load a Aseprite file, and control the animated sprite.
*
*   This example has been created using raylib 5.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Based in Example by Rob Loach (@RobLoach)
*
*   Copyright (c) 2021 Rob Loach (@RobLoach)
*   Copyright (c) 2024 Antonio Jose Ramos Marquez aka bigboss(@psxdev)
*
****************************************************************************************************/

#include <raylib.h>

//needed to use raylib-aseprite is a include only needed to use the whole features
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "raylib-aseprite.h"


#define ATTR_DREAMCAST_WIDTH 640
#define ATTR_DREAMCAST_HEIGHT 480

static bool done = false;
bool dpadLeftDown;
bool dpadRightDown;
bool dpadDownDown;
bool dpadUpDown;

static void updateController(void) 
{
    bool startPressed;

    if(!IsGamepadAvailable(0))
    {
        return;
    }

    startPressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    dpadLeftDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    dpadRightDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    dpadDownDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    dpadUpDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP);


    if(startPressed)
    {
        done = true;
    }
}

int main(int argc, char** argv) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = ATTR_DREAMCAST_WIDTH;
    const int screenHeight = ATTR_DREAMCAST_HEIGHT;

    InitWindow(screenWidth, screenHeight, "raylib aseprite dreamcast example");
    
    // Load the Aseprite file. Remember dreamacast only loves pow of 2 textures so export aseprite files with pow of 2 values
    Aseprite george = LoadAseprite("/rd/george64x64.aseprite");

    // Load the standing animations from the Aseprite tags.
    AsepriteTag down = LoadAsepriteTag(george, "Walk-Down");
    AsepriteTag left = LoadAsepriteTag(george, "Walk-Left");
    AsepriteTag right = LoadAsepriteTag(george, "Walk-Right");
    AsepriteTag up = LoadAsepriteTag(george, "Walk-Up");
    AsepriteTag* current = &down;

    // Center George on the screen. Scale and speed
    const float scale = 1;
    const float speed = 1;

    Vector2 position = {
        GetScreenWidth() / 2 - GetAsepriteWidth(george) / 2 * scale,
        GetScreenHeight() / 2 - GetAsepriteHeight(george) / 2 * scale
    };

    SetTargetFPS(60);     // Set our game to run at 60 frames-per-second



    //----------------------------------------------------------

    // Main game loop
    while(!done) {

        //update dreamcast controller
        updateController();

        //switch animation and position based on updated dpad values
        if (dpadUpDown) 
        {
            current = &up;
            position.y -= speed;
            current->paused = false;
        }
        else if (dpadRightDown)
        {
            current = &right;
            position.x += speed;
            current->paused = false;
        }
        else if (dpadDownDown) 
        {
            current = &down;
            position.y += speed;
            current->paused = false;
        }
        else if (dpadLeftDown) 
        {
            current = &left;
            position.x -= speed;
            current->paused = false;
        }
        else 
        {
            current->paused = true;
        }

        // Have George stop walking with two feet on the ground.
        if (current->paused) {
            SetAsepriteTagFrame(current, 1);
        }

        // Update the animation.
        UpdateAsepriteTag(current);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // Draw one frame of George only to see static frame 
            DrawAseprite(george, 0, 100, 100, WHITE);
            DrawAseprite(george, 4, 100, 150, WHITE);
            DrawAseprite(george, 8, 100, 200, WHITE);
            DrawAseprite(george, 12, 100, 250, WHITE);

            // Draw the walking animation.
            DrawAsepriteTagEx(*current, position, 0, scale, WHITE);

            const char* text = "Use dpad buttons to walk";
            DrawText(text, GetScreenWidth() / 2 - MeasureText(text, 20) / 2, GetScreenHeight() - 80, 20, GRAY);
            DrawFPS(10, 10);
        }
        EndDrawing();
          

        //-----------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadAseprite(george);    // Unload the Aseprite data.

    CloseWindow();     // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}





