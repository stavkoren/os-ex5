#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define FD_SIZE 2
#define ERROR_MSG "Error in system call\n"
#define FD_READ_INDEX 0
#define FD_WRITE_INDEX 1
#define INIT_VALUE 0
#define SIZE_OF_BUF 1
#define MOVE_LEFT_CHAR 'a'
#define MOVE_RIGHT_CHAR 'd'
#define MOVE_DOWN_CHAR 's'
#define ROTATE_SHAPE_CHAR  'w'
#define QUIT_CHAR 'q'
#define FIRST_INDEX 0
#define FAILED_VALUE -1
#define PROG_NAME "draw.out"
#define DIR "./draw.out"
/**
 * write error message
 */
void writeErrorMessage(){
    write(fileno(stderr),ERROR_MSG,strlen(ERROR_MSG));
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}
/**
 * checkValidation return true if the input is a valid character or false otherwise
 * @return
 */
bool checkValidation(char* input){
    if((input[FIRST_INDEX]==QUIT_CHAR)||(input[FIRST_INDEX]==MOVE_LEFT_CHAR)
    ||(input[FIRST_INDEX]==MOVE_RIGHT_CHAR||(input[FIRST_INDEX]==MOVE_DOWN_CHAR)
    ||(input[FIRST_INDEX]==ROTATE_SHAPE_CHAR))){
        return true;
    }
    return false;
}
int main() {
    int fd[FD_SIZE];
    pid_t pid;
    if(pipe(fd)<0){
        writeErrorMessage();
    }
    if((pid=fork())<0){
        writeErrorMessage();
    }else{
        if(pid>0){
            printf("parent\n");
            //parent
            char input [SIZE_OF_BUF]={INIT_VALUE};
            //close the read fd
            if(close(fd[FD_READ_INDEX])==FAILED_VALUE){
                writeErrorMessage();
            }
            while (input[FIRST_INDEX]!=QUIT_CHAR){
                input[FIRST_INDEX]=getch();
                if(checkValidation(input)){
                    if(write(fd[FD_WRITE_INDEX],input, sizeof(input))==FAILED_VALUE){
                        writeErrorMessage();
                    }
                    if(kill(pid,SIGUSR2)==FAILED_VALUE){
                        writeErrorMessage();
                    }
                }
            }
            if(close(fd[FD_WRITE_INDEX]==FAILED_VALUE)){
                writeErrorMessage();
            }

        }else{
            //child
           close(fd[FD_WRITE_INDEX]);
            //redirect reading
            if(dup2(fd[FD_READ_INDEX],STDIN_FILENO)==FAILED_VALUE){
                writeErrorMessage();
            }
            char *args[]={NULL};
            execvp(DIR,args);
            //error in execp
            exit(EXIT_FAILURE);
        };
    }
}

