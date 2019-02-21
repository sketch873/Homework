#!/bin/bash
build: hangman

run: hangman
	./$< ${args}

hangman: citire.o verificare_argumente.o grafica.o hangman.o
	gcc -Wall $^ -o hangman -lcurses -lm

clean:
	rm -f hangman *.o settings savegame

hangman.o: hangman.c
	gcc $< -c

citire.o: citire.c
	gcc $< -c

grafica.o: grafica.c
	gcc $< -c

verificare_argumente.o: verificare_argumente.c
	gcc $< -c
