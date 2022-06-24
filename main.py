#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <unistd.h>
#include<sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>

#define MSGSIZE 100
//creating structure for message queue
struct message_queue
{
    long msg_type;
    char msg_arr[100];
}message;

//other methods for exiting program on Ctrl-C
void signalHandler( int signalValue );
void  INThandler(int signalValue);
bool keepRunning = true;

void interrupt_handler(int signal) {
    
    if(signal == SIGINT)
    {
        keepRunning = false;
    }
}
//driver program
int main()
{
    while (keepRunning) {
        
    
    //processes
    pid_t conv, logg;
    //file pointer
    FILE *ptr;
    //some other pointers and arrays
    char *ptr1;
    char str[10];
    char*ptr3;
    char str2[10];
    //opening the file
    ptr = fopen("./numbers.txt","w+");
    printf("\n\t  ---------------------------------------------------------------------\n");
    printf("\n\t\t Check Auto Generated number.txt File for Random Numbers \n");
    printf("\n\t  ---------------------------------------------------------------------\n");
    printf("\n\n");
    //use of space after each number
    char delim[] = " ";
    //random number generations
    srand(time(0));
    int num, sum = 0, avg = 0, report;
    //child process
    if(fork() == 0)
    {
        //getting ID
        report = getpid();
        printf("Child process ID:%d\n",report);
        printf("\n");
        //reading random numbers
        for(int i=1; i<=10; i++)
        {
            fprintf(ptr, "%d ", rand()%100);
        }
        printf("\nWrote 10 integers to numbers.txt and terminating.\n");
        printf("\n");
        fclose(ptr);
        fflush(NULL);
        return 0;
    }
    //waiting queue
    wait(&report);
    //parent
    if(fork() == 0)
    {
    printf("Child process ID:%d\n",getpid());
    printf("\n");
    //opening the numbers generated in the file
    ptr = fopen("./numbers.txt","rb");
    if( fgets (str, 100, ptr)!=NULL )
    {
        
        ptr1 = strtok(str, delim);
        //addition and average
        while(ptr1 != NULL)
        {
            num = atoi(ptr1);
            sum=sum+num;
            avg=sum/10;
            ptr1 = strtok(NULL, delim);
        }
    }
    printf("Sum : %d, Average : %d and terminating....\n", sum, avg);
    printf("\n");
    
    }
    //waiting queue
    wait(&report);
    //message queue functionality
    printf(" Entering Into Message Queue \n");
    //reading the arrays
    key_t key;
    int msgid;
    //opening the file
    key = ftok("number.txt", 65);
    //generating the numbers
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.msg_type = 10;
    //Fetching data
    printf(" Fetching Data From File..... : \n");
    msgsnd(msgid, &message, sizeof(message), 0);
    //parsing it
    printf(" Writing Data To Message Queue  \n");
    //calculation
    printf(" Sum : %d, Average : %d and terminating....\n", sum, avg);
    printf("\n");
    fclose(ptr);
    wait(NULL);
    
    return 0;
}
}
//terminating program on ctrl-c
//making handlers
void signalHandler( int signalValue )
{
    signal( SIGINT, signalHandler );
    int response;

    printf( "%s%d%s\n%s","\nInterrupt signal ( ", signalValue, " ) received.",
                     "Do you wish to continue ( 1 = yes or 2 = no )? \n" );

    scanf("%d", &response);
    if ( response != 1 )
    {
        signal(SIGINT, INThandler);
    }
}


void  INThandler(int signalValue)
{
    signal(signalValue, SIG_IGN);
    printf("\nCtrl-C command detected!");
    exit(0);
}
