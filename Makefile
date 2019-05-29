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
	$(BUILD_DIR)/response.o \
	$(BUILD_DIR)/request.o \
	$(BUILD_DIR)/utilities.o \
	$(BUILD_DIR)/server.o \
	$(BUILD_DIR)/route.o \
	$(BUILD_DIR)/handlers.o \
	$(BUILD_DIR)/http_server.o \
	$(BUILD_DIR)/md5.o 
	
all: $(BUILD_DIR) $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE_FILE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE_FILE)

$(BUILD_DIR)/main.o: $(SRC_DIR)/core/main.cpp $(SRC_DIR)/core/network.h
	$(CC) -c $(SRC_DIR)/core/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/response.o: $(SRC_DIR)/utils/response.cpp $(SRC_DIR)/utils/response.hpp $(SRC_DIR)/utils/include.hpp
	$(CC) $(CF) -c $(SRC_DIR)/utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DIR)/request.o: $(SRC_DIR)/utils/request.cpp $(SRC_DIR)/utils/request.hpp $(SRC_DIR)/utils/include.hpp $(SRC_DIR)/utils/utilities.hpp
	$(CC) $(CF) -c $(SRC_DIR)/utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/utilities.o: $(SRC_DIR)/utils/utilities.cpp $(SRC_DIR)/utils/utilities.hpp
	$(CC) $(CF) -c $(SRC_DIR)/utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/server.o: $(SRC_DIR)/server/server.cpp $(SRC_DIR)/server/server.hpp $(SRC_DIR)/server/route.hpp $(SRC_DIR)/utils/utilities.hpp \
$(SRC_DIR)/utils/response.hpp $(SRC_DIR)/utils/request.hpp $(SRC_DIR)/utils/include.hpp
	$(CC) $(CF) -c $(SRC_DIR)/server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/route.o: $(SRC_DIR)/server/route.cpp $(SRC_DIR)/server/route.hpp $(SRC_DIR)/utils/utilities.hpp \
$(SRC_DIR)/utils/response.hpp $(SRC_DIR)/utils/request.hpp $(SRC_DIR)/utils/include.hpp
	$(CC) $(CF) -c $(SRC_DIR)/server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DIR)/notification.o: $(SRC_DIR)/core/notification.cpp $(SRC_DIR)/core/notification.h
	$(CC) -c $(SRC_DIR)/core/notification.cpp -o $(BUILD_DIR)/notification.o

$(BUILD_DIR)/comment.o: $(SRC_DIR)/core/comment.cpp $(SRC_DIR)/core/comment.h
	$(CC) -c $(SRC_DIR)/core/comment.cpp -o $(BUILD_DIR)/comment.o

$(BUILD_DIR)/film_repository.o: $(SRC_DIR)/core/film_repository.cpp $(SRC_DIR)/core/film_repository.h $(SRC_DIR)/core/film.h
	$(CC) -c $(SRC_DIR)/core/film_repository.cpp -o $(BUILD_DIR)/film_repository.o

$(BUILD_DIR)/film.o: $(SRC_DIR)/core/film.cpp $(SRC_DIR)/core/film.h $(SRC_DIR)/core/comment.h $(SRC_DIR)/core/exception.h
	$(CC) -c $(SRC_DIR)/core/film.cpp -o $(BUILD_DIR)/film.o

$(BUILD_DIR)/publisher.o: $(SRC_DIR)/core/publisher.cpp $(SRC_DIR)/core/publisher.h $(SRC_DIR)/core/customer.h
	$(CC) -c $(SRC_DIR)/core/publisher.cpp -o $(BUILD_DIR)/publisher.o

$(BUILD_DIR)/customer.o: $(SRC_DIR)/core/customer.cpp $(SRC_DIR)/core/customer.h $(SRC_DIR)/core/film.h $(SRC_DIR)/core/notification.h
	$(CC) -c $(SRC_DIR)/core/customer.cpp -o $(BUILD_DIR)/customer.o

$(BUILD_DIR)/user_repository.o: $(SRC_DIR)/core/user_repository.cpp $(SRC_DIR)/core/user_repository.h $(SRC_DIR)/core/customer.h
	$(CC) -c $(SRC_DIR)/core/user_repository.cpp -o $(BUILD_DIR)/user_repository.o

$(BUILD_DIR)/commands_handler.o: $(SRC_DIR)/core/commands_handler.cpp $(SRC_DIR)/core/commands_handler.h $(SRC_DIR)/core/exception.h
	$(CC) -c $(SRC_DIR)/core/commands_handler.cpp -o $(BUILD_DIR)/commands_handler.o

$(BUILD_DIR)/handlers.o: $(SRC_DIR)/core/handlers.cpp $(SRC_DIR)/server/server.hpp $(SRC_DIR)/utils/utilities.hpp $(SRC_DIR)/utils/response.hpp $(SRC_DIR)/utils/request.hpp $(SRC_DIR)/utils/include.hpp
	$(CC) $(CF) -c  $(SRC_DIR)/core/handlers.cpp -o $(BUILD_DIR)/handlers.o

$(BUILD_DIR)/network.o: $(SRC_DIR)/core/network.cpp $(SRC_DIR)/core/network.h $(SRC_DIR)/core/publisher.h \
$(SRC_DIR)/core/film_repository.h $(SRC_DIR)/core/user_repository.h $(SRC_DIR)/core/commands_handler.h
	$(CC) -c $(SRC_DIR)/core/network.cpp -o $(BUILD_DIR)/network.o

$(BUILD_DIR)/http_server.o: $(SRC_DIR)/core/http_server.cpp $(SRC_DIR)/server/server.hpp $(SRC_DIR)/utils/utilities.hpp $(SRC_DIR)/utils/response.hpp $(SRC_DIR)/utils/request.hpp $(SRC_DIR)/utils/include.hpp
	$(CC) $(CF) -c $(SRC_DIR)/core/http_server.cpp -o $(BUILD_DIR)/http_server.o

$(BUILD_DIR)/md5.o: $(SRC_DIR)/core/md5.cpp $(SRC_DIR)/core/md5.h
	$(CC) -c $(SRC_DIR)/core/md5.cpp -o $(BUILD_DIR)/md5.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out &> /dev/null