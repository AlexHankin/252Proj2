
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

//Stats

uint32_t DynInstCount = 0;

void write_initialization_vector(uint32_t sp, uint32_t gp, uint32_t start) {
        printf("\n ----- BOOT Sequence ----- \n");
        printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", sp, gp, start);
        RegFile[28] = gp;
        RegFile[29] = sp;
        RegFile[31] = start;
        printRegFile();

    }


int main(int argc, char * argv[]) {
  
    int MaxInst = 0;
    int status = 0;
	int jumper = 0;
	int jumped = 0;
    uint32_t i; 
    uint32_t PC,newPC;
    uint32_t CurrentInstruction;

    if (argc < 2) {
      printf("Input argument missing \n");
      return -1;
    }
    sscanf (argv[2], "%d", &MaxInst);

    //Open file pointers & initialize Heap & Regsiters
    initHeap();
    initFDT();
    initRegFile(0);
    
    //Load required code portions into Emulator Memory
    status =  LoadOSMemory(argv[1]);
    if(status <0) { return status; }
    
    //set Global & Stack Pointers for the Emulator
    // & provide startAddress of Program in Memory to Processor
    write_initialization_vector(exec.GSP, exec.GP, exec.GPC_START);

    printf("\n ----- Execute Program ----- \n");
    printf("Max Instruction to run = %d \n",MaxInst);
    PC = exec.GPC_START;

    for(i=0; i<MaxInst ; i++) {
        DynInstCount++;
        CurrentInstruction = readWord(PC,true);  
        printRegFile();
    /********************************/
    //Add your implementation here
    /********************************/

	int OpCode = readByte(PC,true);
	OpCode = OpCode>>2;
	printf("OpCode: %d\n", OpCode);

	
	int RegSource;
	int RegTemp;
	int RegT;
	uint16_t immediate;
	uint32_t immtemp;
	int Func;
	int RegDes;
	int Shamt;
	int16_t offset;
	int32_t offsettemp;
	uint16_t returnaddress;
	
	RegSource = (CurrentInstruction<<6);
	RegSource = (RegSource>>27);
	RegSource = (RegSource & 31);
			
	RegT = (CurrentInstruction<<11);
	RegT = (RegT>>27);
	RegT = (RegT & 31);
				
	RegDes = (CurrentInstruction<<16);
	RegDes = (RegDes>>27);
	
	Shamt = (CurrentInstruction<<21);
	Shamt = (Shamt>>27);
	Shamt = (Shamt & 31);
	
	printf("RegSource: %d\n", RegSource);
	printf("RegT: %d\n", RegT);
	printf("RegDes: %d\n", RegDes);
	printf("Shamt: %d\n", Shamt);
	newPC = readByte(PC,true);
	
	Func = (CurrentInstruction & 63);
	printf("Func: %d\n", Func);
	
	offsettemp = readWord(PC + 2, true);
	offset = offsettemp>>16;
	printf("Offset: %d\n", offset);
	
	switch(OpCode) {
		case 0:
			switch(Func) {
				case 32: //add
					RegFile[RegDes] = RegFile[RegSource]+RegFile[RegT];
					PC = PC + 4;
					break;
				
				case 34: //sub
					RegFile[RegDes] = RegFile[RegSource]-RegFile[RegT];
					PC = PC + 4;
					break;
				
				case 26: //div
					//RegFile[32] = 0;
					//RegFile[33] = 0;
					RegFile[32] = RegFile[RegSource]%RegFile[RegT];
					RegFile[33] = (RegFile[RegSource]-RegFile[32])/RegFile[RegT];
					PC = PC + 4;
					break;
				
				case 24: //mult
					//RegFile[33] = 0;
					RegFile[33] = RegFile[RegSource]*RegFile[RegT];
					PC = PC + 4;
					break;
				
				case 16: //mfhi
					RegFile[RegDes] = RegFile[32];
					PC = PC + 4;
					break;
				
				case 17: //mthi
					RegFile[32] = RegFile[RegSource];
					PC = PC + 4;
					break;
				
				case 18: //mflo
					RegFile[RegDes] = RegFile[33];
					PC = PC + 4;
					break;
				
				case 19: //mtlo
					RegFile[33] = RegFile[RegSource];
					PC = PC + 4;
					break;
				
				case 38: //xor
					RegFile[RegDes] = (RegFile[RegSource] ^ RegFile[RegT]);
					PC = PC + 4;
					break;
				
				case 0: //sll
					switch(RegDes){
						case 0:
							break;
						
						RegFile[RegDes] = (RegFile[RegT]<<Shamt);
						PC = PC + 4;
						break;
					}
				case 2: //srl
					RegFile[RegDes] = (RegFile[RegT]>>Shamt);
					PC = PC + 4;
					break;
				
				case 8: //jr
					jumper = 1;
					returnaddress = RegSource;
					returnaddress = returnaddress<<11;
					returnaddress = returnaddress>>11;
					printf("Return Address: %d\n", returnaddress);
					RegTemp = RegFile[returnaddress];
					break;
					
				case 9: //jalr
					jumper = 1;
					returnaddress = RegDes;
					returnaddress = returnaddress<<11;
					returnaddress = returnaddress>>11;
					printf("Return Address: %d\n", returnaddress);
					RegTemp = RegFile[RegSource];
					RegFile[returnaddress] = PC + 8;
					break;
				
				case 12: //syscall
					SyscallExe(RegFile[2]);
					PC = PC + 4;
					break;
			}
		break;
		
		case 1: 
			switch(RegT){
				case 17: //bgezal
					RegFile[31] = PC + 8;
					if (RegFile[RegSource] >= 0){
					offsettemp = 0;
					offsettemp = offset;
					offsettemp = offsettemp<<2;
					RegTemp = PC + (offsettemp);
					}
					jumper = 2;
					break;
			}
		break;
		case 4: //b or beq
			newPC = PC;
			if (RegFile[RegSource] == RegFile[RegT]){
				offsettemp = 0;
				offsettemp = offset;
				offsettemp = offsettemp<<2;
				RegTemp = PC + (offsettemp);
			}
			jumper = 2;
			break;
		
		case 8: //addi
			
			immtemp = readWord(PC + 2, true);
			immediate = immtemp>>16;
			printf("Immidiate: %d\n", immediate);
		
			if ((RegFile[RegSource] + immediate)>4294967295){
				RegFile[32]=4294967295-(RegFile[RegSource] + immediate);
				RegFile[RegT] = 4294967295;
			}
			else{
			RegFile[RegT] = RegFile[RegSource] + immediate;
			}
			PC = PC + 4;
			break;
			
		case 9: //addiu
			
			immtemp = readWord(PC + 2, true);
			immediate = immtemp>>16;
			printf("Immidiate: %d\n", immediate);
		
			if ((RegFile[RegSource] + immediate)>4294967295){
				RegFile[RegT] = 4294967295;
			}
			else{
			RegFile[RegT] = RegFile[RegSource] + immediate;
			}
			PC = PC + 4;
			break;
		
		case 13: //ori,li
			//printf("Hits ori/li");
			immtemp = readWord(PC + 2, true);
			immediate = immtemp>>16;
			printf("Immidiate: %d\n", immediate);
			RegFile[RegT] = ((RegFile[RegSource])|(immediate));
			PC = PC + 4;
			break;
			
		case 15: //lui
			
			RegFile[RegT] = 0;
			immtemp = readWord(PC + 2, true);
			immediate = immtemp>>16;
			RegFile[RegT] = immediate;
			RegFile[RegT] = RegFile[RegT]<<16;
			printf("Immidiate: %d\n", immediate);
			PC = PC + 4;
			break;
			
		case 35: //lw
			
			RegFile[RegT] = readWord((RegFile[RegSource]+offset),true);
			PC = PC + 4;
			break;
	}
	
	/*int RegSource = (CurrentInstruction<<6);
	RegSource = (RegSource>>27);
	printf("RegSource: %d\n", RegSource);
	printf("CurrIns: %d\n", CurrentInstruction);
	
	int RegT = (CurrentInstruction<<11);
	RegT = (RegT>>27);
	printf("RegT: %d\n", RegT);
	printf("CurrIns: %d\n", CurrentInstruction);*/
	
	printf("RegDes: %d\n", RegDes);
	
	
		
	printf("CurrIns: %d\n", CurrentInstruction);
	//if (OpCode == 0 && RegDes == 0 && RegT == 0 && Func == 0 && Shamt == 0){
	//	break;
	//}
	RegFile[0] = 0;
	if(jumped != 0){
		
		switch(jumped){
			case 1:  //jr,jalr
				PC = RegTemp;
				break;
			case 2:	//b,bal
				PC = newPC + RegTemp;
				break;
		}
		printf("Branch delay completed\n");
		jumped = 0;
	}
	
	if(jumper != 0){
		printf("Branch delay activated\n");
		jumped = jumper;
		jumper = 0;
		PC = PC + 4;
	}
	
    } //end fori
    
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}


