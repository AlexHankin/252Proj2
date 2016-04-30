#include "cache.h"
#include "trace.h"

int write_xactions = 0;
int read_xactions = 0;

/*
  Print help message to user
*/
void printHelp(const char * prog) {
	printf("%s [-h] | [-s <size>] [-w <ways>] [-l <line>] [-t <trace>]\n", prog);
	printf("options:\n");
	printf("-h: print out help text\n");
	printf("-s <cache size>: set the total size of the cache in KB\n");
	printf("-w <ways>: set the number of ways in each set\n");
	printf("-l <line size>: set the size of each cache line in bytes\n");
	printf("-t <trace>: use <trace> as the input file for memory traces\n");
  /* EXTRA CREDIT
  printf("-lru: use LRU replacement policy instead of FIFO\n");
  */
}
/*
	Main function. Feed to options to set the cache
	
	Options:
	-h : print out help message
	-s : set L1 cache Size (B)
	-w : set L1 cache ways
	-l : set L1 cache line size
*/
int main(int argc, char* argv[])
{
	int i;
	uint32_t size = 32; //total size of L1$ (KB)
	uint32_t ways = 1; //# of ways in L1. Default to direct-mapped
	uint32_t line = 32; //line size (B)
	FILE * myTrace;
	FILE * myTrace2;

  // hit and miss counts
  int totalHits = 0;
  int totalMisses = 0;

  char * filename;
  char accessType[1];

	//strings to compare
	const char helpString[] = "-h";
	const char sizeString[] = "-s";
	const char waysString[] = "-w";
	const char lineString[] = "-l";
	const char traceString[] = "-t";
  const char lruString[] = "-lru";
	
  if (argc == 1) {
    // No arguments passed, show help
    printHelp(argv[0]);
    return 1;
  }
  
	//parse command line
	for(i = 1; i < argc; i++)
	{
		//check for help
		if(!strcmp(helpString, argv[i]))
		{
			//print out help text and terminate
		printHelp(argv[0]);
			return 1; //return 1 for help termination
		}
		//check for size
		else if(!strcmp(sizeString, argv[i]))
		{
			//take next string and convert to int
			i++; //increment i so that it skips data string in the next loop iteration
			//check next string's first char. If not digit, fail
			if(isdigit(argv[i][0]))
			{
				size = atoi(argv[i]);
			}
			else
			{
				printf("Incorrect formatting of size value\n");
				return -1; //input failure
			}
		}
		//check for ways
		else if(!strcmp(waysString, argv[i]))
		{
			//take next string and convert to int
			i++; //increment i so that it skips data string in the next loop iteration
			//check next string's first char. If not digit, fail
			if(isdigit(argv[i][0]))
			{
				ways = atoi(argv[i]);
			}
			else
			{
				printf("Incorrect formatting of ways value\n");
				return -1; //input failure
			}
		}
		//check for line size
		else if(!strcmp(lineString, argv[i]))
		{
			//take next string and convert to int
			i++; //increment i so that it skips data string in the next loop iteration
			//check next string's first char. If not digit, fail
			if(isdigit(argv[i][0]))
			{
				line = atoi(argv[i]);
			}
			else
			{
				printf("Incorrect formatting of line size value\n");
				return -1; //input failure
			}
		}
    else if (!strcmp(traceString, argv[i])) {
      filename = argv[++i];
	  myTrace = readTrace(filename);
    }
    else if (!strcmp(lruString, argv[i])) {
      // Extra Credit: Implement Me!
			printf("Unrecognized argument. Exiting.\n");
			return -1;
    }
		//unrecognized input
		else{
			printf("Unrecognized argument. Exiting.\n");
			return -1;
		}
	}
	
  /* TODO: Probably should intitalize the cache */
	uint32_t sets = 1000 * size;
	int j;
	for(j = 0; j<sets; j++){
		if (((line*ways)*j)>=sets){
			sets = j;
			break;
		}
	}
	int rows = sets;
	int columns = (2 * ways) + 2;  //valid, dirty, tag repeat ... followed by first in first out counter
	uint32_t myCache$[rows][columns]; 

	//insantiate everything in cache

	for(int x=0;x<rows;x++)
		for(int y=0;y<columns;y++)
			myCache$[x][y]=0;

	//capacity array for checking misses... 
	uint32_t capacitySize = sets*ways;
	uint32_t capacityArray[capacitySize][2]; 
	uint32_t capacityIndex = 0;

	for (int x = 0; x < capacitySize; x++)
	{
		capacityArray[x][0] = -1;
		capacityArray[x][1] = -1;
	}

	//create an array of all used tags
	
	int lines = 0;
	while(!feof(myTrace))
	{
		char newLine = fgetc(myTrace);
		if(newLine == '\n')
		{
			lines += 1;
		}
	}
	lines = lines - capacitySize;
	uint32_t usedArray[lines][2];
	uint32_t usedIndex = 0;
	//int n = sizeof(usedArray)/sizeof(usedArray[0]);
	//printf("Size of Memory: %d\n", n);
	

	int indexBits = 0;
	int counter = 1;
	while(sets>counter){
		counter = counter * 2;
		indexBits += 1;
	}
	
	int offsetBits = 0;
	counter = 1;
	while(line>counter){
		counter = counter * 2;
		offsetBits += 1;
	}
	
	int tagBits = 32 - indexBits - offsetBits;
  printf("Ways: %u; Sets: %u; Line Size: %uB\n", ways, sets, line);
  printf("Tag: %d bits; Index: %d bits; Offset: %d bits\n", tagBits, indexBits, offsetBits);

	/* TODO: Now we read the trace file line by line */
  
  /* TODO: Now we simulate the cache */  

  /* Print results */
  j = 0;
  uint32_t myAddress;
  uint32_t index;
  uint32_t data;
  //uint32_t offset;
  uint32_t tag;
  int fifoCount = 0;
  int UsedCount = 0;
  int hit = 0;
  char * missType;
  
  rewind(myTrace);
  char * Sim = ".simulated";
  char * Simulated = strcat(filename, Sim);
  printf("%s\n", Simulated);
  myTrace2 = fopen(("%s", Simulated), "w");
  char * newLine;
  char * newLine2;
  
  while(j == 0){
	  char Tester[50];
	  if((fgets(Tester, 50, myTrace)) != '\0'){			//This is what is new as of tonight mah man
														//There are debugging print statements for each part of the address
			hit = 0;											//Also I guess the rest of the program will be contained in this if statement lol
			 accessType[0] = Tester[0];					//AccessType is a 1character char that stores either "l" or "s"
			 //printf("accessType: %c\n", accessType[0]);

			 char AddTemp[9];							//AddTemp takes the substring of the hex part of each line (don't worry about this one)
			 memcpy(AddTemp, &Tester[4], 8);
			 AddTemp[8] = '\0';
			// printf("hex: %s\n", AddTemp);
			 
			 myAddress = strtol(AddTemp, NULL, 16);		//myAddress is AddTemp turned into a 32 bit number
			 //printf("myAdress: %d\n", myAddress);
			 
			 //offset = myAddress<<(32-offsetBits);		//offset is the offset portion of AddTemp
			 //offset = offset>>(32-offsetBits);
			 //printf("offset: %d\n", offset);
			 
			 index = myAddress<<(tagBits);				//index is the index portion of Addtemp
			 index = index>>(tagBits+offsetBits);
			 //printf("index: %d\n", index);
			 
			 tag = myAddress>>(32-tagBits);				//tag is the tag portion of Addtemp
			 //printf("tag: %d\n", tag);
			 
			 data = myAddress>>(offsetBits);			//data is the tag+index
			 
			 //simulate cache i guess
			 //need to use a dynamic old array................ check :)

			 //go to index for loop to compare the tags
			for(int x = 2; x<columns; x++){
				if(myCache$[index][x]==tag){
					if(tag == 0){
						if(myCache$[index][1] == 1){
							hit = 1;
							totalHits = totalHits + 1;
							break;
						}
						else{
							hit = 0;
						}
					}
					else{
						hit = 1;
						totalHits = totalHits + 1;
						break;
					}
				}
						//HIT!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
						x++;
			 }
			 
			 
			 

			 //if not in index then insert by first in first out... 
			 //then add to capacity array first in first out... when removing add to usedArray

			/*if(myCache$[index][myCache$[index][columns-1]] == 0 ){
				myCache$[index][myCache$[index][columns-1]] = 1; 
				myCache$[index][myCache$[index][columns-1] + 2] = tag;
				//COMPULSURY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			else{
				if(capacityArray[capacityIndex] == -1)
					capacityArray[capacityIndex] = myCache$[index][myCache$[index][columns-1] + 2];
				else{
					usedArray[usedIndex]=capacityArray[capacityIndex];
					usedIndex = usedIndex + 1;
				}

				if(capacityIndex + 1 == capacitySize)
					capacityIndex = 0;
				else
					capacityIndex = capacityIndex + 1;
			}

			//check if tag is in capacity array, if so, conflict
			for(int x = 0; x < capacitySize ; x++){
				
			}*/
			 
			 //if not check if in used, if not compolsury and add to used
			 //if so then conflict

			 if (hit == 0){
				 
				totalMisses = totalMisses + 1;
				myCache$[index][1] = 1;
				
				usedArray[UsedCount][0] = myCache$[index][2 + (fifoCount*2)];
				usedArray[UsedCount][1] = myCache$[index][3 + (fifoCount*2)];
				myCache$[index][2 + (fifoCount*2)] = tag;
				myCache$[index][3 + (fifoCount*2)] = data;
					
				UsedCount = UsedCount + 1;
				if(fifoCount = ways - 1){
					fifoCount = 0;
				}
				else{
					fifoCount = fifoCount + 1;
				}
				
			 }
			 int i = 0;
			char TesterSave[50];
			memset(&TesterSave[0], 0, sizeof(TesterSave));
			strncpy(TesterSave, Tester, 12);
			 if (hit == 1){
				missType = " hit";
				newLine = strcat(TesterSave, missType);
				newLine2 = strcat(newLine, "\r\n");
				fprintf(myTrace2, ("%s", newLine2));
			 }
			 else{
				missType = " miss";
				newLine = strcat(TesterSave, missType);
				newLine2 = strcat(newLine, "\r\n");
				fprintf(myTrace2, ("%s", newLine2));
			 }

			 //j += 1;									//This purposefully stops it at one line just for debugging
		}
		else{
			 j += 1;									//Ends the while loop if line is blank
		}
  }
  
  //to do list
  //old array that contains index and tag
  //capacity array that also contains last deleted things for capacity size
  //go to index and compare tags
  //if its there hit else...
  //Check if it is in cpacity, if so Conflict
  //then check if in used, then capacity
  //if not it is compulsory
  //add first in first out to index... if removing add to capacity array
  //add to capacity first in first out, if removing, add to used


  printf("Miss Rate: %8lf%%\n", ((double) totalMisses) / ((double) totalMisses + (double) totalHits) * 100.0);
  printf("Read Transactions: %d\n", read_xactions);
  printf("Write Transactions: %d\n", write_xactions);

  /* TODO: Now we output the file */

  /* TODO: Cleanup */
}
