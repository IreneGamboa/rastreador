CC		=	gcc
CFLAGS	=	-std=gnu99 -g -lm
LIBS	=	-lm
CHECK	=	-Wall -Wextra

all: rastreador.o 
	$(CC) $(CHECK) $(CFLAGS) -o rastreador rastreador.o $(LIBS)

clean:
	rm -f *.o rastreador
