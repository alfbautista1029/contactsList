CFLAGS=-Wall -g -std=c17

all:
	gcc $(CFLAGS) contacts.c database.c -o contacts
clean:
	rm -f contacts
