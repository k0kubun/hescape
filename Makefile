LD=g++
BENCHOBJS=benchmark/benchmark.o hescape.o benchmark/houdini/buffer.o benchmark/houdini/houdini_html_e.o
TESTOBJS=test/html_escape.o hescape.o
.PHONY: all bench test

all: hescape.o

bench: benchmark/benchmark
	benchmark/benchmark

test: test/html_escape
	test/html_escape

benchmark/benchmark: $(BENCHOBJS)
	$(LD) $(BENCHOBJS) -o $@

test/html_escape: $(TESTOBJS)
	$(LD) $(TESTOBJS) -o $@

hescape.o: hescape.c hescape.h
	$(CC) -c -o $@ $<

benchmark/benchmark.o: benchmark/benchmark.c hescape.h benchmark/houdini/houdini.h
	$(CC) -c -o $@ $<

benchmark/houdini/houdini.h:
	git submodule init && git submodule update

benchmark/houdini/buffer.o: benchmark/houdini/houdini.h
	cd benchmark/houdini && $(MAKE) objects

benchmark/houdini/houdini_html_e.o: benchmark/houdini/houdini.h
	cd benchmark/houdini && $(MAKE) objects

test/html_escape.o: test/html_escape.c test/test_helper.h hescape.h
	$(CC) -c -o $@ $<
