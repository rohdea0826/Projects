ROOT_PATH=$(shell pwd)
CLIENT=$(ROOT_PATH)/client
SERVER=$(ROOT_PATH)/server
DATAPOOL=$(ROOT_PATH)/data_pool
COMM=$(ROOT_PATH)/comm
LIB=$(ROOT_PATH)/lib
WINDOW=$(ROOT_PATH)/window

INCLUDE=-I$(DATAPOOL) -I$(COMM) -I$(LIB)/include -I$(WINDOW)
JSONLIB=-L$(LIB)/lib

SER_SRC=$(shell ls $(SERVER)/*.cpp $(DATAPOOL)/*.cpp $(COMM)/*.cpp| sed 's/\/.*\///g')
CLI_SRC=$(shell ls $(CLIENT)/*.cpp $(COMM)/*.cpp $(WINDOW)/*.cpp| sed 's/\/.*\///g')

SER_OBJ=$(shell echo $(SER_SRC) | sed 's/.cpp/.o/g')
CLI_OBJ=$(shell echo $(CLI_SRC) | sed 's/.cpp/.o/g')

LDFLAGS=-lpthread -ljsoncpp

ser=chatSystem
cli=chatClient
cc=g++

.PHONY:all
all:$(ser) $(cli)

$(ser):$(SER_OBJ)
	$(cc) -o $@ $^ $(LDFLAGS) $(JSONLIB)
$(cli):$(CLI_OBJ)
	$(cc) -o $@ $^ $(LDFLAGS) $(JSONLIB) -lncurses
%.o:$(CLIENT)/%.cpp
	$(cc) -c $< $(INCLUDE)
%.o:$(SERVER)/%.cpp
	$(cc) -c -std=c++11 $< $(INCLUDE)
%.o:$(DATAPOOL)/%.cpp
	$(cc) -c $<
%.o:$(COMM)/%.cpp
	$(cc) -c $< $(INCLUDE)
%.o:$(WINDOW)/%.cpp
	$(cc) -c $< $(INCLUDE)

.PHONY:clean
clean:
	rm -f $(ser) $(cli) *.o

.PHONY:debug
debug:
	@echo $(SER_OBJ)
	@echo $(CLI_OBJ)






