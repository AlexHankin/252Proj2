/*
 * Assignment: Bit Twiddling
 * Class: CSC 252 Spring 2016
 *
 * TA: Wolf Honore (whonore@u.rochester.edu)
 *
 * You may use this program to write tests for your solutions in bits.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include "bits.h"



int main(int argc, char **argv) {
	int Test1 = evenBits();
	printf("%d\n", Test1);
	int Test2 = bitAnd(54, 87);
	printf("%d\n", Test2);
	

	int Test3 = swapBytes(0x33221100);
	printf("%d\n", Test3);
	
	int Test4 = rotateLeft(0x76543210, 8);
	printf("%d\n", Test4);
	int Test10 = isPwr2(2);
	int Test11 = isPwr2(3);
	int Test12 = isPwr2(4);
	int Test13 = isPwr2(8);
	int Test14 = isPwr2(16);
	int Test15 = isPwr2(420);
	printf("%d\n", Test10);
	printf("%d\n", Test11);
	printf("%d\n", Test12);
	printf("%d\n", Test13);
	printf("%d\n", Test14);
	printf("%d\n", Test15);

    return 0;
}
