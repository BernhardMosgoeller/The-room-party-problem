#include <stdio.h>  //std libaries
#include <stdlib.h>     
#include <time.h>   //für Zufallsgenerator
#include <unistd.h>     //Linux Libary
#include <pthread.h>    //für Threads
#include <semaphore.h>  //für Threads
#include <string.h>     //für String Befehle (puts)
 

sem_t sem_mos;  //semaphore wird als Mutex benutzt
int number_of_students=0;   //gibt die Zahl der Studenten an im Raum Dean wird nicht mitgezählt
int dean_in_room=0;     //Zeigt an ob der Dean im Raum ist oder nicht
int party_over=0;       //Zeigt an wann der Party vorbei ist also Keine Studenten sind da

void *student(void *arg) {

    if(dean_in_room==0) //schaut ob Dean im Raum ist
    {
        int x=10,y=200,random;  //Variablen für Zufallszahl

        sem_wait(&sem_mos);    //down semaphore, Kritische Zone wird betreten
        number_of_students++;   //Zahl der Studenten im Raum wird um 1 erhöht
        printf("Kritisches Zone Zahl der Studenten im Raum: \t%d\n",number_of_students);    //gibt die neue Anzahl an
        sem_post(&sem_mos);   //up semaphore, Kritische Zone wird verlassen
        printf("*Student hat den Raum betreten*\n\n");

        //srand (time (NULL));    //Zufallsgenerator initialisieren NICHT ansonsten bei jedem thread selbe zahl
        random = (rand () % ((y + 1) - x)) + x;     //bekommt zufällige zahl zwischen x und y
        //printf("Time \t\t %d\n",random);

        sleep(random);  //wartet eine Zufällige Zahl lang

        sem_wait(&sem_mos); //down semaphore, Kritische Zone wird betreten
         number_of_students--; //Zahl der Studenten im Raum wird um 1 verringert
        printf("Kritisches Zone, Zahl der Studenten im Raum: \t%d\n",number_of_students);   //gibt die neue Anzahl an
        sem_post(&sem_mos);   //up semaphore
        printf("*Student hat den Raum verlassen*\n\n");
    }
    else
    {
        printf("Keine Erlaubnis zum Betreten des Raums\n"); //Dean ist im Raum!
    }

    pthread_exit(0); /* exit thread */
}

void *dean(void *arg) {
    puts("Dean Start");
    if(number_of_students==0||number_of_students>=50)   //Dean darf nur in den Raum wenn mehr als 50 Studentne im Raum sind oder 0
    {
        dean_in_room=1; //Setzt Flag,damit keine neue Studenten mehr kommen
        puts("Party Crash, keine neue Studenten dürfen kommen");
        puts("Warten bis keine Studenten mehr im Raum sind");
        while(number_of_students!=0)    //Wartet bis alle Studenten den Raum verlassen haben 
            sleep(1);
        puts("Keine Studenten mehr im Raum\n");
        party_over=1;   // Flag damit Studenten weiter probieren zu kommen, gibt Meldung aus
    }
    else   
        puts("Nicht erlaubt, die Party zum Crashen");

    puts("");//Leerzeile
}


int main()
{

    /*random Zahl bestimmt ob der Dean kommt oder nicht*/
    int x = 1, y = 10;
    int random;
    int i;
    pthread_t tid0,tid1;

    sem_init(&sem_mos, 0, 1);  //Initalisiert Semaphore 

    srand (time (NULL));    //Zufallsgenerator initialisieren
    while(party_over!=1)
    {
        random = (rand () % ((y + 1) - x)) + x;     //bekommt zufällige zahl
        //printf("Zufallszahl %d\n",random);

        if(random==8)
        {
            if(dean_in_room==0)//es gibt keine 2 Deans
            pthread_create(&tid1, NULL, dean, NULL);        //Started Dean Thread
        }
        else
        {
            pthread_create(&tid0, NULL, student, NULL);     //Startet Student Thread
        }
        sleep(1);
    }
    pthread_join(tid1, NULL);//  Zeile unnätig weil die While Schleife aufhört sobald der Dean fertig ist, und Dean hört erst auf wenn kein Student mehr im Raum ist

  return 0;
}

