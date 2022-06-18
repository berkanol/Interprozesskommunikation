#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//create buffer
struct msgbuffer{
    long int msgtype;
    char msgtext[100];
    char arr[100];
}msg;

int main(int argc, const char ** argv){
//needed variables
    int i;
    int returncode_msgget;
    int returncode_msgrcv;
    returncode_msgget = msgget(1234,IPC_CREAT| 0600);
    
    //building multiprocesses
    //creating if statements
    int id1 = fork();
    if (id1 == 0) {
        int id2 = fork();
        if (id2 == 0) {
            int id3 = fork();
            if (id3 == 0) {
                int id4 = fork();
                if (id4 == 0) {
                    
                    //generating random numbers
                    srand(time(NULL));
                    for (i = 0; i < 10; i++) {
                        msg.arr[i] = rand() %101;
                        printf("%d,", msg.arr[i]);
                    }
                }else {
                    wait(NULL);
                    
                    //sending message
                    msg.msgtype = 1;
                    strcpy(msg.msgtext, msg.arr);
                    printf("test1\n");
                    
                    //checking if message was sent
                    if (msgsnd(returncode_msgget, &msg, sizeof(msg.msgtext), 0) == -1){
                        printf("message not sent\n");
                        perror("msgsnd()");
                    }else{
                        
                    printf("Ich bin Conv\n");
                    }
                }
            }else {
                wait(NULL);
                
                //creating buffer for receiver
                struct msgbuffer{
                    long mtype;
                    char mtext [100];
                    char arr [100];
                } msg;
                
                //reaching message queue
                //checking if message queue is reachable
                msg.mtype = 1;
                if ((returncode_msgget = msgget(1234,IPC_CREAT| 0600)) < 0) {
                    printf("could not reach Message queue\n");
                    perror("msgget()");
                }
                
                //checking if message was received
                //if yes than print message and show size
                returncode_msgrcv = msgrcv(returncode_msgget, &msg, sizeof(msg.arr), msg.mtype, MSG_NOERROR|IPC_NOWAIT);
                if (returncode_msgrcv == -1) {
                    printf("message not received\n");
                    perror("msgrcv()");
                
                }else {
                    printf ("Diese Nachricht wurde aus der Warteschlange gelesen : %s \n" , msg.mtext );
                    printf ("Die empfangene Nachricht ist %i Zeichen lang \n" , returncode_msgrcv );
                    }
                
                //saving message in file to send to next process
                printf("Ich bin log\n");
            }
        }else {
            wait(NULL);
            
            //calculate mean and sum
            printf("ich bin stat\n");
        }
    }else {
        wait(NULL);
        
        //printing out
        printf("ich bin report\n");
    }
}


