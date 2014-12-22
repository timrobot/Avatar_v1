CCFLAGS = `pkg-config --cflags opencv`
INCLUDE = -I/usr/X11R6/include
LIBS = -L/usr/X11R6/lib -lX11 `pkg-config --libs opencv`

all:
	g++ -c AvieVideo.cpp $(INCLUDE)
	g++ -c test.cpp
	g++ -o test AvieVideo.o test.o $(INCLUDE) $(LIBS)

clean:
	rm -f *o test
