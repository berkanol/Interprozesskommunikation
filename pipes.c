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
    int fd[2];
    if (pipe(fd) == -1) {
            return 1;
        }
    
    int id1 = fork();
            if (id1 == 0) {
                close(fd[0]);
                
                int i;
                int arr[10];
                
                srand(time(NULL));
                
                printf("Generated: ");
                for (i = 0; i < 10; i++) {
                    arr[i] = rand() % 11;
                    printf("%d ", arr[i]);
                }
                printf("\n");
                
                if (write(fd[1], &arr, sizeof(int) * 10) < 0) {
                    return 4;
                }
                
                close(fd[1]);
                printf("Ich bin conv\n");
            } else {
                close(fd[1]);
                int arr[10];
                int length = sizeof(arr)/sizeof(arr[0]);
                
                if (read(fd[0], &arr, sizeof(int) * 10) < 0) {
                    return 6;
                }
                
                read(fd[0], &arr, sizeof(int) * 10);
                close(fd[0]);
                
                for (int i = 0; i < length; i++) {
                       printf("%d ", arr[i]);
                }
                wait(NULL);
                printf("\n");
                printf("Ich bin log\n");
            }
   
    
    return 0;
}
