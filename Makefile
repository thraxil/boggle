CC=gcc
CFLAGS=-g -Wall -ansi

boggle: boggle.c
	$(CC) $(CFLAGS) -o boggle boggle.c

clean:
	rm -f boggle
