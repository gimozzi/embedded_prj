CC := arm-none-linux-gnueabi-gcc
TARGET = app
SOURCE = app.c
OBJECTS = app.o

all : $(OBJECTS)
	$(CC) -static -o $(TARGET) $(OBJECTS)

app.o : app.c
	$(CC) -c app.c -Wall

clean :
	rm -rf $(TARGET) $(OBJECTS)
