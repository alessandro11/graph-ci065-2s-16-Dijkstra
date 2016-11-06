# Group
TEAM = ae11

CC = gcc
INCLUDE = .
CFLAGS = -O3 -Wall

ifdef DEBUG
	CFLAGS = -g -Wall
endif

.SUFFIXES:		.c
.c.o:
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $<

all: teste

teste: teste.o grafo.o
	$(CC) -o $(@) $^

grafo.o: grafo.c grafo.h

clean:
	-rm -rf *.o teste

clean-all:
	-rm -rf *.o teste *.tar.gz $(TEAM)

tar:
	-mkdir --parents $(TEAM) && \
	cp --archive grafo.c readme.txt $(TEAM) && \
	tar czvf $(TEAM).tar.gz $(TEAM) && \
	rm -rf $(TEAM)
