CFRAGS = -I/usr/X11R6/include
LIBS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lXmu
all: prog1 prog2
prog1: prog1.c
	$(CC) $(CFLAGS) prog1.c -o prog1 $(LIBS)
prog2: prog2.c
	$(CC) $(CFLAGS) prog2.c -o prog2 $(LIBS)