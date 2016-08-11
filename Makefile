LD=g++
BENCHOBJS=benchmark/benchmark.o hescape.o benchmark/houdini/buffer.o benchmark/houdini/houdini_html_e.o
TESTOBJS=test/html_escape.o hescape.o
CFLAGS ?= -O2 -msse4
.PHONY: all bench clean test run

all: hescape.o

bench: benchmark/benchmark
	benchmark/benchmark

clean:
	$(RM) $(BENCHOBJS) $(TESTOBJS)
	cd benchmark/houdini && $(MAKE) clean

run: test/run.c hescape.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o test/run.o $<
	$(LD) test/run.o hescape.o -o test/run
	test/run

test: test/html_escape
	test/html_escape

benchmark/benchmark: $(BENCHOBJS)
	$(LD) $(BENCHOBJS) -o $@

test/html_escape: $(TESTOBJS)
	$(LD) $(TESTOBJS) -o $@

hescape.o: hescape.c hescape.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

benchmark/benchmark.o: benchmark/benchmark.c hescape.h benchmark/houdini/houdini.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

benchmark/houdini/houdini.h:
	git submodule init && git submodule update

benchmark/houdini/buffer.o: benchmark/houdini/houdini.h
	cd benchmark/houdini && CFLAGS="$(CFLAGS)" $(MAKE) objects

benchmark/houdini/houdini_html_e.o: benchmark/houdini/houdini.h
	cd benchmark/houdini && CFLAGS="$(CFLAGS)" $(MAKE) objects

test/html_escape.o: test/html_escape.c test/test_helper.h hescape.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<
