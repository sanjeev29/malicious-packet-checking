# Define compiler
CC = g++

# Define compiler flags
CFLAGS = -g -Wall
SRC = Source/main.cpp Source/sha256.cpp Source/sha256.h

RBFGen: $(SRC)
	echo "Creating $@"
	$(CC) $(CFLAGS) -o $@ $(SRC) 

# Clean all directories; check if they exist to make sure it doesn't generate an error
clean:
	rm RBFGen IPGen.txt Results/*.txt
	