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
    // insert code here...
    int id1 = fork();

    if (id1 == 0) {
        int id2 = fork();
        if (id2 == 0) {
            int id3 = fork();
            if (id3 == 0) {
                printf("Ich bin conv\n");
            } else {
                wait(NULL);
                printf("Ich bin log\n");
            }
        } else {
            wait(NULL);
            printf("Ich bin stat\n");
        }
    } else {
        wait(NULL);
        printf("Ich bin report\n");
    }
    
    
    
    return 0;
}
