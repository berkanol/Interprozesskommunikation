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

int main(int argc, char* argv[]) {
    int fd[3][2];
    int i;
    for (i = 0; i < 3; i++) {
        if (pipe(fd[i]) < 0) {
            return 1;
        }
    }
    
    int pid1 = fork();
    if (pid1 < 0) {
        return 2;
    }
    
    if (pid1 == 0) {
        // Child process 1
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[2][0]);
        close(fd[2][1]);
        int i;
        int arr[10];
        srand(time(NULL));
        
        for (i = 0; i < 10; i++) {
            arr[i] = rand() % 11;
            
        }
                        
        if (write(fd[1][1], &arr, sizeof(int) * 10) < 0) {
            return 4;
            
        }
        close(fd[0][0]);
        close(fd[1][1]);
        printf("Ich bin conv\n");
    }
    
    int pid2 = fork();
    if (pid2 < 0) {
        return 5;
    }
    
    if (pid2 == 0) {
        // Child process 2
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][1]);
        close(fd[2][0]);
        int arr[10];
        int length = sizeof(arr)/sizeof(arr[0]);
                
        if (read(fd[1][0], &arr, sizeof(int) * 10) < 0) {
            return 6;
            
        }
                       
        read(fd[1][0], &arr, sizeof(int) * 10);
        close(fd[1][0]);
        close(fd[2][1]);
        
        printf("Generated: ");
        for (int i = 0; i < length; i++) {
            printf("%d ", arr[i]);
            
        }
        printf("\n");
        printf("Ich bin log\n");
    }
    
    // Parent process
    close(fd[0][0]);
    close(fd[1][0]);
    close(fd[1][1]);
    close(fd[2][1]);
    
    int sum = 0;
    int arr [10];

    read(fd[2][0], &arr, sizeof(int));
    
    close(fd[0][1]);
    close(fd[2][0]);
    
    for (i = 0; i < 10; i++) {
                sum += arr[i];
            }
    
    printf("Result is %d\n", sum);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
