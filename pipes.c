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
    
    int id1 = fork();                           // Prozess 1 wird erschaffen durch den Befehl fork()
    int fd1 [2];                               // Pipes werden erstellt
    
    // Pipe wird geöffnet und vorgehen bei Fehler wird definiert
    if (pipe(fd1) == -1) {
        printf("Pipe konnte nicht geöffnet werden");
        return 1;
    }
    
    // Bennante Pipe wird erstellt
    if(mkfifo("testfifo" ,0666)< 0) {
       printf("Das Anlegen der benannten Pipe ist fehlgeschlagen.\n");
        } else {
       printf("Die benannte Pipe testfifo wurde angelegt.\n");
       }
    
    
    // Erstellen und Ablauf aller 4 Prozesse (CONV, LOG, STAT, REPORT) und der Datenaustausch mithilfe von Pipes
    if (id1 == 0) {
        int id2 = fork();                                   // Prozess 2
        if (id2 == 0) {
            int id3 = fork();                               // Prozess 3
            if (id3 == 0) {
                
                // CONV
                
                close(fd1[0]);                              // Pipe zum Lesen wird geschlossen
                
                int i;
                int arr[10];
                        
                srand(time(NULL));
                int n = 10;
                
                // For-Schleife zum generieren von Zufälligen Zahlen
                printf("Generiert in CONV: ");
                for (i = 0; i < n; i++) {
                    arr[i] = rand() % 11;
                    printf("%d ", arr[i]);
                }
                printf("\n");
                
                // In die Pipe zum Schreiben wird geschrieben
                write(fd1[1], &arr, sizeof(int) * n);
                
                if (write(fd1[1], &arr, sizeof(int) * n) < 0) {
                    return 2;
                }
                
                close(fd1[1]);                              // Pipe zum Schreiben wird geschlossen
            } else {
                
                // LOG
                
                wait(NULL);                                 // Wartet bis der Kindprozess (CONV) sich beendet
                        
                int arr[10];
                int i;
                int n = 10;
                
                // Liest die Daten die vom Kindprozess (CONV) geschrieben wurden und fügt Sie in ein Array ein
                if (read(fd1[0], &arr, sizeof(int) * n) < 0) {
                    return 3;
                }
                
                
                // Benannte Pipes
                int fddatei = open("testfifo", O_RDWR);
                if (fddatei == -1) {
                    return 5;
                }
                            
                if (write(fddatei, &arr, sizeof(int) * n) == -1) {
                    return 6;
                }
                
                // Überprüfen ob alle Zahlen richtig übergeben wurden
                printf("Erhalten in LOG: ");
                for (i = 0; i < n; i++) {
                if (read(fddatei, &arr[i], sizeof(int)) == -1) {
                    return 8;
                    }
                printf("%d ", arr[i]);
                }
                printf("\n");
                
                close(fddatei);
                close(fd1[0]);
                
                if (write(fd1[1], &arr, sizeof(int) * n) < 0) {
                    return 4;
                }
                
                close(fd1[1]);
            }
        } else {
            
            // STAT
            
            wait(NULL);
            
            int arr[10];
            int i;
            int n = 10;
            
                
            if (read(fd1[0], &arr, sizeof(int) * n) < 0) {
                return 5;
            }
    
            close(fd1[0]);
            printf("Erhalten in STAT: ");
            for (i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            int sum = 0;
            for (i = 0; i < n; i++) {
                sum += arr[i];
            }
            printf("Summe ist: %d\n", sum);
            write(fd1[1], &sum, sizeof(sum));
            
            close(fd1[1]);
            
        }
    } else {
        
        // REPORT
        
        wait(NULL);
        close(fd1[1]);
        
        int sum ;
        
        read(fd1[0], &sum, sizeof(sum));
        
        printf("Erhalten in REPORT: %d\n", sum);
        close(fd1[0]);
       
    }
    
    return 0;
}
