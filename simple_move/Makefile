TARGET = simple_move
CC = gcc
CCP = g++ -std=c++11
CSRCS = $(wildcard ./src/*.c)
CPPSRCS = $(wildcard ./src/*.cpp)
OBJS = $(CSRCS:.c=.o) $(CPPSRCS:.cpp=.o)
DEBUG_FLAG = 
INC_PATH = 
LIB_PATH =

$(TARGET) : $(OBJS)
	$(CCP) $(DEBUG_FLAG) $^ -o $@;
	@echo "SUCCESSFULLY!!!!!!!!!!!!!"

%.o : %.c
	$(CC) $(DEBUG_FLAG) $< -c -o $@ $(INC_PATH) $(LIB_PATH)

%.o : %.cpp
	$(CCP) $(DEBUG_FLAG) $< -c -o $@ $(INC_PATH) $(LIB_PATH)

.PHONY:clean

clean:
	rm -f $(OBJS) $(TARGET)
