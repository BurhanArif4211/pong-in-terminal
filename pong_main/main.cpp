#include <iostream> 
#include <conio.h>  // For _kbhit() and _getch() (non-blocking input)
#include <windows.h>  // For Sleep(), clearing screen, hiding cursor, and setting cursor position
#include "menu.h"
using namespace std;

const HANDLE StdH = GetStdHandle(STD_OUTPUT_HANDLE);// this is a HANDLE used to refer to the windows terminal

// Customizable parameters
const int SCREEN_WIDTH = 40;   // Width of the screen (columns)
const int SCREEN_HEIGHT = 20;  // Height of the screen (rows)
const int PADDLE_HEIGHT = 6;  // Height of the paddles
////// Frame rate control: number of frames to skip for slowing down the ball
int frameSkip = 5; // (MUST BE ODD) Skip 2 frame(s) every 3 iterations of the game loop 
int LFrameSkip = 3;
int RFrameSkip = 3;


// Initial positions of the paddles and ball
int ballX = SCREEN_WIDTH / 2, ballY = SCREEN_HEIGHT / 2;
int ballDirectionX = 1, ballDirectionY = 1;

int leftY = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, //center of left paddle
rightY = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2; // Center of right paddle

int scoreLeft = 0, scoreRight = 0;

// Function to hide the cursor to avoid jumping cursor problem
static void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;  // Maximum size
    cursorInfo.bVisible = FALSE;  // Make the cursor invisible
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Function to set the cursor position (at every frame start this is called to avoid scrolling)
static void setCursorPosition(COORD coord) {
    //COORD coord;
    //coord.X = x;
    //coord.Y = y;
    SetConsoleCursorPosition(StdH, coord); //Windows API function to change cursor position
}

static void emptyScreen(char screenToEmpty[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            screenToEmpty[y][x] = ' ';  // Fill with empty space
        }
    }
}

// Function to print the game state using a buffered approach
static void printGame(char screen[SCREEN_HEIGHT][SCREEN_WIDTH], int scoreLeft, int scoreRight) {

    // Move the cursor to the top of the screen before printing the next frame
    setCursorPosition({ 0, 0 });// providing X and Y for COORD coord in setCursorPosition();

    // Print the game state to the console all at once
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (screen[y][x] == 'O') {
                SetConsoleTextAttribute(StdH, BALL_COLOR);
                cout << screen[y][x];
                SetConsoleTextAttribute(StdH, 15);
                
            }
            else if (screen[y][x] == ':') {
                SetConsoleTextAttribute(StdH, LEFT_COLOR);
                cout << screen[y][x];
                SetConsoleTextAttribute(StdH, 15);
            }
            else if (screen[y][x] == ';') {
                SetConsoleTextAttribute(StdH, RIGHT_COLOR);
                cout << screen[y][x];
                SetConsoleTextAttribute(StdH, 15);
            }
            else {
                cout << screen[y][x];
            }
        }
        cout << endl;
    }
    /*
    The different color codes are
    0   BLACK
    1   BLUE
    2   GREEN
    3   CYAN
    4   RED
    5   MAGENTA
    6   BROWN
    7   LIGHTGRAY
    8   DARKGRAY
    9   LIGHTBLUE
    10  LIGHTGREEN
    11  LIGHTCYAN
    12  LIGHTRED
    13  LIGHTMAGENTA
    14  YELLOW
    15  WHITE
    */

    // Print the score
    cout << "----------------Score-------------------" << endl;
    SetConsoleTextAttribute(StdH, LEFT_COLOR);//set color to LEFT_COLOR
    cout << "-----------Left ";

    cout << scoreLeft;
    SetConsoleTextAttribute(StdH, RIGHT_COLOR); //set color to RED_COLOR

    cout << " - Right ";
    cout << scoreRight << "-------------" << endl;
    SetConsoleTextAttribute(StdH, 15); //reset the rest of program color to white
}

int main() {
    ///////////////////////////
    
    ///////////////////////////
    // Hide the cursor to reduce flicker
    hideCursor();
    //Call the Main Menu function from menu.h
    InitiateMainMenu();
    // Create a screen buffer to hold the game state (this reduces lag)
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH]; // Screen buffer for the game area
    int frameCount = 0;
    // Game loop
    while (true) {
        switch (game_difficulty) {
        case EASY:
            frameSkip = 7;
            LFrameSkip = 6;
            RFrameSkip = 6;
            break;
        case NORMAL:
            frameSkip = 5;
            LFrameSkip = 4;
            RFrameSkip = 4;
            break;
        case HARD:
            frameSkip = 3;
            LFrameSkip = 2;
            RFrameSkip = 2;
            break;
        case IMP:
            frameSkip = 1;
            LFrameSkip = 2;
            RFrameSkip = 2;
            break;
        }
            // Initialize the screen buffer to empty spaces
            emptyScreen(screen);
            // Draw the borders
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                screen[0][x] = '-';  // Draw Top border
                screen[SCREEN_HEIGHT - 1][x] = '-'; // Draw Bottom border
            }

            // Draw the paddles and ball in the buffer
            for (int y = 0; y < PADDLE_HEIGHT; ++y) {
                if (leftY + y < SCREEN_HEIGHT) {
                    screen[leftY + y][2] = ':';  // Left paddle
                }
                if (rightY + y < SCREEN_HEIGHT) {
                    screen[rightY + y][SCREEN_WIDTH - 3] = ';';  // Right paddle
                }
                
            }
            screen[ballY][ballX] = 'O';  // Ball

            // Print the buffered screen
            printGame(screen, scoreLeft, scoreRight);
            // Skip frames to make the ball move slower
            if (frameCount % frameSkip == 0) {
                // Move the ball
                ballX += ballDirectionX;
                ballY += ballDirectionY;
            }
            // Ball collision with top/bottom walls
            if (ballY <= 0 || ballY >= SCREEN_HEIGHT - 1) {
                ballDirectionY = -ballDirectionY; // Reverse vertical direction
            }

            // Ball collision with paddles
            if (ballX == 3 && ballY >= leftY && ballY < leftY + PADDLE_HEIGHT) {
                ballDirectionX = -ballDirectionX; // Reverse horizontal direction
            }
            if (ballX == SCREEN_WIDTH - 4 && ballY >= rightY && ballY < rightY + PADDLE_HEIGHT) {
                ballDirectionX = -ballDirectionX; // Reverse horizontal direction
            }

            // Ball out of bounds (scoring)
            if (ballX <= 0) {
                scoreRight++;
                ballX = SCREEN_WIDTH / 2; ballY = SCREEN_HEIGHT / 2; // Reset ball to the center
                ballDirectionX = -ballDirectionX;
            }
            if (ballX >= SCREEN_WIDTH - 1) {
                scoreLeft++;
                ballX = SCREEN_WIDTH / 2; ballY = SCREEN_HEIGHT / 2; // Reset ball to the center
                ballDirectionX = -ballDirectionX;
            }
        

        // Move paddles based on user input
        if (_kbhit()) {  // Check if a key is pressed
            char key = toupper(_getch());  // Get the pressed key
            if (game_mode != ZERO_PLAYER) {
                // Left paddle controls
                if (key == 'W' && leftY > 0) {
                    leftY--;
                }
                if (key == 'S' && leftY < SCREEN_HEIGHT - PADDLE_HEIGHT) {
                    leftY++;
                }
            }
            if (game_mode != ZERO_PLAYER && game_mode == TWO_PLAYER) {
                // Right paddle controls
                if (key == 'I' && rightY > 0) {
                    rightY--;
                }
                if (key == 'K' && rightY < SCREEN_HEIGHT - PADDLE_HEIGHT) {
                    rightY++;
                }
            }
        }

        /// Implementation of AI By Burhan:
        if (game_mode==ZERO_PLAYER) {
            // AI for the left paddle 
            if (frameCount % LFrameSkip == 0) {
                if (ballDirectionX == -1) {  // If the ball is moving towards the left paddle
                    if (ballY < leftY + PADDLE_HEIGHT / 2 && leftY > 0) {
                        leftY--; // Move paddle up
                    }
                    if (ballY > leftY + PADDLE_HEIGHT / 2 && leftY < SCREEN_HEIGHT - PADDLE_HEIGHT) {
                        leftY++; // Move paddle down
                    }
                }
            }
        }
        if (game_mode == ZERO_PLAYER || game_mode == SINGLE_PLAYER) {
            // AI for the right paddle (opponent)
            if (frameCount % RFrameSkip == 0) {
                if (ballDirectionX == 1) {  // If the ball is moving towards the right paddle
                    if (ballY < rightY + PADDLE_HEIGHT / 2 && rightY > 0) {
                        rightY--; // Move paddle up
                    }
                    if (ballY > rightY + PADDLE_HEIGHT / 2 && rightY < SCREEN_HEIGHT - PADDLE_HEIGHT) {
                        rightY++; // Move paddle down
                    }
                }
            }
        }

        // Sleep to control game speed (in milliseconds) and adjust frame rate
        Sleep(3);  // Adding delay to achieve approximately 60 FPS
        frameCount++;  // for frame skipping (changing ball speed)
    }
    return 0;
}

