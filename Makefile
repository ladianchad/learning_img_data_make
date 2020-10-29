
CC = g++
CFLAGS = -g -Wall
SRCS = make_data.cpp
PROG = make_data

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)