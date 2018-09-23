#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<GLUT/glut.h>

int mode = 1; //computer mode = 1, Two player mode = 2
int matrix[3][3]; //this matrix stores the x and o and blank box of the game, a value of 0 is blank, 1 is x and 2 is o
int playerturn; //playerturn if it is 1 then 1st players turn else if it is 2 then its second players turn
int result; //result of the game if it is 0 then draw if it is 1 then player 1 wins if it is 2 then player 2 wins
bool gameover; //gameover if it is 0 then its not game over else if it is 1 then its game over

struct Move {
    int row, col;
    Move() {
        row = -1;
        col = -1;
    }
};

struct WinPosition {
    int x1, y1;
    WinPosition() {
        x1 = y1 = -1;
    }
};

WinPosition winPosition;

void initgame()
{
    playerturn = 1;
    winPosition.x1 = winPosition.y1 = -1;
    for(int i = 0; i <= 2; i++)
        for(int j = 0; j <= 2; j++)
            matrix[i][j] = 0;
}

bool isMovesLeft(int board[3][3])
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j] == 0)
                return true;
    return false;
}

int evaluate()
{
    for (int row = 0; row < 3; row++)
    {
        if (matrix[row][0] == matrix[row][1] && matrix[row][1] == matrix[row][2])
        {
            if (matrix[row][0] == 1) {
                winPosition.x1 = row;
                return 10;
            }
            else if (matrix[row][0] == 2) {
                winPosition.x1 = row;
                return -10;
            }
        }
    }
    
    for (int col = 0; col < 3; col++)
    {
        if (matrix[0][col] == matrix[1][col] && matrix[1][col] == matrix[2][col])
        {
            if (matrix[0][col] == 1) {
                winPosition.y1 = col;
                return 10;
            }
            else if (matrix[0][col] == 2) {
                winPosition.y1 = col;
                return -10;
            }
        }
    }
    
    if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2])
    {
        if (matrix[0][0] == 1) {
            winPosition.x1 = winPosition.y1 = 0;
            return 10;
        }
        else if (matrix[0][0] == 2) {
            winPosition.x1 = winPosition.y1 = 0;
            return -10;
        }
    }
    
    if (matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0])
    {
        if (matrix[0][2] == 1) {
            winPosition.x1 = winPosition.y1 = 1;
            return 10;
        }
        else if (matrix[0][2] == 2) {
            winPosition.x1 = winPosition.y1 = 1;
            return -10;
        }
    }
    return 0;
}

int minimax(int depth, bool isMax)
{
    int score = evaluate();
    if (score == 10)
        return score - depth;
    if (score == -10)
        return score + depth;
    if (isMovesLeft(matrix) == false)
        return 0;
    
    if(isMax) {
        int best = -1000;
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                if (matrix[i][j] == 0)
                {
                    matrix[i][j] = 1;
                    best = fmax(best,minimax(depth+1, !isMax));
                    matrix[i][j] = 0;
                }
            }
        }
        return best;
    }
    else
    {
        int best = 1000;
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                if (matrix[i][j] == 0)
                {
                    matrix[i][j] = 2;
                    best = fmin(best, minimax(depth+1, !isMax));
                    matrix[i][j] = 0;
                }
            }
        }
        return best;
    }
}

Move getBestMove()
{
    int bestVal = 1000;
    Move bestMove;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(matrix[i][j] == 0) {
                matrix[i][j] = 2;
                int moveVal = minimax(0, true);
                matrix[i][j] = 0;
                
                if (moveVal < bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

//this function is called when keyboard button is pressed
void KeyPress(unsigned char key, int x, int y )
{
    switch(key){
        case 'y':
            if(gameover == true)
            {
                gameover = false;
                initgame();
            }
            break;
        case 'n':
            if(gameover == true)
                exit(0);
            break;
        case 27:
            exit(0);
        case 'q':
            exit(0);
    }
}

//This function is called when the mouse button is pressed and this function puts the x or o on the blank box
void click(int button, int state, int x, int y)
{
    if(gameover == false && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if(playerturn == 1)
        {
            if(matrix[(y - 50) / 100][x / 100] == 0)
            {
                matrix[(y - 50) / 100][x / 100] = 1;
                if (mode == 1 && isMovesLeft(matrix)) {
                    Move bestMove = getBestMove();
                    matrix[bestMove.row][bestMove.col] = 2;
                }
                else
                    playerturn = 2;
            }
        }
        else
        {
            if(matrix[(y - 50) / 100][x / 100] == 0)
            {
                matrix[(y - 50) / 100][x / 100] = 2;
                playerturn = 1;
            }
        }
    }
}

//Used to write text in the opengl window
void drawString (void * font, char *s, float x, float y)
{
    unsigned int i;
    glRasterPos2f(x, y);
    for (i = 0; i < strlen (s); i++)
        glutBitmapCharacter (font, s[i]);
}

//This function is used to draw the 4 lines 2 vertical and 2 horizontal
void drawlines()
{
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    //2 vertical lines
    glVertex2f(100, 50);
    glVertex2f(100, 350);
    glVertex2f(200, 350);
    glVertex2f(200, 50);
    //2 horizontal lines
    glVertex2f(0, 150);
    glVertex2f(300, 150);
    glVertex2f(0, 250);
    glVertex2f(300, 250);
    glEnd();
}

//This Function Draw the x and o's
void drawxo()
{
    for(int i = 0; i <= 2; i++)
    {
        for(int j = 0; j <= 2; j++)
        {
            if(matrix[i][j] == 1) //if it is 1 then draw x
            {
                glBegin(GL_LINES);
                glVertex2f(50 + j * 100 - 25, 100 + i * 100 - 25);
                glVertex2f(50 + j * 100 + 25, 100 + i * 100 + 25);
                glVertex2f(50 + j * 100 - 25, 100 + i * 100 + 25);
                glVertex2f(50 + j * 100 + 25, 100 + i * 100 - 25);
                glEnd();
            }
            else if(matrix[i][j] == 2) //if it is 2 then draw o
            {
                glBegin(GL_LINE_LOOP);
                glVertex2f(50 + j * 100 - 25, 100 + i * 100 - 25);
                glVertex2f(50 + j * 100 - 25, 100 + i * 100 + 25);
                glVertex2f(50 + j * 100 + 25, 100 + i * 100 + 25);
                glVertex2f(50 + j * 100 + 25, 100 + i * 100 - 25);
                glEnd();
            }
        }
    }
}

void drawWinningLine(int x1, int y1, int x2, int y2) {
    glBegin(GL_LINES);
    glColor3f(0, 0.5, 0.5);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

//This is the idle call back function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    
    if(mode == 1)
        drawString(GLUT_BITMAP_HELVETICA_18, "Computer Mode", 80, 400);
    else
        drawString(GLUT_BITMAP_HELVETICA_18, "2 Player Mode", 100, 400);
    
    if(playerturn == 1)
        drawString(GLUT_BITMAP_HELVETICA_18, "Player 1's turn", 100, 30);
    else {
        if(mode == 1)
            drawString(GLUT_BITMAP_HELVETICA_18, "Computer's turn", 100, 30);
        else
            drawString(GLUT_BITMAP_HELVETICA_18, "Player 2's turn", 100, 30);
    }
    
    drawlines();
    drawxo();
    
    if(evaluate() != 0)
    {
        if (winPosition.x1 == winPosition.y1) {
            if (winPosition.x1 == 0)
                drawWinningLine(50, 100, 250, 300);
            else
                drawWinningLine(250, 100, 50, 300);
        }
        else {
            if (winPosition.x1 == -1)
                drawWinningLine(100*winPosition.y1 + 50, 100, 100*winPosition.y1 + 50, 300);
            else
                drawWinningLine(50, 100*winPosition.x1 + 100, 250, 100*winPosition.x1 + 100);
        }

        if(playerturn == 1)
        {
            gameover = true;
            result = 2; // player 2 wins
        }
        else
        {
            gameover = true;
            result = 1; // player 1 wins
        }
    }
    else if(isMovesLeft(matrix) == false)
    {
        gameover = true;
        result = 0;
    }
    
    if(gameover == true)
    {
        drawString(GLUT_BITMAP_HELVETICA_18, "Game Over", 100, 160);
        
        if(result == 0)
            drawString(GLUT_BITMAP_HELVETICA_18, "Its a draw", 110, 185);
        
        if(result == 1)
            drawString(GLUT_BITMAP_HELVETICA_18, "Player 1 wins", 95, 185);
        
        if(result == 2) {
            if(mode == 2)
                drawString(GLUT_BITMAP_HELVETICA_18, "Player 2 wins", 95, 185);
            else
                drawString(GLUT_BITMAP_HELVETICA_18, "Computer wins", 95, 185);
        }
        
        drawString(GLUT_BITMAP_HELVETICA_18, "Do you want to continue (y/n)", 40, 210);
    }
    
    glutSwapBuffers();
}

//This function is called on windows resize
void reshape(int x, int y)
{
    glViewport(0, 0, x, y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, x, y, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void menuFunction(int value)
{
    switch (value) {
        case 1:
            mode = 1;
            initgame();
            break;
        case 2:
            mode = 2;
            initgame();
            break;
        case 3:
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    initgame();
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(300,450);
    glutCreateWindow("Tic Tac Toe");
    
    glutCreateMenu(menuFunction);
    glutAddMenuEntry(" Computer Mode ", 1);
    glutAddMenuEntry(" 2 Player Mode ", 2);
    glutAddMenuEntry(" Exit ", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(KeyPress);
    glutMouseFunc(click);
    glutIdleFunc(display);
    glutMainLoop();
}








