//
//  main.c
//  Interprozesskommunikation
//
//  Created by Berkan Oluk on 02.06.22.
//
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    int id1 = fork();
    
    if (pipe(fd) == -1) {
            printf("An error ocurred with opening the pipe\n");
            return 1;
        }
    
    if (id1 == 0) {
           int id2 = fork();
           if (id2 == 0) {
               int id3 = fork();
               if (id3 == 0) {
                   int id4 = fork();
                   if (id4 == 0) {
                       printf("CONV\n");
                   } else {
                       wait(NULL);
                       printf("LOG\n");
                   }
               } else {
                   wait(NULL);
                   printf("STAT\n");
               }
           } else {
               wait(NULL);
               printf("REPORT\n");
       }
    return 0;
}
