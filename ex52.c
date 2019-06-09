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
#define SIZE_OF_BYTES_TO_READ 1
#define MOVE_LEFT_CHAR 'a'
#define MOVE_RIGHT_CHAR 'd'
#define MOVE_DOWN_CHAR 's'
#define ROTATE_SHAPE_CHAR  'w'
#define QUIT_CHAR 'q'
#define FIRST_INDEX 0
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
    int i=0;
    int j=0;
    for (; i < FRAME_SIZE; i++) {
        for (; j < FRAME_SIZE; j++) {
            if(board[i][j]==SHAPE_CHAR||board[i][j]==BORDER_CHAR){
                printf("%c",board[i][j]);
            }else{
                printf("%c",SPACE_CHAR);
            }
        }
        j=0;
        printf("%s",UNDER_LINE);
    }
}
/**
 * fill frame with boarder char
 */
void fillFrame(){
    int i=0;
    for (; i < FRAME_SIZE; i++) {
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
        int i=shape.beginPoint.row;
        for (;i<=shape.endPoint.row;i++){
            board[i][shape.endPoint.column]=SHAPE_CHAR;
        }
    }else{
        //the shape is horizontal
        int i=shape.beginPoint.column;
        for (;i<=shape.endPoint.column;i++){
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
    if(shape.endPoint.column<FRAME_SIZE+INDEX_BACK+INDEX_BACK){
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
        if(shape.endPoint.row<FRAME_SIZE+INDEX_BACK&&shape.beginPoint.row>FIRST_ROW+INDEX_NEXT){
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
    system("clear");
}
/**
 * exit program
 */
void exitProgram(){
    runProg=false;
    close(STDIN_FILENO);
    exit(EXIT_SUCCESS);
}
/**
 * delete shape from board
 */
void deleteShape(){
    if(isVertical()){
        int i=shape.beginPoint.row;
        for (;i<=shape.endPoint.row;i++){
            board[i][shape.endPoint.column]=SPACE_CHAR;
        }
    }else{
        //the shape is horizontal
        int i=shape.beginPoint.column;
        for (;i<=shape.endPoint.column;i++){
            board[shape.endPoint.row][i]=SPACE_CHAR;
        }
    }
}
/**
 * alarm procedure- move shape down on board
 * @param sig
 */
void alarmHand(int signum,siginfo_t *info, void*ptr){
    clearScreen();
    deleteShape();
    moveShapeDown();
    markShapeOnBord();
    drawBoard();
    alarm(ALARM_INTERVAL);
}
/**
 * signal handler func- defines procedure for user's choice
 * @param signum
 * @param info
 * @param ptr
 */
void signalHandler(int signum,siginfo_t *info, void*ptr){
    char userChoice[1];
    read(STDIN_FILENO,userChoice,SIZE_OF_BYTES_TO_READ);
    if(userChoice[FIRST_INDEX]==QUIT_CHAR){
        exitProgram();
    }
    clearScreen();
    deleteShape();
    if(userChoice[FIRST_INDEX]==MOVE_DOWN_CHAR){
        moveShapeDown();
    }
    if (userChoice[FIRST_INDEX]==MOVE_LEFT_CHAR){
        moveShapeLeft();
    }
    if(userChoice[FIRST_INDEX]==MOVE_RIGHT_CHAR){
        moveShapeRight();
    }
    if (userChoice[FIRST_INDEX]==ROTATE_SHAPE_CHAR){
        rotateShape();
    }
    markShapeOnBord();
    drawBoard();
}
int main() {
    //initialize board and shape
    fillFrame();
    initShape();
    //initialize sigaction
    struct sigaction act;
    act.sa_sigaction=signalHandler;
    sigaction(SIGUSR2,&act,NULL);
    //initialize sigaction
    struct sigaction alarmAct;
    alarmAct.sa_sigaction=alarmHand;
    sigaction(SIGALRM,&alarmAct,NULL);
    alarm(ALARM_INTERVAL);
    while (runProg){
        pause();
    }
    return 0;
}