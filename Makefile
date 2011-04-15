CC=gcc
CFLAGS=-g

boggle: boggle.c
	$(CC) $(CFLAGS) -o boggle boggle.c

clean:
	rm -f boggle
