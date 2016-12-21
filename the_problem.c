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
	puts("Hier ist der DEAN");
}

int main()
{
	pthread_t tid0,tid1;

	pthread_create(&tid1, NULL, dean, NULL);        //Started Dean Thread
	pthread_create(&tid0, NULL, student, NULL);     //Startet Student Thread

	pthread_join(tid0, NULL);
	pthread_join(tid1, NULL);

	return 0;
}
