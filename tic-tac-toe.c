//*****************************************************
// TIC-TAC-TOE V1 with some own AI
// 1.2.2024 Juha Koivukorpi
//*****************************************************

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define ToggleBoolean(b) ((b) ? (b = false) : (b = true));

COORD c = {0, 0};

const int MAX_WIDTH = 2;  // movable area max width
const int MAX_HEIGHT = 2; // movable area max height
const int GAME_X_SIZE = 3; // number of x blocks 
const int GAME_Y_SIZE = 3; // number of y blocks

bool difficultyEasy = false; // Start HARD difficulty level

// Structure(s)------------------------------------------------------
struct peliRuutu {
    int x;
    int y;
    char mark;
};

// Introduction ----------------------------------------------------
void setCursorXY (int, int); // Set cursor in defines position (x,y) on screen
void cursorVisible(bool,int); // Hide / show curson. Change cursor size
void InitGameArray(struct peliRuutu[GAME_X_SIZE][GAME_Y_SIZE]); // Set initial values to array
char askUserMark(); // ask if user plays with X or O
void showDifficulty(bool); // print difficulty in screen
int  RandomNumber(int, int); // returns random number
void setComputerRandomMark(struct peliRuutu [GAME_X_SIZE][GAME_Y_SIZE], char); // Computer set mark randomly = EASY
void setComputerFirstMark(struct peliRuutu [GAME_X_SIZE][GAME_Y_SIZE], char); // If computer starts set mark here
void setComputerMarks(struct peliRuutu [GAME_X_SIZE][GAME_Y_SIZE], char, char); // Sets computer marks on grid
int  findTwoInLine(struct peliRuutu [GAME_X_SIZE][GAME_Y_SIZE], char, char); // Find is there's two same marks and one empty on line
bool setPlayerMark(struct peliRuutu [GAME_X_SIZE][GAME_Y_SIZE], int, int, char); // Set players mark on grid
char checkTable(struct peliRuutu [GAME_X_SIZE][GAME_Y_SIZE]); // Check if there's three in line 
char readKeyboard(int*,int*); // Read keyboard and also return pressed key

//-----------------------------------------------------------------

// Set cursor position (x,y) on screen. Origin = left,top
void setCursorXY (int x, int y) {
    c.X = x; c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Hide or show cursor, change cursor size 0-100%
void cursorVisible(bool cursorHidden, int cursorSize) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = cursorSize;
    info.bVisible = cursorHidden;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// set screen positions to array and fill with empty spaces
void InitGameArray(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE]){
    gameArray[0][0].x = 7; gameArray[0][0].y = 2; // set 0,0 on grid (x,y) is 7,2 on screen etc.
    gameArray[0][1].x = 7; gameArray[0][1].y = 4;
    gameArray[0][2].x = 7; gameArray[0][2].y = 6;
    gameArray[1][0].x = 11; gameArray[1][0].y = 2;
    gameArray[1][1].x = 11; gameArray[1][1].y = 4;
    gameArray[1][2].x = 11; gameArray[1][2].y = 6;
    gameArray[2][0].x = 15; gameArray[2][0].y = 2;
    gameArray[2][1].x = 15; gameArray[2][1].y = 4;
    gameArray[2][2].x = 15; gameArray[2][2].y = 6;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            gameArray[i][j].mark = ' ';
        }
    }
}

// Print playfield
    void printField(){
        printf("      TIC-TAC-TOE\n");
        printf("     +---+---+---+\n");
        printf("     |   |   |   |  Use arrow keys or WASD keys\n");        
        printf("     +---+---+---+  to move the cursor. \n");
        printf("     |   |   |   |  Press 'E' to set mark\n");
        printf("     +---+---+---+  and ESC to exit game.\n");
        printf("     |   |   |   |  Toggle difficulty with 'R'\n");
        printf("     +---+---+---+  any time during the game\n");
    
    }

// Ask users mark, X or O, X is default
char askUserMark(){
    char mark = 'X';
    setCursorXY(0,11);
    printf("    Choose your mark:\n");
    printf("    1 = X (starts game)\n");
    printf("    2 = O\n");
    setCursorXY(22,11);
    scanf(" %c",&mark);
    if (mark == '2' || mark == 'o' || mark == 'O' ) mark = 'O';
    else mark = 'X';
    return mark;
}

// prints difficulty level in screen. x and y is used to
void showDifficulty(bool difficultyEasy){
    setCursorXY(4,9);
    if (difficultyEasy){
        printf("Difficulty: EASY");
    } else {
        printf("Difficulty: HARD");
    }
}

// return random number between min and min+range
int RandomNumber(int min, int range) {
    return (rand() % range + min); 
}

//===============================================================
// game started - set marks on grid
//===============================================================
// Set RANDOMLY computers mark. Tryes until empty place was found.
void setComputerRandomMark(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE], char mark) {
    bool markExists = true;
    do{
        int x = RandomNumber(0,3);
        int y = RandomNumber(0,3);
        if (gameArray[x][y].mark == ' ') { 
            gameArray[x][y].mark = mark;
            setCursorXY( gameArray[x][y].x, gameArray[x][y].y );
            printf("%c\033[1D" ,mark);
            markExists = false;
        }    
    } while (markExists == true);
}
//-----------------------------------------------------------------------------------------
// More logical computer player
void setComputerFirstMark(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE], char cMark) {
    // In first game turn use either of top corners 
        if (gameArray[0][0].mark == ' ') {
            gameArray[0][0].mark = cMark;
            setCursorXY( gameArray[0][0].x, gameArray[0][0].y );        
        }
        else {
            gameArray[2][0].mark = cMark; 
            setCursorXY( gameArray[2][0].x, gameArray[2][0].y ); 
        } 
        printf("%c\033[1D" ,cMark);  
}

// If no attack or defend places were found, this function sets the mark
void setComputerMarkNoLines(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE], char cMark){
    int x = -1, y = -1;
    // First check if grids center is free
    if (gameArray[1][1].mark == ' ') { x = 1; y = 1; goto setmark; }
    // if center is occupied find free corner
    if (gameArray[0][2].mark == ' ') { x = 0; y = 2; goto setmark; }
    if (gameArray[2][2].mark == ' ') { x = 2; y = 2; goto setmark; }
    if (gameArray[2][0].mark == ' ') { x = 2; y = 0; goto setmark; }
    if (gameArray[0][0].mark == ' ') { x = 0; y = 0; goto setmark; }

    setmark:
    if ( x != -1 ){
        gameArray[x][y].mark = cMark; // Set computer mark on array
        setCursorXY( gameArray[x][y].x, gameArray[x][y].y ); // Align cursor on the grid...
        printf("%c\033[1D" ,cMark); // ...and print mark. \033[1D moves cursor one step back
    } else {setComputerRandomMark(gameArray, cMark); } // Set mark in random place. Propably never used 
}

// Set computers mark. First find opportunity to win, then defend if player has two marks in line
void setComputerMarks(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE], char cMark, char pMark){
    int x = 0, y = 0, r = 0;
    // Attack. Find if there is two computer marks in line and win
    r = findTwoInLine(gameArray, cMark, pMark); 
    if ( r == 0 ) {  // if winning spot was not found...
    // Defend. Find two players marks in line and block player
        r = findTwoInLine(gameArray, pMark, cMark);
    }
    // Set computers mark on the place found in function 'FindTwoInLine'
    switch (r) {
    case 1:
        for (int i = 0; i < 3; i++){ if (gameArray[i][i].mark == ' ') {x = i; y = i;}}
        break;
    case 2:
        for (int i = 0; i < 3; i++){ if (gameArray[0][i].mark == ' ') {x = 0; y = i;}}
        break;
    case 3:
        for (int i = 0; i < 3; i++){ if (gameArray[1][i].mark == ' ') {x = 1; y = i;}}
        break;
    case 4:
        for (int i = 0; i < 3; i++){ if (gameArray[2][i].mark == ' ') {x = 2; y = i;}}
        break;
    case 5:
        if (gameArray[2][0].mark == ' ') {  x = 2; y = 0;}
        if (gameArray[1][1].mark == ' ') {  x = 1; y = 1;}
        if (gameArray[0][2].mark == ' ') {  x = 0; y = 2;}
        break;
    case 6:
        for (int i = 0; i < 3; i++){ if (gameArray[i][0].mark == ' ') {x = i; y = 0;}}
        break;
    case 7:
        for (int i = 0; i < 3; i++){ if (gameArray[i][1].mark == ' ') {x = i; y = 1;}}
        break;
    case 8:
        for (int i = 0; i < 3; i++){ if (gameArray[i][2].mark == ' ') {x = i; y = 2;}}
        break;
    default:
        setComputerMarkNoLines(gameArray,cMark);
        return;
        break;
    }
    setCursorXY( gameArray[x][y].x, gameArray[x][y].y );
    gameArray[x][y].mark = cMark;
    printf("%c\033[1D" ,cMark);

    setCursorXY(0,15);
    printf("line %d         ", r);
}

// Find if there is two marks in line
// Returns interger which tells the line where two marks are. 1 and 5 are corner to corner, as shown below
// 1 2 3 4 5
// 6 X X X
// 7 X X X
// 8 X X X
//
int findTwoInLine(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE], char searchMark, char oppositeMark ){
    int foundMarks = 0, i= 0, j = 0;
    // columns from up to down
    for (i = 0; i < 3; i++){
        foundMarks = 0;
        for (j = 0; j < 3; j++){  
            if (gameArray[i][j].mark == searchMark) { foundMarks++; } // Searched mark adds counter... 
            if (gameArray[i][j].mark == oppositeMark) { foundMarks--; } // and unwanted substracts counter
        }
        if (foundMarks == 2) return (i+2); // If counter value is 2, it means there is two searhed marks and one empty space
    }
    // lines from left to right
    for (i = 0; i < 3; i++){
        foundMarks = 0;
        for (j = 0; j < 3; j++){  
            if (gameArray[j][i].mark == searchMark){ foundMarks++; }
            if (gameArray[j][i].mark == oppositeMark){ foundMarks--; }
        }
    if (foundMarks == 2) return (i+6);    
    }
    // top left -> bottom right
    foundMarks = 0;
    for (i = 0; i < 3; i++){
        if (gameArray[i][i].mark == searchMark){ foundMarks++; }
        if (gameArray[i][i].mark == oppositeMark) { foundMarks--; }
    }
    if (foundMarks == 2) return 1;

    // top right -> bottom left 
    foundMarks = 0;
        if (gameArray[2][0].mark == searchMark){ foundMarks++; }
        if (gameArray[2][0].mark == oppositeMark){ foundMarks--; }
        if (gameArray[1][1].mark == searchMark) { foundMarks++; }
        if (gameArray[1][1].mark == oppositeMark){ foundMarks--; }
        if (gameArray[0][2].mark == searchMark){ foundMarks++; }
        if (gameArray[0][2].mark == oppositeMark){ foundMarks--; }
        if (foundMarks == 2) return 5;
    return 0;
}

//-----------------------------------------------------------------------------------------
// Sets players mark on grid if there was an empty space, and returns true.
// If position was already occupied, return false.
bool setPlayerMark(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE], int x, int y, char mark){
    bool markSet = false;
    if (gameArray[x][y].mark == ' ') { 
        gameArray[x][y].mark = mark;
        markSet = true;
        setCursorXY( gameArray[x][y].x, gameArray[x][y].y );
        printf("%c" ,mark);
    }
    return markSet;
}

//----------------------------------------------------------------------------------
// Check if three same marks in line.
// Returns tempMark (= X or O) if winner was found.
// Otherwise, returns NoWinner (=' ')
//----------------------------------------------------------------------------------
char checkTable(struct peliRuutu gameArray[GAME_X_SIZE][GAME_Y_SIZE]){
    //int x = 0, y = 0;
    char NoWinner = ' ';
    char tempMark = ' ';
    // ROWS 1,2,3
    for (int i = 0; i < 3; i++) 
    if (gameArray[i][0].mark != ' '){
        tempMark = gameArray[i][0].mark;
        if (gameArray[i][1].mark == tempMark && gameArray[i][2].mark == tempMark) {
          return tempMark;  
        }
    }
    // COLUMNS 1,2,3
    for (int i = 0; i < 3; i++) 
    if (gameArray[0][i].mark != ' '){
        tempMark = gameArray[0][i].mark;
        if (gameArray[1][i].mark == tempMark && gameArray[2][i].mark == tempMark) {
          return tempMark;  
        }
    }
    // top left -> bottom right
    if (gameArray[0][0].mark != ' '){
        tempMark = gameArray[0][0].mark;
        if (gameArray[1][1].mark == tempMark && gameArray[2][2].mark == tempMark) {
            return tempMark;
        }
    }
    // top right -> bottom left 
    if (gameArray[2][0].mark != ' '){
        tempMark = gameArray[2][0].mark;
        if (gameArray[1][1].mark == tempMark && gameArray[0][2].mark == tempMark) {
            return tempMark;
        }
    }
    return NoWinner;
}
// -------------------------------------------------------------------------------------

char readKeyboard(int *xCoordinate,int *yCoordinate){
    int x = *xCoordinate, y = *yCoordinate; 
    char key_code = _getch(); // get pressed key
    if (key_code == 'a' || key_code == 75) { x--; if (x < 0)  {x = 0;} } // left
    if (key_code == 'd' || key_code == 77) { x++; if (x > MAX_WIDTH) { x = MAX_WIDTH; }} // right
    if (key_code == 'w' || key_code == 72) { y--; if (y < 0)  {y = 0;} } // up
    if (key_code == 's' || key_code == 80) { y++; if (y > MAX_HEIGHT)  { y = MAX_HEIGHT; }} // down
    *xCoordinate = x, *yCoordinate = y;
    return key_code;
}

//================================================================
// Tic tac toe 
//================================================================
void tictactoe() { 
    
    char key_code;
    int x = 0, y = 0;
    char markPlayer = 'X';
    char markComputer = 'O';
    bool playerReady = false;
    int freePlaces = 9; // empty places in game (array)
    char winningMark = ' '; // default - no winner
 

    // Initialize game -------------------------------------
    struct peliRuutu pArray[GAME_X_SIZE][GAME_Y_SIZE];
    InitGameArray(pArray);
    cursorVisible(true,100); // show big cursor
    system("cls"); // clear screen
    printField(); // print playfield on screen
    //------------------------------------------------------

    // Ask players mark and set computers mark
    markPlayer = askUserMark();
    if (markPlayer == 'X') { markComputer = 'O'; }
    else { markComputer = 'X';}
    //------------------------------------------------------
    showDifficulty(difficultyEasy);
    setCursorXY(pArray[0][0].x, pArray[0][0].y); // set cursor to top left on the grid

    // Start game
    if (markComputer == 'X') { // If computer plays X, it starts
        setComputerFirstMark(pArray,markComputer);
        setCursorXY(pArray[x][y].x,pArray[x][y].y);
        freePlaces--; 
    }
    // Game loop
    do {
        playerReady = false;
        do {
            if ( _kbhit() ){ // wait keypress
                key_code = readKeyboard(&x ,&y); // if key was pressed read keycode
                setCursorXY(pArray[x][y].x,pArray[x][y].y); // Set curson to new place
                if (key_code == 'e' || key_code == 'E' ) { // set player mark on grid
                    playerReady = setPlayerMark(pArray,x,y,markPlayer);
                    if (playerReady == true) freePlaces--; 
                }
                if (key_code == 27) {cursorVisible(true,10); exit(0);} // press x to end program
                if (key_code == 'r' || key_code == 'R') { 
                    ToggleBoolean(difficultyEasy);  // Toggle true <-> false
                    showDifficulty(difficultyEasy); // Show new difficulty 
                    setCursorXY(pArray[x][y].x,pArray[x][y].y); // Return cursor to its place on grid
                }
            }
        } while ( playerReady == false);
        winningMark = checkTable(pArray); // Check if there's three in line after layers mark

        // player ready && empty cells on grid (<- IMPORTANT) && no winner yet
        if (playerReady == true && freePlaces > 0 && winningMark == ' ') { 
            if (difficultyEasy == true) {  // Don't call these functions if freePlaces == 0
                setComputerRandomMark(pArray,markComputer); // EASY: Computer sets mark in random place
            } else {
                setComputerMarks(pArray,markComputer,markPlayer); // HARD: Kind of AI
            }
            playerReady == false;
            freePlaces--; 
            setCursorXY(pArray[x][y].x,pArray[x][y].y); // set cursor back where it was
        }
        winningMark = checkTable(pArray); // Check if there's three in line after computers mark
    } while (winningMark == ' ' && freePlaces > 0); // No winner && empty places on grid
    
    setCursorXY(0,8);
    if (winningMark != ' ') {
        printf("  *** WINNER IS %c ***", winningMark);
    } else {
        printf("     IT'S A TIE!!!");  
    }
    setCursorXY(0,12);
}

int main() {
    srand(time(NULL));
    bool exitGame = false;
    char yesNo = ' ';

    do {
        tictactoe();
        setCursorXY(0,11);
        printf("                            "); // clear line
        setCursorXY(0,11);
        printf("    New game (y/n)?: ");
        scanf(" %c", &yesNo);
        if (yesNo == 'y' || yesNo == 'Y'){
            exitGame = false;
        } else {
             exitGame = true;
        }     
    } while (exitGame == false);

    system("cls"); // clear screen
    cursorVisible(true,10); // Show normal cursor (size = 10 %)
    return 0;
}