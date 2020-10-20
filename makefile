CC=gcc
CFLAGS= -Wall -finput-charset="UTF-8" -fexec-charset="UTF-8"
LDFLAGS=
EXEC=main.out

all: $(EXEC) execute

main: voyageur.o gare.o main.c
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf *.h.gch
	rm main
	rm main.out

execute:
	./$(EXEC)
