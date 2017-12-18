# Edit this if your computer has SDL set up differently.

.PHONY: chaos fern

chaos:
	gcc -std=c99 -pedantic -Wall -O3 chaos.c display.c list.c -lm -lSDL2 -o chaos
	
fern:
	gcc -std=c99 -pedantic -Wall -O3 fern.c display.c -lSDL2 -o fern