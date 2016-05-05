#include "cache.h"
#include "trace.h"
#include <limits.h>

/* Project 4 completed by Naropa Perez and Alex Hankin, all implementation is given below with comments
	All initial objectives created, along with all extra credit
	Just for the simulated files will appear when the tests are run in the traces folder alongside the initial trace file
	For the 3rd part of the extra credit, the custom created trace file is in the traces folder labeled "ec3.trace"
*/

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
	int lru = 0;
	FILE * myTrace;
	FILE * myTrace2;

  // hit and miss counts
  int totalHits = 0;
  int totalMisses = 0;

  char * filename;


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
    		i++;
    		lru=1;
			//printf("Unrecognized argument. Exiting.\n");
			//return -1;
    }
		//unrecognized input
		else{
			printf("Unrecognized argument. Exiting.\n");
			return -1;
		}
	}
	
  /* TODO: Probably should intitalize the cache */
	uint32_t sets = 1024 * size;
	int j;
	for(j = 0; j<sets; j++){
		if (((line*ways)*j)>=sets){
			sets = j;
			break;
		}
	}
	uint32_t rows = sets;
	uint32_t columns = (4 * ways) + 1;  //valid, dirty, tag, recently used repeat ... followed by first in first out counter
	uint32_t myCache$[rows][columns];
	//insantiate everything in cache

	for(int x=0;x<rows;x++)
		for(int y=0;y<columns;y++)
			myCache$[x][y]=0;
	//capacity array for checking misses... 
	uint32_t capacitySize = sets*ways;
	uint32_t capacityArray[capacitySize][3];//index tag recently used 
	uint32_t capacityIndex = 0;//first in first out counter
	for (int x = 0; x < capacitySize; x++)
	{
		capacityArray[x][0] = -1;
		capacityArray[x][1] = -1;
		capacityArray[x][2] = -1;
	}

	//create an array of all used tags
	
	uint32_t lines = 0;
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
  missType = (char *) malloc(12 * sizeof(char));
  char * accessType;
  accessType = (char *) malloc( sizeof(char));
  char * AddTemp;
  AddTemp = (char *) malloc(9* sizeof(char));


  
  rewind(myTrace);
  char * Sim = ".simulated";
  char * Simulated = strcat(filename, Sim);
  //printf("%s\n", Simulated);
  myTrace2 = fopen(("%s", Simulated), "w");
  char * newLine=(char *) malloc(100 * sizeof(char));
  char * newLine2=(char *) malloc( 100 * sizeof(char));
  while(j == 0){

	  char * Tester;
	  Tester = (char *) malloc(50);
	  if((fgets(Tester, 50, myTrace)) != '\0'){			//This is what is new as of tonight mah man
														//There are debugging print statements for each part of the address
			hit = 0;											//Also I guess the rest of the program will be contained in this if statement lol
			 accessType[0] = Tester[0];					//AccessType is a 1character char that stores either "l" or "s"
			 //printf("accessType: %c\n", accessType[0]);

			 //char * AddTemp[9];						//AddTemp takes the substring of the hex part of each line (don't worry about this one)
			 memcpy(AddTemp, &Tester[4], 8);
			 AddTemp[8] = '\0';
			//printf("hex: %s\n", AddTemp);
			 
			 myAddress = strtol(AddTemp, NULL, 16);		//myAddress is AddTemp turned into a 32 bit number
			 //printf("myAdress: %d\n", myAddress);
			 
			 //offset = myAddress<<(32-offsetBits);		//offset is the offset portion of AddTemp
			 //offset = offset>>(32-offsetBits);
			 //printf("offset: %d\n", offset);
			 if(indexBits!=0){
			 	index = myAddress<<(tagBits);				//index is the index portion of Addtemp
			 	index = index>>(tagBits+offsetBits);
			}
			else
				index=0;
			 //printf("index: %d\n", index);
			 
			 tag = myAddress>>(32-tagBits);				//tag is the tag portion of Addtemp
			 //printf("tag: %d\n", tag);
			 
			 data = myAddress>>(offsetBits);			//data is the tag+index

			 //go to index for loop to compare the tags

			for(int x = 0; x<columns-1; x = x + 4){

				if(myCache$[index][x]==1){

					if(myCache$[index][x+2]==tag){

						hit = 1;
						totalHits = totalHits + 1; //ad to total hit
						if(accessType[0]=='s')
							myCache$[index][x+1] = 1; //if save make dirty...
						
						if(lru==1){
							myCache$[index][x+3]=myCache$[index][columns-1]+1;
							myCache$[index][columns-1] = myCache$[index][columns-1] + 1;
						}
						
						int inCapacity = 0;
						for(int z = 0; z<capacitySize;z++){//add to hit counter for our capacity array
			 				if(capacityArray[z][0]==index)
			 					if(capacityArray[z][1]==tag){
			 						inCapacity = 1;
			 						if(lru==1){
				 						capacityArray[z][2]=capacityIndex;
				 						capacityIndex=capacityIndex+1;
				 					}
				 					break;
			 					}
			 			}
			 			
			 			if (inCapacity==0){
			 				if(lru==0){
			 					//add new value to capacity array first in first out...
								//if capacity deletes add to used...
								if(capacityArray[capacityIndex][0]==-1){
										capacityArray[capacityIndex][0] = index;
										capacityArray[capacityIndex][1] = tag;
								}
								else{
										usedArray[usedIndex][0] = capacityArray[capacityIndex][0];
										usedArray[usedIndex][1] = capacityArray[capacityIndex][1];

										capacityArray[capacityIndex][0] = index;
										capacityArray[capacityIndex][1] = tag;

										//incrememnt used
										usedIndex = usedIndex + 1;
								}

								//properly increase capacity Array's index
								if(capacityIndex + 1 == capacitySize)
									capacityIndex = 0;
								else
									capacityIndex = capacityIndex + 1;
					 		}
			 				else{
			 					//add new value to capacity array least recently used...
								//if capacity deletes add to used...
								int lruCapacityRow = 0;
								int lowestUsedCapacity = INT_MAX;
								for(int z=0;z<capacitySize;z++){
									if(capacityArray[z][2]+1<lowestUsedCapacity){
										lowestUsedCapacity = capacityArray[z][2]+1;
										lruCapacityRow=z;
									}
								}
								if(capacityArray[lruCapacityRow][0]==-1){
										capacityArray[lruCapacityRow][0] = index;
										capacityArray[lruCapacityRow][1] = tag;
										capacityArray[lruCapacityRow][2] = capacityIndex;
								}
								else{
										usedArray[usedIndex][0] = capacityArray[lruCapacityRow][0];
										usedArray[usedIndex][1] = capacityArray[lruCapacityRow][1];

										capacityArray[lruCapacityRow][0] = index;
										capacityArray[lruCapacityRow][1] = tag;
										capacityArray[lruCapacityRow][2] = capacityIndex;

										//incrememnt used
										usedIndex = usedIndex + 1;
								}
								capacityIndex=capacityIndex+1;
			 				}
			 			}
						break;
					}
				}
			 }

			 //if not hit
			 if (hit==0){
			 	totalMisses = totalMisses + 1; //add to misses
			 	int caseA = 0;
			 	int caseB = 0;

			 	//Check if in capacity if its there then conflict else...
			 	for(int x = 0; x<capacitySize;x++){
			 		if(capacityArray[x][0]==index)
			 			if(capacityArray[x][1]==tag){
			 				missType = " conflict";
			 				caseA = 1;
			 				//if(lru==1){//hit in capacity array update accordingly
			 				//	capacityArray[x][2]=capacityIndex;
			 				//	capacityIndex=capacityIndex+1;
			 				//}
			 				break;
			 			}
			 	}
			 	//check if in used then its capacity else...
			 	if(caseA==0){
			 		for(int x = 0; x<usedIndex; x++){
			 			if(usedArray[x][0]==index)
			 				if(usedArray[x][1]==tag){
			 					missType = " capacity";
			 					caseB = 1;
			 				}
			 		}
			 		//compulsory
			 		if(caseB==0){
			 			missType = " compulsory";
			 		}
			 	}

			 	if(lru==0){//IF FIRST IN FIRST OUT
				 	//insert into cache first in first out

				 	//if not valid make valid and insert
					if(myCache$[index][myCache$[index][columns-1]] == 0 ){
						myCache$[index][myCache$[index][columns-1]] = 1;
						myCache$[index][myCache$[index][columns-1] + 2] = tag;
					}
					else{
						//else just insert....after check dirty
						if(myCache$[index][myCache$[index][columns-1] + 1]==1)
							write_xactions = write_xactions + 1;
						myCache$[index][myCache$[index][columns-1] + 2] = tag;
					}

					//update dirty
					if(accessType[0]=='s')
								myCache$[index][myCache$[index][columns-1] + 1] = 1; //if save make dirty... maybe else make it to 0?
							else
								myCache$[index][myCache$[index][columns-1] + 1] = 0;
					
					if(caseA==0){
						//add new value to capacity array first in first out...
						//if capacity deletes add to used...
						if(capacityArray[capacityIndex][0]==-1){
								capacityArray[capacityIndex][0] = index;
								capacityArray[capacityIndex][1] = tag;
						}
						else{
								usedArray[usedIndex][0] = capacityArray[capacityIndex][0];
								usedArray[usedIndex][1] = capacityArray[capacityIndex][1];

								capacityArray[capacityIndex][0] = index;
								capacityArray[capacityIndex][1] = tag;

								//incrememnt used
								usedIndex = usedIndex + 1;
						}

						//properly increase capacity Array's index
						if(capacityIndex + 1 == capacitySize)
							capacityIndex = 0;
						else
							capacityIndex = capacityIndex + 1;
					}

					//finally incremenet the index first in first out temp variable by 4 and reset to 0 if it has reached the limit!
					if(myCache$[index][columns-1] + 4 < columns-1 )
						myCache$[index][columns-1] = myCache$[index][columns-1] + 4;
					else
						myCache$[index][columns-1] = 0;
				}
				else{// LEAST RECENTLY USED
					//insert into cachde least recently used
					int lruColumn = 0;
					int lowestUsed = INT_MAX;
					for(int x = 3; x < columns-1 ; x = x + 4)
						if(myCache$[index][x] < lowestUsed ){
							lowestUsed = myCache$[index][x];
							lruColumn = x-3;
						}

					//if not valid make valid and insert
					if(myCache$[index][lruColumn] == 0 ){
						myCache$[index][lruColumn] = 1;
						myCache$[index][lruColumn + 2] = tag;
					}
					else{
						//else just insert....after check dirty
						if(myCache$[index][lruColumn + 1]==1)
							write_xactions = write_xactions + 1;
						myCache$[index][lruColumn + 2] = tag;
					}

					//reset lruColumn
					myCache$[index][lruColumn + 3] = myCache$[index][columns-1]+1;

					//update dirty
					if(accessType[0]=='s')
						myCache$[index][lruColumn + 1] = 1; //if save make dirty... maybe else make it to 0?
					else
						myCache$[index][lruColumn + 1] = 0;
					
					if(caseA==0){
						
						//add new value to capacity array least recently used...
						//if capacity deletes add to used...
						int lruCapacityRow = 0;
						int lowestUsedCapacity = INT_MAX;
						for(int x=0;x<capacitySize;x++){
							if(capacityArray[x][2]+1<lowestUsedCapacity){
								lowestUsedCapacity = capacityArray[x][2]+1;
								lruCapacityRow=x;
							}
						}
						if(capacityArray[lruCapacityRow][0]==-1){
								capacityArray[lruCapacityRow][0] = index;
								capacityArray[lruCapacityRow][1] = tag;
								capacityArray[lruCapacityRow][2] = capacityIndex;
						}
						else{
								usedArray[usedIndex][0] = capacityArray[lruCapacityRow][0];
								usedArray[usedIndex][1] = capacityArray[lruCapacityRow][1];

								capacityArray[lruCapacityRow][0] = index;
								capacityArray[lruCapacityRow][1] = tag;
								capacityArray[lruCapacityRow][2] = capacityIndex;

								//incrememnt used
								usedIndex = usedIndex + 1;
						}
					}
					else{
						//if conflict update capacity Array acordingly
						for(int x = 0; x<capacitySize;x++){
			 					if(capacityArray[x][0]==index)
			 						if(capacityArray[x][1]==tag){
			 							capacityArray[x][2]=capacityIndex;
			 							break;
			 						}
			 			}
					}
					//incremment highest recent number for both capacity and last column in Index in cache
					capacityIndex  = capacityIndex + 1;
					myCache$[index][columns-1]=myCache$[index][columns-1] + 1;
				}
			}

			 int i = 0;
			//char TesterSave[50];
			//memset(&TesterSave[0], 0, sizeof(TesterSave));
			char * TesterSave;
  			TesterSave = (char *) malloc(50* sizeof(char));
			strncpy(TesterSave, Tester, 12);

			 if (hit == 1){
				missType = " hit";
				newLine = strcat(TesterSave, missType);
				newLine2 = strcat(newLine, "\n");
				//printf("%s", newLine2);
				fprintf(myTrace2, ("%s", newLine2));
			 }
			 else{
				//missType = " miss";
				newLine = strcat(TesterSave, missType);
				newLine2 = strcat(newLine, "\n");
				//printf("%s", newLine2);
				fprintf(myTrace2, ("%s", newLine2));
			 }

			 //j += 1;									//This purposefully stops it at one line just for debugging
		}
		else{
			 j += 1;									//Ends the while loop if line is blank
		}
  }
	//for(int x=0;x<capacitySize;x++){
	//			printf("C1: %d C2: %d c3: %d\n",capacityArray[x][0],capacityArray[x][1],capacityArray[x][2]);
	//		}
  //attemt 3
  //go to index compare tags, if its there hit else...
  //Check if in capacity if its there then conflict else...
  //check if in used then its capacity else
  //compulsory

  //Whenever you put something into the chache, aka not hit, add to capacity first in first out
  //if capacity deletes something add to old


  printf("Miss Rate: %8lf%%\n", ((double) totalMisses) / ((double) totalMisses + (double) totalHits) * 100.0);
  read_xactions = totalMisses;
  printf("Read Transactions: %d\n", read_xactions);
  printf("Write Transactions: %d\n", write_xactions);

  /* TODO: Now we output the file */

  /* TODO: Cleanup */
}
