#
# Assignment: Bit Twiddling
# Class: CSC 252 Spring 2016
#
# TA: Wolf Honore (whonore@u.rochester.edu)
#
# Makefile for bits.c and tests.c.
#

CC = gcc
CFLAGS = -O -Wall -m32
LIBS =

all: tests

tests: tests.c bits.h
	$(CC) $(CFLAGS) $(LIBS) -o tests tests.c bits.c

clean:
	rm -f *.o tests *~
