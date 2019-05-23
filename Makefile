CC := g++ -std=c++11
SRC_DIR = src
BUILD_DIR = build
EXECUTABLE_FILE = network.out
OBJECTS = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/network.o \
	$(BUILD_DIR)/commands_handler.o \
	$(BUILD_DIR)/customer.o \
	$(BUILD_DIR)/publisher.o \
	$(BUILD_DIR)/user_repository.o \
	$(BUILD_DIR)/film.o \
	$(BUILD_DIR)/film_repository.o \
	$(BUILD_DIR)/comment.o \
	$(BUILD_DIR)/notification.o \
	$(BUILD_DIR)/md5.o \

all: $(BUILD_DIR) $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE_FILE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE_FILE)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/network.h
	$(CC) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/notification.o: $(SRC_DIR)/notification.cpp $(SRC_DIR)/notification.h
	$(CC) -c $(SRC_DIR)/notification.cpp -o $(BUILD_DIR)/notification.o

$(BUILD_DIR)/comment.o: $(SRC_DIR)/comment.cpp $(SRC_DIR)/comment.h
	$(CC) -c $(SRC_DIR)/comment.cpp -o $(BUILD_DIR)/comment.o

$(BUILD_DIR)/film_repository.o: $(SRC_DIR)/film_repository.cpp $(SRC_DIR)/film_repository.h $(SRC_DIR)/film.h
	$(CC) -c $(SRC_DIR)/film_repository.cpp -o $(BUILD_DIR)/film_repository.o

$(BUILD_DIR)/film.o: $(SRC_DIR)/film.cpp $(SRC_DIR)/film.h $(SRC_DIR)/comment.h $(SRC_DIR)/exception.h
	$(CC) -c $(SRC_DIR)/film.cpp -o $(BUILD_DIR)/film.o

$(BUILD_DIR)/publisher.o: $(SRC_DIR)/publisher.cpp $(SRC_DIR)/publisher.h $(SRC_DIR)/customer.h
	$(CC) -c $(SRC_DIR)/publisher.cpp -o $(BUILD_DIR)/publisher.o

$(BUILD_DIR)/customer.o: $(SRC_DIR)/customer.cpp $(SRC_DIR)/customer.h $(SRC_DIR)/film.h $(SRC_DIR)/notification.h
	$(CC) -c $(SRC_DIR)/customer.cpp -o $(BUILD_DIR)/customer.o

$(BUILD_DIR)/user_repository.o: $(SRC_DIR)/user_repository.cpp $(SRC_DIR)/user_repository.h $(SRC_DIR)/customer.h
	$(CC) -c $(SRC_DIR)/user_repository.cpp -o $(BUILD_DIR)/user_repository.o

$(BUILD_DIR)/commands_handler.o: $(SRC_DIR)/commands_handler.cpp $(SRC_DIR)/commands_handler.h $(SRC_DIR)/exception.h
	$(CC) -c $(SRC_DIR)/commands_handler.cpp -o $(BUILD_DIR)/commands_handler.o

$(BUILD_DIR)/network.o: $(SRC_DIR)/network.cpp $(SRC_DIR)/network.h $(SRC_DIR)/publisher.h \
$(SRC_DIR)/film_repository.h $(SRC_DIR)/user_repository.h $(SRC_DIR)/commands_handler.h
	$(CC) -c $(SRC_DIR)/network.cpp -o $(BUILD_DIR)/network.o

$(BUILD_DIR)/md5.o: $(SRC_DIR)/md5.cpp $(SRC_DIR)/md5.h
	$(CC) -c $(SRC_DIR)/md5.cpp -o $(BUILD_DIR)/md5.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out