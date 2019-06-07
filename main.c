#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FRAME_SIZE 20
#define BORDER_CHAR '*'
#define SHAPE_CHAR '-'
#define SPACE_CHAR ' '
#define UNDER_LINE "\n"
#define FIRST_ROW 0
#define FIRST_COLUMN 0
#define INDEX_BACK -1
#define INDEX_NEXT 1
#define INIT_ROW 1
#define MIDDLE 2
#define ALARM_INTERVAL 1
struct point{
    int row;
    int column;
}point;
struct shape{
    struct point beginPoint;
    struct point endPoint;
};
char board[FRAME_SIZE][FRAME_SIZE];
struct shape shape;
bool runProg=true;
/**
 * draw board to screen
 */
void drawBoard(){
    for (int i = 0; i < FRAME_SIZE; i++) {
        for (int j = 0; j < FRAME_SIZE; j++) {
            if(board[i][j]==SHAPE_CHAR||board[i][j]==BORDER_CHAR){
                printf("%c",board[i][j]);
            }else{
                printf("%c",SPACE_CHAR);
            }
        }
        printf("%s",UNDER_LINE);
    }
}
/**
 * fill frame with boarder char
 */
void fillFrame(){
    for (int i = 0; i < FRAME_SIZE; i++) {
        //fill first row
        board[FIRST_ROW][i]=BORDER_CHAR;
        //fill last row
        board[FRAME_SIZE+INDEX_BACK][i]=BORDER_CHAR;
        //fill first column
        board[i][FIRST_COLUMN]=BORDER_CHAR;
        //fill last column
        board[i][FRAME_SIZE+INDEX_BACK]=BORDER_CHAR;
    }
}
/**
 * check if the shape is vertical
 * @return true if vertical, false otherwise
 */
bool isVertical(){
    if(shape.beginPoint.column==shape.endPoint.column){
        return true;
    }
    return false;
}
/**
 * mark shape on board
 */
void markShapeOnBord(){
    if(isVertical()){
        for (int i=shape.beginPoint.row;i<=shape.endPoint.row;i++){
            board[i][shape.endPoint.column]=SHAPE_CHAR;
        }
    }else{
        //the shape is horizontal
        for (int i=shape.beginPoint.column;i<=shape.endPoint.column;i++){
            board[shape.endPoint.row][i]=SHAPE_CHAR;
        }
    }

}
/**
 * initialize shape position
 */
void initShape(){
    shape.beginPoint.column=FRAME_SIZE/MIDDLE+INDEX_BACK;
    shape.endPoint.column=FRAME_SIZE/MIDDLE+INDEX_NEXT;
    shape.beginPoint.row=INIT_ROW;
    shape.endPoint.row=INIT_ROW;
}

/**
 * move shape down or initialize its position
 */
void moveShapeDown(){
    if(shape.endPoint.row+INDEX_NEXT<FRAME_SIZE+INDEX_BACK){
        //move shape one index ahead
        shape.beginPoint.row+=INDEX_NEXT;
        shape.endPoint.row+=INDEX_NEXT;
    }else{
        initShape();
    }
}
/**
 * move shape one index left if possible
 */
void moveShapeLeft(){
    if(shape.beginPoint.column>FIRST_COLUMN+INDEX_NEXT){
        shape.beginPoint.column=shape.beginPoint.column+INDEX_BACK;
        shape.endPoint.column=shape.endPoint.column+INDEX_BACK;
    }
}
/**
 * move shape one index right if possible
 */
void moveShapeRight(){
    if(shape.endPoint.column<FRAME_SIZE+INDEX_BACK){
        shape.beginPoint.column=shape.beginPoint.column+INDEX_NEXT;
        shape.endPoint.column=shape.endPoint.column+INDEX_NEXT;
    }
}
/**
 * rotate shape is possible
 */
void rotateShape(){
    if (isVertical()){
        shape.beginPoint.row+=INDEX_NEXT;
        shape.endPoint.row+=INDEX_BACK;
        shape.beginPoint.column+=INDEX_BACK;
        shape.endPoint.column+=INDEX_NEXT;
    }else{
        //the shape is horizontal
        if(shape.endPoint.row<FRAME_SIZE+INDEX_BACK){
            shape.beginPoint.row+=INDEX_BACK;
            shape.endPoint.row+=INDEX_NEXT;
            shape.beginPoint.column+=INDEX_NEXT;
            shape.endPoint.column+=INDEX_BACK;
        }
    }
}
/**
 * clear screen
 */
void clearScreen(){
   // system("clear");
    system("@cls||clear");
}
/**
 * exit program
 */
void exitProgram(){
    close(STDIN_FILENO);
    exit(EXIT_SUCCESS);
}
/**
 * delete shape from board
 */
void deleteShape(){
    if(isVertical()){
        for (int i=shape.beginPoint.row;i<=shape.endPoint.row;i++){
            board[i][shape.endPoint.column]=SPACE_CHAR;
        }
    }else{
        //the shape is horizontal
        for (int i=shape.beginPoint.column;i<=shape.endPoint.column;i++){
            board[shape.endPoint.row][i]=SPACE_CHAR;
        }
    }
}
/**
 * alarm procedure- move shape down on board
 * @param sig
 */
void alarmHand(int sig){
    clearScreen();
    deleteShape();
    moveShapeDown();
    markShapeOnBord();
    drawBoard();
    signal(SIGALRM,alarmHand);
    alarm(ALARM_INTERVAL);
}
int main() {
    fillFrame();
    initShape();
    signal(SIGALRM,alarmHand);
    alarm(ALARM_INTERVAL);
    while (runProg){
        pause();
    }
    return 0;
}