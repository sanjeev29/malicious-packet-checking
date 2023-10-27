# Define compiler
CC = g++

# Define compiler flags
CFLAGS = -g -Wall
SRC = Source/main.cpp Source/RBFGen.cpp Source/sha256.cpp
IP_SRC = Source/main.cpp Source/RBFGen.cpp Source/sha256.cpp Source/IPCheck.cpp

# Both have to be run separate
# Make RBFGen
RBFGen: $(SRC)
	echo "Creating RBFGen"
	$(CC) $(CFLAGS) -o RBFGen $(SRC) 

# Make IPCheck
IPCheck: $(IP_SRC)
	echo "Creating IPCheck"
	$(CC) $(CFLAGS) -o IPCheck $(IP_SRC) 

# Clean all directories; check if they exist to make sure it doesn't generate an error
clean:
	rm RBFGen IPGen.txt
