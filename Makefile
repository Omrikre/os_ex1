# Makefile for ex1_q1 winter 2023B

CFLAGS   = -Wall -std=c99
LDFLAGS  = -lm
CC       = gcc
ECHO     = @echo "going to build target $@ (dependent on $^)"

# the following is used to assign different values according to OS
# uname is a linux command (try it).
# commented out for now.
# OS := $(shell uname)
# ifneq (,$(findstring LINUX,$(OS)))
#   VAR =
# else
#   VAR =
# endif

PROG = ex1_q1

all: $(PROG) test

ex1_q1.o: $(PROG).c #  $(PROG).h 
	$(ECHO)
	$(CC) $(CFLAGS) -c $<

$(PROG): $(PROG).o
	$(ECHO)
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -vf *.o ${PROG} all_std.txt *.temp

test: $(PROG)
	@echo going to run test...
	./$(PROG) test.txt test2.txt test4.txt
