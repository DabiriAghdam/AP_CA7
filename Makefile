CC := g++ -std=c++11

all: main.out

main.out: main.o network.o orders_handler.o customer.o publisher.o film.o comment.o notification.o
	$(CC) main.o network.o orders_handler.o customer.o publisher.o film.o comment.o notification.o -o main.out

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

notification.o: notification.cpp
	$(CC) -c notification.cpp -o notification.o

comment.o: comment.cpp
	$(CC) -c comment.cpp -o comment.o

film.o: film.cpp
	$(CC) -c film.cpp -o film.o

publisher.o: publisher.cpp
	$(CC) -c publisher.cpp -o publisher.o

customer.o: customer.cpp
	$(CC) -c customer.cpp -o customer.o

orders_handler.o: orders_handler.cpp
	$(CC) -c orders_handler.cpp -o orders_handler.o

network.o: network.cpp
	$(CC) -c network.cpp -o network.o

.PHONY: clean
clean:
	rm -r *.o