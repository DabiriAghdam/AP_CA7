CC := g++ -std=c++11 -Wall
SRC_DIR = src
BUILD_DIR = build
EXECUTABLE_FILE = network.out
OBJECTS = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/network.o \
	$(BUILD_DIR)/orders_handler.o \
	$(BUILD_DIR)/customer.o \
	$(BUILD_DIR)/publisher.o \
	$(BUILD_DIR)/film.o \
	$(BUILD_DIR)/comment.o \
	$(BUILD_DIR)/notification.o \

all: $(BUILD_DIR) $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE_FILE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE_FILE)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/notification.o: $(SRC_DIR)/notification.cpp
	$(CC) -c $(SRC_DIR)/notification.cpp -o $(BUILD_DIR)/notification.o

$(BUILD_DIR)/comment.o: $(SRC_DIR)/comment.cpp
	$(CC) -c $(SRC_DIR)/comment.cpp -o $(BUILD_DIR)/comment.o

$(BUILD_DIR)/film.o: $(SRC_DIR)/film.cpp
	$(CC) -c $(SRC_DIR)/film.cpp -o $(BUILD_DIR)/film.o

$(BUILD_DIR)/publisher.o: $(SRC_DIR)/publisher.cpp
	$(CC) -c $(SRC_DIR)/publisher.cpp -o $(BUILD_DIR)/publisher.o

$(BUILD_DIR)/customer.o: $(SRC_DIR)/customer.cpp
	$(CC) -c $(SRC_DIR)/customer.cpp -o $(BUILD_DIR)/customer.o

$(BUILD_DIR)/orders_handler.o: $(SRC_DIR)/orders_handler.cpp
	$(CC) -c $(SRC_DIR)/orders_handler.cpp -o $(BUILD_DIR)/orders_handler.o

$(BUILD_DIR)/network.o: $(SRC_DIR)/network.cpp
	$(CC) -c $(SRC_DIR)/network.cpp -o $(BUILD_DIR)/network.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out