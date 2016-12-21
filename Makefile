##	Mosgöller Bernhard & Daniel Gschwandtner
##	The Room Party Problem
##	Version 1 Makefile

-include config.mk 

CC = gcc
PRJNAME	= The_Room_Party
OBJS = the_problem.c
PFLAGS = -pthread

$(PRJNAME): $(OBJS) 
	$(CC) -o $@  $(OBJS) $(PFLAGS)

clean:
	rm -f $(PRJNAME)

.PHONY:
	rm -f $(PRJNAME)	#Phony wird aufgerufen wenn es ein File doppelt gibt