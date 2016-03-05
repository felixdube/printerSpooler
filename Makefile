LIBS := -lrt -lpthread

all: printer.o client.o

printer: printer.c
	gcc $^ -o $@ $(LIBS)

client: client.c
	gcc $^ -o $@ $(LIBS)

clean:
	-rm printer.o client.o
