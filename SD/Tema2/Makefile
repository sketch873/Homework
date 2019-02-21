build: minimax

minimax: arbore.c aux1.c aux2.c
	gcc -Wall $^ -o minimax

clean:
	rm -f minimax
zip: tema.zip

tema.zip: arbore.c arbore.h aux1.c aux2.c Makefile Readme
	zip tema.zip $^
