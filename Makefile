CC = g++
CCFLAGS = `pkg-config --cflags opencv`
INCLUDE = -I/usr/X11R6/include \
				-I/usr/include/festival -I/usr/include/speech_tools
LIBS = -L/usr/X11R6/lib -lX11 \
				`pkg-config --libs opencv` \
				-L/usr/lib/festival -lFestival -lestools -lestbase -leststring

OBJECTS = AvieWindow.o AvieAudio.o AvieCamera.o test.o
TARGET = test

all: $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) $^ -o $@ $(INCLUDE) $(LIBS)

test.o: test.cpp
	$(CC) $(CCFLAGS) -c $< -o $@ $(INCLUDE)

AvieWindow.o: AvieWindow.cpp
	$(CC) $(CCFLAGS) -c $< -o $@ $(INCLUDE)

AvieAudio.o: AvieAudio.cpp
	$(CC) $(CCFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -f $(OBJECTS) $(TARGET)
