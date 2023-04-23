#include "include/raylib.h"
//IMAGES
#include "include/avatar.h"
#include "include/bottomborder.h"
#include "include/rightborder.h"

Texture2D PlayerAvatar;
Texture2D WindowRightBorder;
Texture2D WindowBottomBorder;

#define MAX_INPUT_CHARS 47

int main(void)
{   
    // WINDOW CHARACTERISTICS INITIALISATION
    int screenWidth = 720;
    int screenHeight = 520;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(screenWidth, screenHeight, "raylib [text] example - input box");

    // DEFINING INPUT & OUTPUT CHARACTER LIMITS
    char userInput[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;

    char output[1000 + 1]; 
    for (int i = 0; i < 1000; i++) output[i] = 'a';
    output[1000] = '\0';

    
    //                   { X axis, Y axis, box width, box height}
    Rectangle inputBox = {50, screenHeight - 40, screenWidth, 40 };
    Vector2 inputBoxTextPos = {inputBox.x, inputBox.y};
    Rectangle outputBox = {25, 25, screenWidth - 200, screenHeight - 200 };
    Vector2 outputBoxTextPos = {outputBox.x, outputBox.y};

    Rectangle directionBox = {10, screenHeight - 160, 40, 60};

    // ANOTHER DYNAMIC RECTANGLE NEEDED TO OBSCURE "OFF-SCREEN" TEXT. AND ANOTHER TO CREATE THE ILLUSION OF A BORDER ON THE RIGHT SIDE.

    // LOADING FONT
    Font pixelplay = LoadFont("include/pixelplay.png");

    // LOADING TEXTURES
    Image avatar = { 0 };
    avatar.format = AVATAR_FORMAT;
    avatar.height = AVATAR_HEIGHT;
    avatar.width = AVATAR_WIDTH;
    avatar.data = AVATAR_DATA;
    avatar.mipmaps = 1;
    PlayerAvatar = LoadTextureFromImage(avatar); 
    Vector2 playerPos = {screenWidth / 2, screenHeight / 2};
    
    Image rightBorder = { 0 };
    rightBorder.format = RIGHTBORDER_FORMAT;
    rightBorder.height = RIGHTBORDER_HEIGHT;
    rightBorder.width = RIGHTBORDER_WIDTH;
    rightBorder.data = RIGHTBORDER_DATA;
    rightBorder.mipmaps = 1;
    WindowRightBorder = LoadTextureFromImage(rightBorder); 
    Vector2 rightBorderPos = { screenWidth - 159, 0 };
    
    Image bottomBorder = { 0 };
    bottomBorder.format = BOTTOMBORDER_FORMAT;
    bottomBorder.height = BOTTOMBORDER_HEIGHT;
    bottomBorder.width = BOTTOMBORDER_WIDTH;
    bottomBorder.data = BOTTOMBORDER_DATA;
    bottomBorder.mipmaps = 1;
    WindowBottomBorder = LoadTextureFromImage(bottomBorder);
    Vector2 bottomBorderPos = { 230, screenHeight - 160 };  // y coordinate was 360

    SetTargetFPS(50);               // Set game to run at 50 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
            {
                userInput[letterCount] = (char)key;
                userInput[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyDown(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            userInput[letterCount] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            // MAKE A FUNCTION TO PROCESS THE ARRAY. RECEIVE AND DISPLAY OUTPUT THE OUTPUT. 


            // OUTPUT IS RETURNED TO A LIST WHICH IS ALREADY FULL OF SPACES. EVERY NEW ENTRY DELETES sizeof((char)output) OF SPACES AT 

            // EVERY LINE OF OUTPUT IS MEASURED, THE NUMBER OF LINES REQUIRED IS CALCULATED AND IT IS FED INTO THE BOTTOM ARRAY OF THE 2D ARRAY.
            // THE PREVIOUS ARRAY OF VALUES HAS ITS INDEX REDUCED BY ONE, AND THEREFORE MOVES UP THE 2D ARRAY. THIS HAPPENS FOR EVERY ROW, EXCEPT!
            // THE VERY TOP ONE WHICH IS SIMPLY OVERWRITTEN BY THE VALUES OF THE ARRAY BELOW IT. 
            // IF THE USER WOULD LIKE TO KEEP AN ADVENTURE LOG, AN OPTION TO CREATE AN EXTERNAL TXT FILE. SIMPLY APPEND EVERY INSTANCE OF OUTPUT
            // TO THIS FILE. 
            
            // DELETE STRING
            letterCount = 0;
            userInput[letterCount] = '\0';
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            screenWidth = GetScreenWidth();  // ALLOWS THE BELOW ELEMENTS TO ADJUST AFTER SCREEN RESIZING
            screenHeight = GetScreenHeight();

            DrawText("PLACE MOUSE OVER INPUT BOX!", 240, 140, 20, GRAY);
            DrawTexture(WindowRightBorder, screenWidth - 159, rightBorderPos.y, WHITE);
            DrawTexture(WindowBottomBorder, bottomBorderPos.x, screenHeight - 160, WHITE);
            DrawTexture(PlayerAvatar, playerPos.x, playerPos.y, WHITE);
            //CREATE FOLLOWING FUNCTION --> DrawTexture(determinePlayerAvatar(), playerPos.x, playerPos.y, WHITE); 

            
            DrawRectangleRec(inputBox, LIGHTGRAY);
            DrawRectangleRec(outputBox, LIGHTGRAY);
            DrawRectangleRec(directionBox, LIGHTGRAY);
            // DYNAMICALLY RESIZING/REPOSITIONING ELEMENTS
            inputBox.width = screenWidth;
            inputBox.y = screenHeight - 40;
            inputBoxTextPos.x = inputBox.x; inputBoxTextPos.y = inputBox.y;
            outputBox.width = screenWidth - 200;
            outputBox.height = screenHeight - 200;

            // DrawText(userInput, (int)inputBox.x + 5, (int)inputBox.y + 3, 30, MAROON);
            DrawTextEx(pixelplay, userInput, inputBoxTextPos, 33.0, 4.0, BLACK);
            DrawTextEx(pixelplay, output, outputBoxTextPos, 33.0, 4.0, BLACK);

            DrawText(TextFormat("INPUT\nCHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 315, 250, 8, DARKGRAY);

            
            // if (letterCount < MAX_INPUT_CHARS)
            // {
            //     // Draw blinking underscore char
            //     if (((framesCounter/20)%2) == 0) DrawText("|", (int)inputBox.x + 8 + MeasureText(userInput, 40), (int)inputBox.y + 12, 40, MAROON);
            // }
            // else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}