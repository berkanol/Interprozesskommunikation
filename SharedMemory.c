// Erstellt von Cem Sögüt
// Matrikelnummer: 1392940


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#define MAXMEMSIZE 20
#include <sys/sem.h>
#include<signal.h>

char *sharedmempointer_conv;
int *sharedmempointer_stat;
int *sharedmempointer_report;

int shared_memory_key_conv = 12345;
int shared_memory_key_stat = 54321;
int shared_memory_key_report = 11111;

int key_lock_conv=12345;
int key_unlock_conv=54321;
int key_lock_stat = 56789;
int key_unlock_stat = 98765;
int key_lock_report = 11112;
int key_unlock_report = 11113;

int rc_semget_conv_lock;
int rc_semget_conv_unlock;
int rc_semget_stat_lock;
int rc_semget_stat_unlock;
int rc_semget_report_lock;
int rc_semget_report_unlock;

int returncode_shmget_conv;
int returncode_shmget_stat;
int returncode_shmget_report;


// ein Signalhädnler wird erstellt für das Beenden des Programms
void sig_handler(int signum){
    
    shmdt(sharedmempointer_conv);
    shmctl(returncode_shmget_conv, IPC_RMID, 0);
    shmdt(sharedmempointer_stat);
    shmctl(returncode_shmget_stat, IPC_RMID, 0);
    shmdt(sharedmempointer_report);
    shmctl(returncode_shmget_report, IPC_RMID, 0);
    
    semctl(rc_semget_conv_lock, 0, IPC_RMID, 0);
    semctl(rc_semget_conv_unlock, 0, IPC_RMID, 0);
    semctl(rc_semget_stat_lock, 0, IPC_RMID, 0);
    semctl(rc_semget_stat_unlock, 0, IPC_RMID, 0);
    semctl(rc_semget_report_lock, 0, IPC_RMID, 0);
    semctl(rc_semget_report_unlock, 0, IPC_RMID, 0);
    
    
}





int main(int argc, const char * argv[]) {
    
    
    signal(SIGINT,sig_handler); // Register signal handler
    
    
    
    // die Shared Memories werden erstellt
    
    returncode_shmget_conv = shmget(shared_memory_key_conv,
                                    MAXMEMSIZE,
                                    IPC_CREAT | 0600);
    
    if (returncode_shmget_conv < 0) {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    } else {
        printf("Das Segment wurde erstellt.\n");
    }
    
    // Gemeinsames Speichersegment anhängen
    sharedmempointer_conv = shmat(returncode_shmget_conv, 0, 0);
    if (sharedmempointer_conv==(char *)-1) {
        printf("Das Segment konnte nicht angehÃ¤ngt werden.\n");
        perror("shmat");
        exit(1);
    } else {
        printf("Das Segment wurde angehÃ¤ngt.\n");
    }
    
    
    // zweite Shared Memory für den Stat Prozess
    returncode_shmget_stat = shmget(shared_memory_key_stat,
                                    sizeof(int),
                                    IPC_CREAT | 0600);
    
    if (returncode_shmget_stat < 0) {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    } else {
        printf("Das Segment wurde erstellt.\n");
    }
    
    // Gemeinsames Speichersegment anhängen
    sharedmempointer_stat = (int*) shmat(returncode_shmget_stat, 0, 0);
    if (sharedmempointer_stat== (int*)-1) {
        printf("Das Segment konnte nicht angehÃ¤ngt werden.\n");
        perror("shmat");
        exit(1);
    } else {
        printf("Das Segment wurde angehÃ¤ngt.\n");
    }
    
    // dritte Shared Memory für den Report Prozess
    returncode_shmget_report = shmget(shared_memory_key_report,
                                    sizeof(int)*2,
                                    IPC_CREAT | 0600);
    
    if (returncode_shmget_report < 0) {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    } else {
        printf("Das Segment wurde erstellt.\n");
    }
    
    // Gemeinsames Speichersegment anhängen
    sharedmempointer_report = (int*)shmat(returncode_shmget_report, 0, 0);
    if (sharedmempointer_report == (int*) -1) {
        printf("Das Segment konnte nicht angehÃ¤ngt werden.\n");
        perror("shmat");
        exit(1);
    } else {
        printf("Das Segment wurde angehÃ¤ngt.\n");
    }
    
    
    // Semaphorengruppe für conv erstellt
    rc_semget_conv_lock = semget(key_lock_conv, 1, IPC_CREAT  | 0600);
    if (rc_semget_conv_lock < 0) {
        printf("Die Semaphorgruppe konnte nicht erstellt werden.\n");
        perror("semget");
        // Programmabbruch
        exit(1);
    } else {
        printf("Semaphorgruppe %i mit Key %i wurde erstellt.\n",
               rc_semget_conv_lock, key_lock_conv);
    }
    
    // Neue Semaphorgruppe 54321 mit einer Semaphore erstellen
    rc_semget_conv_unlock = semget(key_unlock_conv, 1, IPC_CREAT | 0600);
    if (rc_semget_conv_unlock < 0) {
        printf("Die Semaphorgruppe konnte nicht erstellt werden.\n");
        perror("semget");
        // Programmabbruch
        exit(1);
    } else {
        printf("Semaphorgruppe %i mit Key %i wurde erstellt.\n",
               rc_semget_conv_unlock, key_unlock_conv);
    }
    
    
    
    // P-Operation definieren. Wert der Semaphore um eins dekrementieren
    struct sembuf p_operation = {0, -1, 0};
    
    // V-Operation definieren. Wert der Semaphore um eins inkrementieren
    struct sembuf v_operation = {0, 1, 0};
    
    
    // Erste Semaphore der Semaphorgruppe 12345 initial auf Wert 1 setzen
    int rc_semctl = semctl(rc_semget_conv_lock, 0, SETVAL, 1);
    if (rc_semctl < 0) {
        printf("Der Wert von %i konnte nicht gesetzt werden.\n", rc_semget_conv_lock);
        perror ("semctl SETVAL");
        exit (1);
    }
    
    //  Semaphore der Semaphorgruppe 54321 initial auf Wert 0 setzen
    rc_semctl = semctl(rc_semget_conv_unlock, 0, SETVAL, 0);
    if (rc_semctl < 0) {
        printf("Der Wert von %i konnte nicht gesetzt werden.\n", rc_semget_conv_unlock);
        perror ("semctl SETVAL");
        exit (1);
    }
    
    
    
    
    
    // eine weitere Semaphorgruppe 54321 mit einer Semaphore erstellen
    rc_semget_stat_lock = semget(key_lock_stat, 1, IPC_CREAT | 0600);
    if (rc_semget_stat_lock < 0) {
        printf("Die Semaphorgruppe konnte nicht erstellt werden.\n");
        perror("semget");
        // Programmabbruch
        exit(1);
    } else {
        printf("Semaphorgruppe %i mit Key %i wurde erstellt.\n",
               rc_semget_stat_lock, key_lock_stat);
    }
    
    
    // eine weitere Semaphorgruppe mit einer Semaphore erstellen
    rc_semget_stat_unlock = semget(key_unlock_stat, 1, IPC_CREAT | 0600);
    if (rc_semget_stat_unlock < 0) {
        printf("Die Semaphorgruppe konnte nicht erstellt werden.\n");
        perror("semget");
        // Programmabbruch
        exit(1);
    } else {
        printf("Semaphorgruppe %i mit Key %i wurde erstellt.\n",
               rc_semget_stat_unlock, key_unlock_stat);
    }
    
    
    
    // Erste Semaphore der Semaphorgruppe 12345 initial auf Wert 1 setzen
    rc_semctl = semctl(rc_semget_stat_lock, 0, SETVAL, 1);
    if (rc_semctl < 0) {
        printf("Der Wert von %i konnte nicht gesetzt werden.\n", key_lock_stat);
        perror ("semctl SETVAL");
        exit (1);
    }
    
    // Erste Semaphore der Semaphorgruppe 54321 initial auf Wert 0 setzen
    rc_semctl = semctl(rc_semget_stat_unlock, 0, SETVAL, 0);
    if (rc_semctl < 0) {
        printf("Der Wert von %i konnte nicht gesetzt werden.\n", key_unlock_stat);
        perror ("semctl SETVAL");
        exit (1);
    }
    
    rc_semget_report_lock = semget(key_lock_report, 1, IPC_CREAT | 0600);
    if (rc_semget_conv_lock < 0) {
        printf("Die Semaphorgruppe konnte nicht erstellt werden.\n");
        perror("semget");
        // Programmabbruch
        exit(1);
    } else {
        printf("Semaphorgruppe %i mit Key %i wurde erstellt.\n",
               rc_semget_report_lock, key_lock_report);
    }
    
    
    // Neue Semaphorgruppe 54321 mit einer Semaphore erstellen
    rc_semget_report_unlock = semget(key_unlock_report, 1,  IPC_CREAT | 0600);
    if (rc_semget_report_unlock < 0) {
        printf("Die Semaphorgruppe konnte nicht erstellt werden.\n");
        perror("semget");
        // Programmabbruch
        exit(1);
    } else {
        printf("Semaphorgruppe %i mit Key %i wurde erstellt.\n",
               rc_semget_report_unlock, key_unlock_report);
    }
    
    // Erste Semaphore der Semaphorgruppe 12345 initial auf Wert 1 setzen
    rc_semctl = semctl(rc_semget_report_lock, 0, SETVAL, 1);
    if (rc_semctl < 0) {
        printf("Der Wert von %i konnte nicht gesetzt werden.\n", key_lock_report);
        perror ("semctl SETVAL");
        exit (1);
    }
    
    // Erste Semaphore der Semaphorgruppe 54321 initial auf Wert 0 setzen
    rc_semctl = semctl(rc_semget_report_unlock, 0, SETVAL, 0);
    if (rc_semctl < 0) {
        printf("Der Wert von %i konnte nicht gesetzt werden.\n", key_unlock_report);
        perror ("semctl SETVAL");
        exit (1);
    }
    
    int conv_pid = fork();
    if ( conv_pid == 0 ) {
        for(;;)
        {
            int randomzahl = rand() %11;
            
            // Schreibe Zufallszahl in Conv Sharedmem
            semop(rc_semget_conv_lock, &p_operation, 1);
            // Eine Zeichenkette in das gemeinsame Speichersegment schreiben.
            sprintf(sharedmempointer_conv, "%d", randomzahl);
            sleep(1);
            semop(rc_semget_conv_unlock, &v_operation, 1);

            // Schreibe Zufallszahl in Stat Sharedmem
            semop(rc_semget_stat_lock, &p_operation, 1);
            *sharedmempointer_stat = randomzahl;
            sleep(1);
            semop(rc_semget_stat_unlock, &v_operation, 1);
        }
        
    }
    
    int stat_pid = fork();
    if ( stat_pid == 0 ) {
        int summe = 0;
        int werte = 0;
        for(;;)
        {
            // Ermittle Anzahl an Werte und Summe aller Werte
            semop(rc_semget_stat_unlock, &p_operation, 1);
            werte++;
            summe += *sharedmempointer_stat;
            semop(rc_semget_stat_lock, &v_operation, 1);

            // füge die Ergbenisse in Conv-Segment
            semop(rc_semget_report_lock, &p_operation, 1);
            sharedmempointer_report[0] = summe/werte;
            sharedmempointer_report[1] = summe;
            semop(rc_semget_report_unlock, &v_operation, 1);
        }
    }
    
    int report_pid = fork();
    if ( report_pid == 0 ) {
        for(;;)
        {
            semop(rc_semget_report_unlock, &p_operation, 1);
            printf("Report: Mittelwert %d, Summe %d\n", sharedmempointer_report[0], sharedmempointer_report[1]);
            semop(rc_semget_report_lock, &v_operation, 1);
        }
    }
    // log Prozess
    if ( conv_pid > 0 ) {
        for (;;) {
            semop(rc_semget_conv_unlock, &p_operation, 1); // P-Operation Semaphore 12345
            FILE *file = fopen("numbers.txt", "a");
            fprintf(file, "%s\n", sharedmempointer_conv);
            printf("%s\n", sharedmempointer_conv);
            fclose(file);
            semop(rc_semget_conv_lock, &v_operation, 1); // V-Operation Semaphore 54321
        }
        
        
        
    }
    
}
