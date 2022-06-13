//
//  main.c
//  Interprozesskommunikation
//
//  Created by Berkan Oluk on 02.06.22.
//
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

int main(int argc, const char * argv[]) {
    
    int fd[2];
    int id1 = fork();
    
    if (pipe(fd) == -1) {
            printf("An error ocurred with opening the pipe\n");
            return 1;
        }
    
    mkfifo("testfifo" ,0666);
    
    if (id1 == 0) {
           int id2 = fork();
           if (id2 == 0) {
               int id3 = fork();
               if (id3 == 0) {
                   int id4 = fork();
                   if (id4 == 0) {
                       int i;
                       int arr[10];
                       int n = 10;
                       
                       srand(time(NULL));
                       printf("Generiert in CONV: ");
                       for (i = 0; i < n; i++) {
                       arr[i] = rand() % 11;
                       printf("%d ", arr[i]);
                       }
                       printf("\n");
                       
                       write(fd[1], &arr, sizeof(int) * n);
                       close(fd[1]);
                       
                   } else {
                       wait(NULL);
                       int arr[10];
                       int i;
                       int n = 10;
                       
                       read(fd[0], &arr, sizeof(int) * n);
                       
                       int fddatei = open("testfifo", O_RDWR);
                       if (fddatei == -1) {
                           return 5;
                       }
                       
                       write(fddatei, &arr, sizeof(int) * n);
                       
                       printf("Erhalten in LOG: ");
                       for (i = 0; i < n; i++) {
                           if (read(fddatei, &arr[i], sizeof(int)) == -1) {
                               return 8;
                               
                           }
                           printf("%d ", arr[i]);
                           
                       }
                       printf("\n");
                                                   
                       close(fddatei);
                       close(fd[0]);
                       
                       write(fd[1], &arr, sizeof(int) * n);
                       close(fd[1]);
                       
                   }
               } else {
                   wait(NULL);
                   
                   int arr[10];
                   int i;
                   int n = 10;
                   
                   read(fd[0], &arr, sizeof(int) * n);
                   
                   printf("Erhalten in STAT: ");
                   for (i = 0; i < n; i++) {
                       printf("%d ", arr[i]);
                       
                   }
                   printf("\n");
                   
                   int sum = 0;
                   for (i = 0; i < n; i++) {
                       sum += arr[i];
                   }
                   
                   write(fd[1], &sum, sizeof(sum));
                   
                   close(fd[1]);
                   
               }
           } else {
               wait(NULL);
               int sum;
               
               read(fd[0], &sum, sizeof(sum));
               printf("Erhaltene Zahl in REPORT: %d", sum);
               printf("\n");
               
               close(fd[0]);
           
           }
    }
    return 0;
}

