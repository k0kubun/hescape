.PHONY: all compile

all: compile

compile: hescape.c hescape.h
	$(CC) -c -o hescape.o $<
