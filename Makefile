
all: autocomplete

clean:
	rm -f autocomplete
	@rm -f *~
	@rm -f *.o


autocomplete: main.c autocomplete.c autocomplete.h tern.c tern.h
	gcc --std=c11 -g -o autocomplete main.c autocomplete.c tern.c -lncurses
