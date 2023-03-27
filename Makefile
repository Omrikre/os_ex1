####################################
# BASIC Makefile for ex1_q1 winter 2023B
#  You can change this if needed 
#  make sure you leave here the first line of the test section (line 28).
####################################


CFLAGS   = -Wall -std=c99
LDFLAGS  = -lm
CC       = gcc
ECHO     = @echo "going to build target $@ (dependent on $^)"

PROG = ex1_q1

all: $(PROG) test

ex1_q1.o: $(PROG).c #  $(PROG).h 
	$(ECHO)
	$(CC) $(CFLAGS) -c $<

$(PROG): $(PROG).o
	$(ECHO)
	gcc $^ $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -vf *.o ${PROG} all_std.log *.temp err.log

test: $(PROG)
	@echo going to run test...
	./$(PROG) gr_1.txt gr_2.txt   2> err.log 
