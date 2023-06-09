#include <string.h>
#include "include/raylib.h"
#include "processInput.c"
//IMAGES
#include "include/avatar.h"
#include "include/bottomborder.h"
#include "include/rightborder.h"

// MODIFIED RAYLIB FUNCTIONS FOR ALTERED \n PROPERTIES
void DrawTextEx2(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint); 
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);   // Draw text using font inside rectangle limits
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);    // Draw text using font inside rectangle limits with support for text selection 

Texture2D PlayerAvatar;
Texture2D WindowRightBorder;
Texture2D WindowBottomBorder;

#define MAX_INPUT_CHARS 55
#define MAX_OUTPUT_CHARS 1750

int main(void)
{   
    // WINDOW CHARACTERISTICS INITIALISATION
    int screenWidth = 720;
    int screenHeight = 520;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(screenWidth, screenHeight, "gooey text adventure prototype");

    // DEFINING INPUT & OUTPUT CHARACTER LIMITS
    char userInput[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
    int letterCount = 0;
    char output[MAX_OUTPUT_CHARS + 1];

    for (int i = 0; i < MAX_OUTPUT_CHARS; i++) output[i] = 'e';
    for (int i = 800; i < MAX_OUTPUT_CHARS; i++) output[i] = 'a';
    output[400] = '\0';  // MAX CHARACTERS SHOULD BE ABOUT 1700

    bool wordWrap = true;

    //                   { X axis, Y axis, box width, box height}
    Rectangle inputBox = {10, screenHeight - 40, screenWidth, 40 };
    Vector2 inputBoxTextPos = {inputBox.x, inputBox.y};
    Rectangle outputBox = {25, 25, screenWidth - 200, screenHeight - 200 };
    // Vector2 outputBoxTextPos = {outputBox.x, outputBox.y};
    Rectangle directionBox = {10, screenHeight - 160, 40, 60};

    // ANOTHER DYNAMIC RECTANGLE NEEDED TO OBSCURE "OFF-SCREEN" TEXT. AND ANOTHER TO CREATE THE ILLUSION OF A BORDER ON THE RIGHT SIDE.

    // LOADING FONT
    Font font = LoadFont("include/pixelplay.png");

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
    int keyPressTimer = 0;          // IN ORDER TO MAKE BACKSPACE FEEL NATURAL
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key CHANGE ESC KEY TO SOMETHING AKIN TO START
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

        if (IsKeyPressed(KEY_ENTER))
        {    
            returnFeedback(userInput, output);  // FUNCTION FROM processInput.c

            // int stringSize = sizeof(feedback);  // DETERMINE HOW MANY CHARACTERS NECESSARY TO DELETE TO ACCOMODATE FEEDBACK IN output[]
            
            // printf("stringSize = %d", stringSize);

            // for (int i = 0; i <= MAX_OUTPUT_CHARS; i++) {
            //     output[i] = output[i + stringSize];  // OVERWRITING PREVIOUS INDICES WITH nth INDICES
            //     if ((output[i + stringSize]) == '\0') break;
            // }

            // strcat(output, feedback);

            // OUTPUT IS RETURNED TO A LIST WHICH IS ALREADY FULL OF SPACES. EVERY NEW ENTRY DELETES 
            // (edit) THE NUMBER OF LINES NECESSARY TO FIT THE OUTPUT ON 

            // EVERY LINE OF OUTPUT IS MEASURED, THE NUMBER OF LINES REQUIRED IS CALCULATED AND IT IS FED INTO THE BOTTOM ARRAY OF THE 2D ARRAY.
            // THE PREVIOUS ARRAY OF VALUES HAS ITS INDEX REDUCED BY ONE, AND THEREFORE MOVES UP THE 2D ARRAY. THIS HAPPENS FOR EVERY ROW, EXCEPT!
            // THE VERY TOP ONE WHICH IS SIMPLY OVERWRITTEN BY THE VALUES OF THE ARRAY BELOW IT. 
            // IF THE USER WOULD LIKE TO KEEP AN ADVENTURE LOG, AN OPTION TO CREATE AN EXTERNAL TXT FILE. SIMPLY APPEND EVERY INSTANCE OF OUTPUT
            // TO THIS FILE. 
            
            // DELETE STRING
            letterCount = 0;
            userInput[letterCount] = '\0';
        }

        // MAKING BACKSPACE FEEL MORE NATURAL ----------------------------------------------------------------
        if (IsKeyPressed(KEY_BACKSPACE)) 
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            userInput[letterCount] = '\0';
        }
        if (IsKeyDown(KEY_BACKSPACE))
        {
            keyPressTimer++;
            if (keyPressTimer > 16) {
                letterCount--;
            if (letterCount < 0) letterCount = 0;
            userInput[letterCount] = '\0';
            }
        }
        if (IsKeyReleased(KEY_BACKSPACE)) 
        {
            keyPressTimer = 0;
        }

        // DRAWING ----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            screenWidth = GetScreenWidth();  // ALLOWS THE BELOW ELEMENTS TO ADJUST DIMENSIONS/POSITION AFTER SCREEN RESIZING
            screenHeight = GetScreenHeight();

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

            DrawTextEx2(font, userInput, inputBoxTextPos, 33.0, 0.3, BLACK);
            // DrawTextEx2(font, output, outputBoxTextPos, 33.0, 0.3, BLACK);
            DrawTextBoxed(font, output, (Rectangle){ outputBox.x + 4, outputBox.y + 4, outputBox.width - 4, outputBox.height - 4 }, 33.0f, 0.03f, wordWrap, BLACK);

            
            // if (letterCount < MAX_INPUT_CHARS)  // COULD REINCORPORATE FRAMESCOUNTER FOR THE SAKE OF BLINKING UNDERSCOR.
            //                                     // COULD USE IT IN MY RNG ... POSSIBLY. IF FRAME ON CERTAIN NUMBER THEN CRIT GUARANTEED. ALMOST LIKE AN ELECTRIC WIND GOD FIST
            // {
            //     // Draw blinking underscore char
            //     if (((framesCounter/20)%2) == 0) DrawText("_", (int)inputBox.x + 8 + MeasureText(userInput, 40), (int)inputBox.y + 12, 40, MAROON);
            // }
            // else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);

        EndDrawing();
        
    }
    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
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

void appendToOutput(char *pointerToOutput, char *feedback) 
{
    ;
}

void DrawTextEx2(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint)
{
    if (font.texture.id == 0) font = GetFontDefault();  // Security check in case of not valid font

    int size = TextLength(text);    // Total size in bytes of the text, scanned by codepoints in loop

    int textOffsetY = 0;            // Offset between lines (on linebreak '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/font.baseSize;         // Character quad scaling factor

    for (int i = 0; i < size;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepointNext(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += (int)((font.baseSize + font.baseSize/100.0f)*scaleFactor);
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint(font, codepoint, (Vector2){ position.x + textOffsetX, position.y + textOffsetY }, fontSize, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += ((float)font.recs[index].width*scaleFactor + spacing);
            else textOffsetX += ((float)font.glyphs[index].advanceX*scaleFactor + spacing);
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
{
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop, checks for '\0' ending

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = MEASURE_STATE;  // CHANGED THIS LINE FROM ORIGINAL 11/05/2023

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);  // GETS ASCII VALUE OF CORRESPONDING CHARACTER IN text
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1)? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n') state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + font.baseSize/100)*scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize/100)*scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize*scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec((Rectangle){ rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize*scaleFactor }, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + font.baseSize/100)*scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
    }
}