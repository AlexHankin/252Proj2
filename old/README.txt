CSC 252 Spring 2016
Project 2: Bit Twiddling
Members: Alex Hankin and Naropa Perez
Submission Date: 3/21/2016

General Comments:


bits.c:
Contains all methods that were needed to implemented by the student.
All relevant methods are listed below:

	evenBits (Implemented by Alex Hankin):
	-Return the 32-bit number with 1's at every even bit.

	bitAnd (Implemented by Alex Hankin)
	-Returns the logical numbers (x and y) when inputed.

	swapBytes (Implemented by Alex Hankin)
	-Swaps the 0th and second most bytes of given number x.

	rotateLeft (Implemented by Alex Hankin)
	-Shifts the given number over by a given value.

	addOver (Implemented by Naropa Perez)
	-returns 1 or 0 if two numbers added overflow.

	boundedMult (Implemented by Naropa Perez)
	-Computes 2x the given number, but returns the largest integer of the same sign if it overflows.

	absHalf (Implemented by Naropa Perez)
	-Computes the rounded down variation of the absolute value of a given number, then divided by 2.

	lessThan (Implemented by Naropa Perez)
	-Returns 0 or 1 depending on whether a given number is less than another given number.

	multFiveEighths (Implemented by Naropa Perez)
	-Returns 5/8 of a given number rounded down to the nearest whole number.

	isPwr2 (Implemented by Alex Hankin)
	-Returns 1 if a given number is a power of 2, returns 0 otherwise.

bits.h: Corresonding h-file for the bits file.

tests.c: tests file used to create tests to make sure the methods in bits.c are working
properly

Makefile: Included makefile used by students in order to test the program.

README.txt (Written by Alex Hankin)



Comments for each method written by the respective student who implmented them.