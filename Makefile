build:main.c x-o_functions.c
	gcc -std=c99 -Wall -Wextra x-o_functions.c main.c -o gigel
run:build
	./gigel
clean:
	rm -f gigel
