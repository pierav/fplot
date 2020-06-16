LEX = flex --warn
YACC = bison -d -v --graph
# --graph --
CC = gcc
LD = gcc
CFLAGS = -Wall -Wextra  -g -Iinclude/ -Isrc/ -fno-stack-protector
LDFLAGS = -L./lib -I./include -lm

EXEC = bin/main
SRC=$(shell find src/ -type f -name '*.c') src/grammar.l.c src/grammar.y.c
OBJ=$(patsubst src/%.c,obj/%.o,$(SRC))
DIRS=$(patsubst src/%,obj/%,$(shell find src/ -type d)) bin/tests obj/tests

TESTS=$(wildcard tests/*.c)
TEST_BINS=$(patsubst tests/%.c,bin/tests/%,$(TESTS))

DISPLAY_IMAGE=$(shell which viewnior 2>/dev/null || which eog 2>/dev/null || which feh 2>/dev/null || which display 2>/dev/null ||  which tycat 2> /dev/null)

all: $(DIRS) $(SRC) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(EXTRA_LFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $^ -o $@

src/%.y.c: src/%.y
	$(YACC) --defines=$(patsubst %.c,%.h,$@) -o $@ $^

src/%.l.c: src/%.l
	$(LEX) -o $@ $^


tests: $(DIRS) $(TEST_BINS)
	@for t in $(TEST_BINS) ; do \
		if ! $$t ; then \
			echo "Failure at test $$t" ; \
		fi ; \
	done

bin/tests/%: obj/tests/%.o $(filter-out obj/main.o,$(OBJ))
	$(CC) -o $@ $^ $(LDFLAGS) $(EXTRA_LFLAGS)

obj/tests/%.o: tests/%.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $^ -o $@


$(DIRS): %:
	mkdir -p $@

display_ast: all
	bin/main < fpsrc/condition.fp
	dot -Tpng out.dot > /tmp/ast.png
	$(DISPLAY_IMAGE)  /tmp/ast.png


graphs:
	mkdir -p graphs

profile: EXTRA_CFLAGS=-pg
profile: EXTRA_LFLAGS=-pg
profile: scripts/gprof2dot.py graphs clean all
	bin/main
	echo -e "\n\n"
	gprof bin/main | scripts/gprof2dot.py | dot -Tpng -o graphs/profile-graph.png
	$(DISPLAY_IMAGE) graphs/profile-graph.png

clean:
	rm -rf obj/
	rm -rf bin/
	rm -f src/*.l.c
	rm -f src/*.y.*

.PHONY: all tests clean
