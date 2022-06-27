//
//  main.c
//  Interprozesskommunikation
//
//  Created by Berkan Oluk on 02.06.22.
//  Matrikelnummer: 1394692
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>

bool keepRunning = true;

// Funktion zum abfangen von Signals
void interrupt_handler(int signal) {
    
    if(signal == SIGINT)
    {
        keepRunning = false;
    }
}

int main(int argc, const char * argv[]) {
    
    // Wartet auf das Signal, in diesem Fall ctrl-c
    signal(SIGINT, interrupt_handler);
    
    // Lokale Datei wird erstellt
    FILE *file = NULL;
    
    // Endlosschleife
    while(keepRunning)
    {
        
        // Pipes (fd) und Prozesse (prozess1) werden definiert
        int fd[2];
        int fd2[2];
        
        int prozess1 = fork();  // Prozess 2

        // Pipe wird geöffnet, falls nicht soll Fehlermeldung ausgegeben werden
        if (pipe(fd) == -1) {
            printf("Beim öffnen der Pipe ist ein Fehler unterlaufen\n");
            return 1;
        }
        if (pipe(fd2) == -1) {
            printf("Beim öffnen der Pipe ist ein Fehler unterlaufen\n");
            return 1;
        }
    
        // Benannte Pipe wird angelegt
        mkfifo("testfifo" ,0666);
        
        
        
        // Weitere Prozesse werden definiert (id2, id3, id4) und Ablauf der Prozesse (CONV, LOG, STAT, REPORT)
        if (prozess1 == 0) {
            int prozess2 = fork();                                      // Prozess 2
            if (prozess2 == 0) {
                int prozess3 = fork();                                  // Prozess 3
                if (prozess3 == 0) {
                    int prozess4 = fork();                              // Prozess 4
                    if (prozess4 == 0) {
                        
                        // CONV
                                                
                        // Variablen werden definiert
                        int i;
                        int arr[10];
                        int n = 10;
                                            
                                                
                        srand(time(NULL));              // srand(), damit bei jedem durchlauf neue Zahle generiert werden
                        printf("Generiert in CONV: ");
                                                
                        // for-Schleife um Zufallszahlen zu generieren
                        for (i = 0; i < n; i++) {
                            arr[i] = rand() % 11;
                            printf("%d ", arr[i]);
                            
                        }
                        printf("\n");
                        printf("\n");
                                                
                        // Daten werden in die Pipe geschrieben und Pipe zum schreiben wird geschlossen
                        write(fd[1], &arr, sizeof(int) * n);
                        close(fd[1]);
                        
                        // exit um den Prozess zu terminieren und zu schließen
                        exit(0);
                    } else {
                        
                        // LOG
                                                
                        // wait() um auf den Kinfprozess (CONV) zu warten
                        wait(NULL);
                        int arr[10];
                        int i;
                        int n = 10;
                        
                        // Lokale Datei wird geöffnet
                        file = fopen("zahlen.txt" , "a");
                        
                        
                        // Daten werden aus der Pipe gelesen und in das neue Array geschrieben
                        read(fd[0], &arr, sizeof(int) * n);
                        
                        // In die Lokale Datei wird geschrieben
                        for (i = 0; i < n; i++) {
                            fprintf(file, "%d\n", arr[i]);
                        }
                        fclose(file);
                        
                        
                        // Benannte Pipe wird geöffnet und es werden Daten reingeschrieben
                        int fddatei = open("testfifo", O_RDWR);
                        if (fddatei == -1) {
                            return 5;
                        }
                        
                        write(fddatei, &arr, sizeof(int) * n);
                        
                        // Zur Überprüfung werden die in die benannte Pipe geschrieben Daten ausgelesen
//                        for (i = 0; i < n; i++) {
//                            if (read(fddatei, &arr[i], sizeof(int)) == -1) {
//                                return 8;
                                
//                            }
//                            printf("%d ", arr[i]);
                            
//                        }
//                        printf("\n");
                                        
                        // Benannte Pipe und Pipe zum lesen werden geschlossen
                        close(fddatei);
                        close(fd[0]);
                        
                        // Daten werden in Pipe geschrieben und anschließend geschlossen
                        write(fd[1], &arr, sizeof(int) * n);
                        close(fd[1]);
                        exit(0);
                    }
                } else {
                    
                    //STAT
                                        
                    // wait() um auf den Kindprozess (LOG) zu warten
                    wait(NULL);
                                        
                    int arr[10];
                    int i;
                    int n = 10;
                                        
                    // Daten werden aus der Pipe gelesen#
                    read(fd[0], &arr, sizeof(int) * n);
                                    
                    // Summe des Arrays wird bestimmt
                    int sum = 0;
                    for (i = 0; i < n; i++) {
                        sum += arr[i];
                        
                    }
                                        
                    // Summe wird in die Pipe geschrieben und Lese Pipe und Schreibe Pipe wird geschlossen
                    write(fd[1], &sum, sizeof(sum));
                    close(fd[0]);
                    close(fd[1]);
                                        
                    // Mittelwert wird berechnet und in 2. Pipe geschrieben und anschließend geschlossen
                    float mittelwert;
                                    
                    mittelwert = (float)sum / n;
                                        
                    write(fd2[1], &mittelwert, sizeof(mittelwert));
                    close(fd2[1]);
                    exit(0);
                }
            } else {
                
                //REPORT
                                
                // wait() um auf den Kindprozess (STAT) zu warten
                wait(NULL);
                int sum;
                float mittelwert;
                                
                                
                // Summe und Mittelwert werden aus den 2 Pipes gelesen und die Pipes werden anschließend geschlossen
                read(fd[0], &sum, sizeof(sum));
                printf("Erhaltene Summe in REPORT: %d", sum);
                printf("\n");
                close(fd[0]);
                                
                read(fd2[0], &mittelwert, sizeof(mittelwert));
                printf("Erhaltener Mittelwert in REPORT: %f", mittelwert);
                printf("\n");
                printf("\n");
                close(fd2[0]);
                exit(0);
            }
        }
        // lässt die schleife eine Sekunde lang warten, damit das System zeit hat neue zahlen zu generieren
        sleep(1);
    }
}
