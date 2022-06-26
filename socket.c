#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
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
    
    
    int sv[2], sv2[2]; /* the pair of socket descriptors */
    
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1 || socketpair(AF_UNIX, SOCK_STREAM, 0, sv2) == -1) {
        perror("socketpair");
        exit(1);
    }
    
    
    //Prozesse (prozess1) werden definiert
    
    int prozess1 = fork();                                            // Prozess 1
    
    
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
                    
                    // srand(), damit bei jedem durchlauf neue Zahle generiert werden
                    srand(time(NULL));              
                    printf("Generiert in CONV: ");
                    
                    // for-Schleife um Zufallszahlen zu generieren
                    for (i = 0; i < n; i++) {
                        arr[i] = rand() % 11;
                        printf("%d ", arr[i]);
                        
                    }
                    printf("\n");
                    
                    // Daten werden in das Socket geschrieben
                    write(sv[0], &arr, sizeof(int) * n);
                    
                } else {
                    
                    // LOG
                    
                    // wait() um auf den Kindprozess (CONV) zu warten
                    wait(NULL);
                    int arr[10];
                    int i;
                    int n = 10;
                    
                    // Daten werden aus dem Socket gelesen und in das neue Array geschrieben
                    read(sv[1], &arr, sizeof(int) * n);
                    
                    
                    // Es werden Daten reingeschrieben
                    int fddatei = open("testfifo", O_RDWR);
                    if (fddatei == -1) {
                        return 5;
                    }
                    
                    write(fddatei, &arr, sizeof(int) * n);
                    
                    // Zur Überprüfung werden die geschriebene Daten ausgelesen
                    printf("Erhalten in LOG: ");
                    for (i = 0; i < n; i++) {
                        if (read(fddatei, &arr[i], sizeof(int)) == -1) {
                            return 8;
                            
                        }
                        printf("%d ", arr[i]);
                        
                    }
                    printf("\n");
                    
                    
                    close(fddatei);
                    
                    
                    // Daten werden ins Socket geschrieben
                    write(sv[1], &arr, sizeof(int) * n);
                    
                }
            } else {
                
                //STAT
                
                // wait() um auf den Kindprozess (LOG) zu warten
                wait(NULL);
                
                int arr[10];
                int i;
                int n = 10;
                
                // Daten werden aus der Socket gelesen
                read(sv[0], &arr, sizeof(int) * n);
                
                // Zur Überprüfung was STAT von LOG erhalten hat
                printf("Erhalten in STAT: ");
                for (i = 0; i < n; i++) {
                    printf("%d ", arr[i]);
                    
                }
                printf("\n");
                
                // Summe des Arrays wird bestimmt
                int sum = 0;
                for (i = 0; i < n; i++) {
                    sum += arr[i];
                    
                }
                
                // Summe wird ins Socket geschrieben und anschließend geschlossen
                write(sv[0], &sum, sizeof(sum));
                close(sv[0]);
                
                
                // Mittelwert wird berechnet und ins 2. Socket geschrieben und anschließend geschlossen
                float mittelwert;
                
                mittelwert = (float)sum / n;
                
                write(sv2[0], &mittelwert, sizeof(mittelwert));
                close(sv2[0]);
            }
        } else {
            
            //REPORT
            
            // wait() um auf den Kindprozess (STAT) zu warten
            wait(NULL);
            int sum;
            float mittelwert;
            
            
            // Summe und Mittelwert werden aus dem Socket gelesen und anschließend geschlossen
            read(sv[1], &sum, sizeof(sum));
            printf("Erhaltene Summe in REPORT: %d", sum);
            printf("\n");
            close(sv[1]);
            
            read(sv2[1], &mittelwert, sizeof(mittelwert));
            printf("Erhaltener Mittelwert in REPORT: %f", mittelwert);
            printf("\n");
            close(sv2[1]);
            
        }
    }
    return 0;
}

