icalgen_old: icalgen_old.c
	gcc -Wall icalgen_old.c -o icalgen_old -lncurses

icalgen: icalgen.c funcs.c
	gcc -Wall icalgen.c funcs.c -o icalgen -lncurses -lform
