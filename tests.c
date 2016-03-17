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
	int Tester = 4294967295;
	Tester = Tester<<29;
	Tester = Tester>>29;
	printf("%d\n", Tester);
    return 0;
}
