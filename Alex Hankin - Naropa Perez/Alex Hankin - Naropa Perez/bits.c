/*

CSC 252 Spring 2016
Project 2: Bit Twiddling
Members: Alex Hankin and Naropa Perez
Submission Date: 3/21/2016


 * TA: Wolf Honore (whonore@u.rochester.edu)
 *
 * Instructions:
 * Fill in the following functions and turn this file in.
 *
 * Restrictions:
 * You MAY NOT change any of the function defintions.
 * You MAY NOT define any global variables, macros, or other functions.
 * You MAY NOT use any operations not listed in the function's description.
 * You MAY NOT use any control statements (if, loops, switch, function calls).
 * You MAY NOT use any constants outside of an unsigned byte (0 - 255 (0xFF) inclusive).
 * You MAY NOT cast any variables.
 * You MAY NOT use data types other than ints.
 *
 * You MAY define local variables.
 * You MAY use the assignment versions of allowed operators (+=, |=, etc).
 * You MAY use more than one operator per line.
 *
 * Your results will be tested on a machine that:
 * Uses 2s complement, 32-bit representations of integers.
 * Performs right shifts arithmetically.
 * Has unpredictable behavior when shifting an integer by more than the word size.
 *
 * If any of the instructions or function descriptions are unclear please contact the TA (see
 * above).
 */

/*
 * evenBits: Return the word with all even-numbered bits set to 1.
 *   Legal ops: ! ~ & ^ | + << >>
 */
int evenBits(void) {
    int temp = 0x55;				//We make a local variable be the highest number with evenbits thats less than 255
	int word = temp + (temp<<8) + (temp<<16) + (temp<<24);	//Use shifting to create larger evenbit numbers and add them together
	return word;
}

/*
 * bitAnd: Compute the bitwise-and of x and y.
 *   Example: bitAnd(3, 5) = 1
 *   Legal ops: ~ |
 */
int bitAnd(int x, int y) {
	int word = ~((~(x|x))|(~(y|y)));	//This is merely an 'and' logic gate created using a combination of the given operations.
    return word;
}

/*
 * swapBytes: Swap the 0th (rightmost) and 2nd bytes of x.
 *   Example: swapBytes(0x33221100) = 0x33001122
 *   Legal ops: ! ~ & ^ | + << >>
 */
int swapBytes(int x) {
	int constant = 0xFF;					//Create a constant for 255 (8 bits of 1's)
	int temp0 = (constant & x);				//We set temp0 equal to the first 8 bits (0th byte)
	temp0 = (temp0<<16);					//We shift it 16 spaces to where it will eventually be moved to.
	int temp2 = constant<<16;				//Create temp2 and set it to 255 displaced to where the 2nd byte is
	temp2 = (temp2 & x);					//Set temp2 equal to the 2nd Byte of x
	temp2 = (temp2>>16);					//Move it to the 0th byte (to where it will eventually be)
	int final = (constant<<24) + (constant<<8);	//Create a final which will be the leftovers (1st and 3rd bytes)
	final = (final & x);					//Set final to the specified bytes
	final = (final | temp0 | temp2);		//Merge all thre components using or
    return final;
}

/*
 * rotateLeft: Rotate x to the left by n bits. (For 0 <= n <= 31)
 *   Example: rotateLeft(0x76543210, 8) = 0x54321076
 *   Legal ops: ~ & ^ | + << >>
 */
int rotateLeft(int x, int n) {
	int nminus = (~n) + 1;								//creates a constant set to -n
	int shifter = 32 + nminus;							//sets shifter equal to 32-n
	int theRotated = (x>>shifter);						//moves the integer by (32-x) these are the bits that will be rotated to the end
	int theRotatedShifted = (theRotated<<shifter);		//shift them back to their original location temporaily
	int UnRotated = (theRotatedShifted ^ x);			//Use XOR with the rotated bits in order to find the bits that won't be looped back to the beginning
	int UnRotatedShifted = (UnRotated<<n);				//Shift these bits to the left end of the number
	int final =(theRotated|UnRotatedShifted);			//Combine the shifted non rotated values with our original rotated ones
	return final;
}

/*
 * addOver: Determine if x + y overflows.
 *   Example: addOver(0x7FFFFFFE, 1) = 0
 *            addOver(0x7FFFFFFE, 2) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 */
int addOver(int x, int y) {
	//Base Case 1: Positive Negative, can't overflow
	//Base Case 2: Positive Postive, answer must also be positive
	//Base Case 2: Negative negative, answer must also be negative

	int answer=x+y; // compute the sum

	int xSign = x>>31; // if postive 0, if negative -1 for x

	int ySign = y>>31; // if postive 0, if negative -1 for y

	int answerSign = answer>>31; // if postive 0, if negative -1 for the answer

    return !((xSign^ySign)|(!(xSign^answerSign)&!(ySign^answerSign))); //not (exlusive or on the signs or (not exclusive or between x and the answer, and not exc. or between y and the answer)
}

/*
 * boundedMult: Compute 2x, but return the largest positive or negative signed integer if overflow
 *              would occur.
 *   Example: boundedMult(0x00000001) = 0x00000002
 *            boundedMult(0x50000000) = 0x7FFFFFFF (bound to largest positive int)
 *            boundedMult(0x60000000) = 0x80000000 (bound to largest negative int)
 *   Legal ops: ! ~ & ^ | + << >>
 */
int boundedMult(int x, int n) {
	int result = x + x; // calculate the result

	int sign = x >> 31; // get the sign of x (to know what to do if we over flow) 0 if postive, -1 if negative

	int maxNeg = 1 << 31; // compute max neg num
	int maxPos = maxNeg + ~0; // compute max pos num

	int lastDigitp = x >> 30; //I want the last digit right after the bit that is designate for the sign
	lastDigitp = lastDigitp << 31; //shift back right 31 to get rid of bight sign
	lastDigitp = lastDigitp >> 31; // back over to isolate the digit i wanted, 0 if 0, -1 if 1
	//if this last digit is a 1 then there will be an overflow, other wise there won't be

	int lastDigitn = ~x >> 30; //same thing but first get the compliment. This is for the negative case
	lastDigitn = lastDigitn << 31; 
	lastDigitn = lastDigitn >> 31;

	//NOTE: -1 & x = x
	//NOTE: 0 & x = 0
	// I make use of the above two pieces of information and join them through adition to make a makeshift if statement !

    //        if negative...  (if 0....   return res      if 1.... return maxNeg)        if postive...  ( if 0... return res         if 1... return maxPos)
    return ( (~!!sign+1) & ( ( ~lastDigitn & result ) + ( lastDigitn & maxNeg ) ) ) + ( (~!sign+1) & ( ( ~lastDigitp & result ) + ( lastDigitp & maxPos ) ) ) ;
}

/*
 * absHalf: Compute |x / 2| rounding towards 0.
 *   Example: absHalf(3)  = 1
 *            absHalf(-6) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 */
int absHalf(int x) {
	int sign = x>>31; // get the sign of x
	int negX = ~x+1; //get neg x
	//                if neg...  set to negX    if pos... set to X
	int absValueX = ( (~!!sign+1) &  negX ) + ( (~!sign+1) & x );
    //return absValueX>>1;
    // shift to right to divide by 2
    return absValueX>>1;
}

/*
 * lessThan: Check if x < y.
 *   Example: lessThan(4, 5) = 1
 *            lessThan(5, 4) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 */
int lessThan(int x, int y) {
	int compX = ~x+1 ; // get x's 2's compliment 
	int sum = y + compX; // add x's 2 compliment to y
	int signOfSum = sum >> 31; //get the sign of the result (if the result is negative then return 0)
    return !signOfSum & 1 ; // note ! signOfSum would make a 0 to a 1 and a -1 to a 0. When you and 1 with 0 u get 0, and when you and with 1 you get 1.
}

/*
 * multFiveEighths: Compute 5/8ths of x rounding towards 0.
 *   Example: multFiveEighths(8)  = 5
 *            multFiveEighths(-9) = -5
 *   Legal ops: ! ~ & ^ | + << >>
 */
int multFiveEighths(int x) {
	int sign = x>>31; // get the sign of x
	int negX = ~x+1; // get neg x

	//if sign is -1 then !! is 1 (the first ! sets it to 0) and the complement + 1 would be -1.
	//if sign is 0 then !! keeps it at 0 and the complement +1 would be 0.
	//-1 & x = x and 0 & x = 0
	//in the first case if negative it yields -1, meaning we should return -x (if positive it returns 0)
	//in the second case if postive it yields -1, meaning we should return x (if negative it returns 0)

	int absValueX = ( (~!!sign+1) &  negX ) + ( (~!sign+1) & x );
	absValueX = absValueX>>3; //devide by 8
	absValueX = absValueX+absValueX+absValueX+absValueX+absValueX; //mult by 5
	//no over flow because 8>5
	//same logic to return the answer
    return ( (~!!sign+1) &  (-absValueX) ) + ( (~!sign+1) & absValueX );
}

/*
 * isPwr2: Check if x is a power of 2. Assume x > 0.
 *   Example: isPwr2(2) = 1
 *            isPwr2(3) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 */
int isPwr2(unsigned int x) {
	int minusone = (~1) + 1;				//Create a constant for -1
	int PowerTest = x + minusone;			//subtract our int by 1, this will make it all ones if it is a power of two
	PowerTest = (x & PowerTest);			//use and between x and (x-1) if and only if it is a power of two, it will return all zeroes.
	int Indicator = (!PowerTest);			//Use not to make it so the specific zero will become 1 when returned, and 0 if anything else was produced.
    return Indicator;
}
