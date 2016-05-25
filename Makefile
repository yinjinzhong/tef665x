#Usage: make [HOST=arm-none-linux-gnueabi- OPT=-O2] [TARGET]
	
HOST	:= arm-linux-gnueabihf-
OPT     := -g
CFLAG	:= -I./
LIBS	:= -L./

CPP     := $(HOST)g++
CC		:= $(HOST)gcc
AR		:= $(HOST)ar rsc

SRCS := tef665x-test.c
OBJS  = $(addsuffix .o, $(basename $(SRCS)))

.PHONY: all clean

all:$(OBJS)
	$(CPP) $(OPT) $(OBJS) -o radio-test

##	./scp_to_remote.sh

clean:
	rm -f $(OBJS)

%.o:%.cpp
	$(CPP) $(OPT) $(CFLAG) -c $< -o $@

%.o:%.c
	$(CC) $(OPT) $(CFLAG) -c $< -o $@

