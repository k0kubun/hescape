LD=g++
OBJS=test/html_escape.o hescape.o
.PHONY: all test

all: hescape.o

hescape.o: hescape.c hescape.h
	$(CC) -c -o $@ $<

test: test/html_escape
	test/html_escape

test/html_escape: $(OBJS)
	$(LD) $(OBJS) -o $@

test/html_escape.o: test/html_escape.c test/test_helper.h hescape.h
	$(CC) -c -o $@ $<
