# Group
TEAM = ae11

CC = gcc
INCLUDE = .
CFLAGS = -O3 -Wall
DEFINES =

ifdef DEBUG
	DEFINES = -DDEBUG
	CFLAGS = -g -Wall
endif

.SUFFIXES:		.c
.c.o:
	$(CC) $(DEFINES) $(CFLAGS) -I$(INCLUDE) -c $<

all: teste

teste: teste.o grafo.o
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
