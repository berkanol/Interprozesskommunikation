 //
//  main.c
//  Interprozesskommunikation
//
//  Created by Saliem Salha on 02.06.22.
//  Matrikelnummer : 1398229
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

//create buffer
struct msgbuffer{
    long msgtype;
    char msgtext[100];
}msg;

//true statement for endless loop
bool loop = true;

//method for stoping loop with CRTL-C
void interrupt_handler(int signal) {
    
    if(signal == SIGINT)
    {
        loop = false;
    }
}

int main(int argc, const char ** argv){
    
    signal(SIGINT, interrupt_handler);
    
    // endless loop
    while(loop == true) {
        
//needed variables
    int i = 0;
    int numbers = 0;
    int summe = 0;
    int mittelwert = 0;
    int returncode_msgget = 0;
    int returncode_msgrcv = 0;
    
    //building multiprocesses
    //creating multiple if statements
    int id1 = fork();
    if (id1 == 0) {
        int id2 = fork();
        if (id2 == 0) {
            int id3 = fork();
            if (id3 == 0) {
                int id4 = fork();
                if (id4 == 0) {
                    
                    // CONV //
                    
                    //generating random numbers
                    printf("Zufallszahlen generiert in CONV:\n");
                    
                    //new numbers each time
                    srand(time(NULL));
                    for (i = 0; i < 10; i++) {
                    numbers = rand() %101;
                    msg.msgtext[i] = numbers;
                        printf("%d ", msg.msgtext[i]);
                     }
                    printf("\n");
                    
                }else {
//                    wait(NULL);
                    
                    //creating Message Queue
                    returncode_msgget = msgget(12345, IPC_CREAT| 0600);
                    
                    //checking if message was sent
                    if (msgsnd(returncode_msgget, &msg, sizeof(msg.msgtext), 0) == -1){
                        printf("message not sent\n");
                        perror("msgsnd()");
                        printf("\n");
                        exit(0);
                    }
                }
            }else {
                
                // LOG //
                
                //wait for process
                wait(NULL);
                
                //reaching message queue
                //checking if message queue is reachable
                //if not show error
                msg.msgtype = 1;
                if ((returncode_msgget = msgget(12345,IPC_CREAT| 0600)) == -1) {
                    printf("could not reach Message queue\n");
                    perror("msgget()");
                    printf("\n");
                    exit(0);
                }
                
                //checking if message was received
                //if not show error
                returncode_msgrcv = msgrcv(returncode_msgget, &msg, sizeof(msg), msg.msgtype, IPC_NOWAIT);
                if (returncode_msgrcv == -1) {
                    printf("message not received\n");
                    perror("msgrcv()");
                    printf("\n");
                    exit(0);
                }
                
                //creating and opening file for numbers
                FILE *fp;
                fp = fopen("numbers.txt", "w+");
                
                //saving message in file to send to next process
                fgets(msg.msgtext, sizeof(msg.msgtext), fp);
                
                //closing file after use
                fclose(fp);
                
                //sendin numbers into message queue to next process
                //if not sent show error
                if (msgsnd(returncode_msgget, &msg, sizeof(msg.msgtext), 0) == -1) {
                    printf("numbers not sent\n");
                    perror("msgsnd()");
                    printf("\n");
                    exit(0);
                }
                //prinzing the random numbers
                printf("Übergeben in LOG: ");
                printf("%d ", msg.msgtext[i]);
                printf("\n");
            }
        }else {
            
            // STAT //
            
            //wait for process
            wait(NULL);
            
            //reading message out of queue
            //if not possible show error
            if (msgrcv(returncode_msgget, &msg, sizeof(msg), msg.msgtype,IPC_NOWAIT) == -1) {;
                printf("numbers not received\n");
                perror("msgrcv()");
                printf("\n");
                exit(0);
            }
            
            //calculate mean and sum
            for (i = 0; i < 10; i++) {
                summe += msg.msgtext[i];
             }
            mittelwert = summe / 10;
            
            //sending mean and sum into queue
            //if not possible show error
            if (msgsnd(returncode_msgget, &msg, sizeof(msg.msgtext), 0) == -1) {
                printf("mean and sum not sent\n");
                perror("msgsnd()");
                printf("\n");
                exit(0);
            }
        }
    }else{
        
        // REPORT //
        
        //wait for process
        wait(NULL);
        
        //reading mean and sum out of queue
        //if not possible show error
        if (msgrcv(returncode_msgget, &msg, sizeof(msg), msg.msgtype,IPC_NOWAIT) == -1) {;
            printf("mean and sum not received\n");
            perror("msgrcv()");
            exit(0);
        }
        
        //printing out mean and sum
        printf("Summe: %d Mittelwert %d", summe, mittelwert);
        printf("ich bin report\n");
        }
        
        //do 1 second pause during each loop
        sleep(1);
    }
}

