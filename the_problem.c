#include <stdio.h>  //std libaries
#include <stdlib.h>     
#include <time.h>   //für Zufallsgenerator
#include <unistd.h>     //Linux Libary
#include <pthread.h>    //für Threads
#include <semaphore.h>  //für Threads
#include <string.h>     //für String Befehle (puts)

void *student(void *arg) {
	puts("Hier ist ein Student");
}

void *dean(void *arg) {
	puts("Hier ist der DEAN");
}

int main()
{
	pthread_t tid0,tid1;

	pthread_create(&tid1, NULL, dean, NULL);        //Started Dean Thread
	pthread_create(&tid0, NULL, student, NULL);     //Startet Student Thread

	return 0;
}
