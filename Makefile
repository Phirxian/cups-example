CC=g++
LN=g++
OPT=-std=gnu++17 -Werror -fPIC
LIB=-lm -lstdc++ -lpthread

SRC =src/network/address.cpp src/network/file_descriptor.cpp  src/network/poller.cpp src/network/socket.cpp src/network/timestamp.cpp
SRC+=src/cups/state/IsPrintingState.cpp src/cups/state/OffState.cpp src/cups/state/OnState.cpp src/cups/state/SuspendState.cpp
SRC+=src/cups/User.cpp src/cups/Spooler.cpp src/cups/Printer.cpp

OBJR=$(SRC:src/%.cpp=obj/release/%.o)
OBJD=$(SRC:src/%.cpp=obj/debug/%.o)

all: OPT += -Os -O3
all: $(OBJR)
	$(LN) $(OPT) $(LIB) $(OBJR) src/server.cpp -o server
	$(LN) $(OPT) $(LIB) $(OBJR) src/client.cpp -o client
	$(LN) $(OPT) $(LIB) $(OBJR) src/printer.cpp -o printer
	#$(LN) $(OPT) $(LIB) $(OBJR) src/test.cpp -o test
  
debug: OPT += -g -pg -D__DEBUG
debug: $(OBJD)
	$(LN) $(OPT) $(LIB) $(OBJD) src/server.cpp -o serverd
	$(LN) $(OPT) $(LIB) $(OBJD) src/client.cpp -o clientd
	$(LN) $(OPT) $(LIB) $(OBJD) src/printer.cpp -o printerd
	#$(LN) $(OPT) $(LIB) $(OBJD) src/test.cpp -o test
  
$(OBJR): obj/release/%.o : src/%.cpp
	$(CC) $(OPT) -c $< -o $@
  
$(OBJD): obj/debug/%.o : src/%.cpp
	$(CC) $(OPT) -c $< -o $@
  
clean:
	rm -f $(OBJR) $(OBJD) server client test

dir:
	mkdir -p obj/debug/cups/state
	mkdir -p obj/debug/network
	mkdir -p obj/debug/sync
	mkdir -p obj/release/cups/state
	mkdir -p obj/release/network
	mkdir -p obj/release/sync