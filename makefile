CC=gcc
CFLAGS= -Wall
LDFLAGS=
EXEC=main.out

all: $(EXEC) execute

main: gare.o main.c
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf *.h.gch
	rm main
	rm main.out

execute:
	./main.out
