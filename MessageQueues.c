#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>
#include <sys/msg.h>

//buffer anlegen
struct msgbuffer{
    long msgtype;
    char msgtext [100];
    int arr[100];
}msg;

int main(int argc , char ** argv){
    //benötigte variablen
    int returncode_msgget;
    int returncode_msgrcv;
    int i;
    
    //message queue anlegen
    returncode_msgget = msgget(1234, IPC_CREAT |0600);
    
    //for schleife um zufallszahlen zu generieren
    srand(time(NULL));
    for (i = 0; i < 10; i++) {
        msg.arr[i] = rand() %101;
        printf("%d,", msg.arr[i]);
    }
    //message senden
    msg.msgtype = 1;
    strcpy(msg.msgtext,"test");
//    int datei = fputs("ich", &msg);
    
    //if schleife zur überprüfung, ob nachricht abgeschickt wurde
    if (msgsnd(returncode_msgget, &msg, sizeof(msg.msgtext), 0) == -1){
        printf("message not sent\n");
        perror("msgsnd()");
    }
    //buffer für message receiver
    wait(NULL);
    struct msgbuffer{
        long mtype;
        char mtext [100];
    } msg;
    
    // auf bereits erstellten message queue zugreifen
    // überprüfung ob auf message queue zugegriffen werden kann
    msg.mtype = 1;
    if ((returncode_msgget = msgget(1234, IPC_CREAT| 0600)) < 0) {
        printf("could not reach Message queue\n");
        perror("msgget()");
    }
    //überprüfen ob nachricht angekommen ist
    returncode_msgrcv = msgrcv(returncode_msgget, &msg, sizeof(msg.mtext), msg.mtype, MSG_NOERROR|IPC_NOWAIT);
    if (returncode_msgrcv == -1) {
        printf("message not received\n");
        perror("msgrcv()");
    //wenn message angekommen ist, soll message angezeigt werden
    }else {
         printf ("Diese Nachricht wurde aus der Warteschlange gelesen : %s \n" , msg . mtext );
         printf ("Die empfangene Nachricht ist %i Zeichen lang .\n" , returncode_msgrcv );
         }
//    printf("Drei");
}


