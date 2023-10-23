# Define compiler
CC = g++

# Define compiler flags
CFLAGS = -g -Wall
SRC = Source/main.cpp Source/sha256.cpp Source/sha256.h

RBFGen: $(SRC)
	echo "Creating $@"
	$(CC) $(CFLAGS) -o $@ $(SRC) 
clean:
	rm RBFGen