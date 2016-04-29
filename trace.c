#include "trace.h"

// Put anything for reading trace files / writing output files here.

/* TODO: read trace file function */
FILE * readTrace(char* address){
	FILE * myTrace;
	myTrace = fopen(("%s", address), "r");
	return myTrace;
}


/* TODO: write output file function */