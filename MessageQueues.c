#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct msgbuffer{
    long msgtype;
    char msgtext[100];
    int arr[100];
}msg;

int main(int argc, const char ** argv){
    struct msgbuffer msg;
    int i;
    int n = 10;
    int returncode_msgget, returncode_msgrcv;
    returncode_msgget = msgget(1234, 0600| IPC_CREAT);
    
    int id1 = fork();
    if (id1 == 0) {
        int id2 = fork();
        if (id2 == 0) {
            int id3 = fork();
            if (id3 == 0) {
                int id4 = fork();
                if (id4 == 0) {
                    srand(time(NULL));
                    for (i = 0; i < n; i++) {
                        msg.arr[i] = rand() %101;
                        printf("%d,", msg.arr[i]);
                    }
                }else {
                    wait(NULL);
//                    int i = 0;
//                    int arr [100];
                    strcpy(msg.msgtext, "test");
                    printf("be\n");
//                    int datei = fputs("ich", &msg);
                    printf("hallo");
                    if (msgsnd(returncode_msgget, &msg, sizeof(msg.msgtext), 0) == -1){
                        printf("message not sent\n");
                        perror("msgsnd()");
//                        close(datei);
                    }else{
                        printf("kaka");
                    }
                    printf("Ich bin Conv\n");
                    
                }
            }else {
                wait(NULL);
                struct msgbuffer{
                    long mtype;
                    char mtext [100];
                } msg;
                
//                int i = 0;
//                struct msgbuffer mq;
//                msg receivebuffer;
                msg.mtype = 1;
                if ((returncode_msgget = msgget(1234, 0600)) < 0) {
                    printf("could not reach Message queue\n");
                    perror("msgget()");
                }
                returncode_msgrcv = msgrcv(returncode_msgget, &msg, sizeof(msg.mtext), msg.mtype, MSG_NOERROR|IPC_NOWAIT);
                if (returncode_msgrcv == -1) {
                    printf("message not received\n");
                    perror("msgrcv()");
                
                }
//                printf("%s\n", msg.mtext);
                printf("Ich bin log\n");
            }
        }else {
            wait(NULL);
            printf("ich bin stat\n");
        }
    }else {
        wait(NULL);
        printf("ich bin report\n");
    }
}

