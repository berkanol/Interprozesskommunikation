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
#include <time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(int argc, char* argv[]) {
    int fd[2];
    
    if(mkfifo("testfifo" ,0666)< 0) {
   printf("Das Anlegen der benannten Pipe ist fehlgeschlagen.\n");
    } else {
   printf("Die benannte Pipe testfifo wurde angelegt.\n");
   }
    
    
    
    
    if (pipe(fd) == -1) {
        return 1;
    }
    
    int pid = fork();
    if (pid == -1) {
        return 2;
    }
    
    if (pid == 0) {
        // Conv
        close(fd[0]);
        
        int i;
        int arr[10];
        
        srand(time(NULL));
        int n = 10;
        
        printf("Generiert in CONV: ");
        for (i = 0; i < n; i++) {
            arr[i] = rand() % 11;
            printf("%d ", arr[i]);
        }
        printf("\n");
        
        
        if (write(fd[1], &arr, sizeof(int) * n) < 0) {
            return 3;
        }

        close(fd[1]);
    } else {
        // log
        wait(NULL);
        close(fd[1]);
        
        int arr[10];
        int i;
        int n = 10;
        
        
        if (read(fd[0], &arr, sizeof(int) * n) < 0) {
            return 4;
        }
        
        printf("Übergeben wurde in LOG: ");
        close(fd[0]);
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        
        int fddatei = open("testfifo", O_WRONLY);
            if (fddatei == -1) {
                return 5;
            }
            
            if (write(fddatei, &arr, sizeof(int) * n) == -1) {
                return 6;
            }
            
            close(fddatei);
        
    }

        int fd2 [2];
        int pid2 = fork();
        
        if (pid2 == -1) {
            return 7;
        }
        
        if (pid2 == 0) {
            // stat
            waitpid(pid, NULL, 0);
            close(fd2[0]);
            
            
            int arr[10];
            int n = 10;
            int fddatei = open("testfifo", O_RDONLY);
            if (fddatei == -1) {
                return 7;
            }
                
            int i;
            
            printf("Received in STAT: ");
            for (i = 0; i < n; i++) {
                if (read(fddatei, &arr[i], sizeof(int)) == -1) {
                    return 8;
                }
                printf("%d ", arr[i]);
            }

           
            int sum = 0;
                for (i = 0; i < n; i++) {
                    sum += arr[i];
                }
            
            printf("Summe ist %d\n", sum);
            if (write(fd2[1], &sum, sizeof(int)) < 0) {
                return 9;
            }
            
            close(fddatei);
            close(fd2[1]);
            
            
            
        } else {
            // report
            close(fd2[1]);
            int sum = 0;
            
            
            if (read(fd2[0], &sum, sizeof(int)) < 0) {
                return 10;
            }
            printf("\nsumme ist: %d ", sum);
            
            
            close(fd2[0]);
        }
   
    return 0;
    }

