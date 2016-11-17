# Group
TEAM = ae11

CC = gcc
INCLUDE = .
CFLAGS = -std=c99 -Wall
DEFINES =
OBJS = teste.o grafo.o

ifndef DEBUG
	CFLAGS += -O3
else
	DEFINES = -DDEBUG
	CFLAGS += -g
	OBJS += debug.o
endif

.SUFFIXES:		.c
.c.o:
	$(CC) $(DEFINES) $(CFLAGS) -I$(INCLUDE) -c $<

all: teste

teste: $(OBJS)
	$(CC) -o $(@) $^ -l cgraph

grafo.o: grafo.c grafo.h

clean:
	-rm -rf *.o teste

clean-all:
	-rm -rf *.o teste *.tar.gz $(TEAM) Debug Release

tar:
	-mkdir --parents $(TEAM) && \
	cp --archive grafo.c readme.txt $(TEAM) && \
	tar czvf $(TEAM).tar.gz $(TEAM) && \
	rm -rf $(TEAM)
