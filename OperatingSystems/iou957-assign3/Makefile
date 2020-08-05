all: part1 part2

part1: part1.c
	gcc -o part1 part1.c

part2: part2.c pagetable.c phypages.c
	gcc -o part2 part2.c pagetable.c phypages.c -I.

clean: 
	rm pagetable.o
	rm phypages.o