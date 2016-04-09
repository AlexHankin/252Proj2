
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
	int InsCount = 0;

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
	printf("Current Instruction: %08x\n", readWord(PC, true));
    printf("Instructions Completed: %d\n", InsCount);
	
    int RegSource;
    int RegTemp, RegTemp2;
    int RegT;
    uint16_t immediate;
	int16_t Signimmediate;
    uint32_t immediateJump;
    uint32_t immtemp;
    int Func, j, counter;
    int RegDes;
    int JumpTemp;
    int Shamt;
    int16_t offset;
    int32_t offsettemp;
    uint16_t returnaddress;
    int8_t byteStore;
    int16_t AddModifier;
    
    
    AddModifier = 15;
    
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
    //newPC = readByte(PC,true);
    
    Func = (CurrentInstruction & 63);
    printf("Func: %d\n", Func);
    
    offsettemp = readWord(PC + 2, true);
    offset = offsettemp>>16;
    printf("Offset: %d\n", offset);
    int byteNo = (RegFile[RegSource]+offset)%4;
    
    switch(OpCode) 
    {
        case 0:
            switch(Func) 
            {
                case 32: //add
                    RegFile[RegDes] = RegFile[RegSource]+RegFile[RegT];
                    PC = PC + 4;
                    break;
                
                case 33: //addu     
                    if ((RegFile[RegSource] + RegFile[RegT])>2147483647){
					printf("Arith Overflow");
					RegFile[RegT] = 2147483647;
					}
					else if ((RegFile[RegSource] + RegFile[RegT])<-2147483648){
					printf("Arith Overflow");
					RegFile[RegT] = -2147483648;
					}
                    else{
                    RegTemp = RegFile[RegSource] + RegFile[RegT];
                    RegFile[RegDes] = RegTemp;
                    }
                    PC = PC + 4;
                    break;
                
                case 34: //sub
                    RegFile[RegDes] = RegFile[RegSource]-RegFile[RegT];
                    PC = PC + 4;
                    break;
                    
                case 35: //subu
                    if ((RegFile[RegSource] - RegFile[RegT]) < 0){
                        RegFile[RegT] = 0;
                    }
                    else{
                    RegTemp = RegFile[RegSource] - RegFile[RegT];
                    RegFile[RegT] = RegTemp;
                    }
                    PC = PC + 4;
                    break;
                
                case 26: //div
                    //RegFile[32] = 0;
                    //RegFile[33] = 0;
                    RegFile[32] = RegFile[RegSource]%RegFile[RegT];
                    RegFile[33] = (RegFile[RegSource]-RegFile[32])/RegFile[RegT];
                    PC = PC + 4;
                    break;
                    
                case 27: //divu
                    RegFile[32] = RegFile[RegSource]%RegFile[RegT];
                    RegFile[33] = (RegFile[RegSource]-RegFile[32])/RegFile[RegT];
                    PC = PC + 4;
                    break;
                
                case 24: //mult
                    //RegFile[33] = 0;
                    RegFile[33] = RegFile[RegSource]*RegFile[RegT];
                    PC = PC + 4;
                    break;
                    
                case 25: //multu
                    if ((RegFile[RegSource]*RegFile[RegT]) > 4294967295){
                        RegFile[32]=4294967295-(RegFile[RegSource]*RegFile[RegT]);
                        RegFile[33] = 4294967295;
                    }
                    else{
                    RegTemp = RegFile[RegSource]*RegFile[RegT];
                    RegFile[33] = RegTemp;
                    }
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
                
                case 36: //add
                    RegFile[RegDes] = RegFile[RegSource] & RegFile[RegT];
                    PC = PC + 4;
                    break;
                
                case 37: //or
                    RegFile[RegDes] = RegFile[RegSource] | RegFile[RegT];
                    PC = PC + 4;
                    break;
                
                case 38: //xor
                    RegFile[RegDes] = (RegFile[RegSource] ^ RegFile[RegT]);
                    PC = PC + 4;
                    break;
                    
                case 39: //nor
                    RegFile[RegDes] = ~((RegFile[RegSource] | RegFile[RegT]));
                    PC = PC + 4;
                    break;
                
                case 0: //sll
                    RegTemp = (RegFile[RegT]<<Shamt);
                    RegFile[RegDes] = RegTemp;
                    PC = PC + 4;
                    break;
                    
                case 2: //srl
                    RegTemp = (RegFile[RegT]>>Shamt);
                    RegFile[RegDes] = RegTemp;
                    PC = PC + 4;
                    break;
                    
                case 3: //sra
                    RegTemp = RegFile[RegT]>>Shamt;
                    RegFile[RegDes] = RegTemp;
                    PC = PC + 4;
                    break;
                    
                case 4: //sllv
                    RegTemp = (RegFile[RegT]<<RegFile[RegSource]);
                    RegFile[RegDes] = RegTemp;
                    PC = PC + 4;
                    break;
                    
                case 5: //srlv
                    RegTemp = (RegFile[RegT]>>RegFile[RegSource]);
                    RegFile[RegDes] = RegTemp;
                    PC = PC + 4;
                    break;
                    
                case 7: //srav
                    RegTemp = RegFile[RegT]>>RegFile[RegSource];
                    RegFile[RegDes] = RegTemp;
                    PC = PC + 4;
                    break;
                
                case 42: //slt
                    if (RegFile[RegSource]<RegFile[RegT]){
                        RegFile[RegDes] = 1;
                    }
                    else{
                        RegFile[RegDes] = 0;
                    }
                    PC = PC + 4;
                    break;
                
                case 43: //sltu
                    if(RegFile[RegSource] < 0){
                        RegTemp = 0;
                    }
                    else{
                        RegTemp = RegFile[RegSource];
                    }
                    if(RegFile[RegT] < 0){
                        RegTemp2 = 0;
                    }
                    else{
                        RegTemp2 = RegFile[RegT];
                    }
                    if (RegTemp<RegTemp2){
                        RegFile[RegDes] = 1;
                    }
                    else{
                        RegFile[RegDes] = 0;
                    }
                    PC = PC + 4;
                    break;
                
                case 8: //jr
                    jumper = 3;
                    //returnaddress = RegSource;
                    //returnaddress = returnaddress<<11;
                    //returnaddress = returnaddress>>11;
                    //printf("Return Address: %d\n", returnaddress);
                    JumpTemp = RegFile[RegSource];
                    break;
                    
                case 9: //jalr
                    jumper = 3;
                    returnaddress = RegDes;
                    returnaddress = returnaddress<<11;
                    returnaddress = returnaddress>>11;
                    printf("Return Address: %d\n", returnaddress);
                    JumpTemp = RegFile[RegSource];
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
                    newPC = PC;
                    RegFile[31] = PC + 8;
                    if (RegFile[RegSource] >= 0){
                    offsettemp = 0;
                    offsettemp = offset;
                    offsettemp = offsettemp<<2;
                    JumpTemp = offsettemp;
                    }
                    else{
                        PC = PC + 4;
                    }
                    jumper = 2;
                    break;
                    
                case 1: //bgez
                    newPC = PC;
                    if (RegFile[RegSource] >= 0){
                    offsettemp = 0;
                    offsettemp = offset;
                    offsettemp = offsettemp<<2;
                    JumpTemp = offsettemp;
                    }
                    else{
                        PC = PC + 4;
                    }
                    jumper = 2;
                    break;
                    
                case 0: //bltz
                    newPC = PC;
                    if (RegFile[RegSource] < 0){
                    offsettemp = 0;
                    offsettemp = offset;
                    offsettemp = offsettemp<<2;
                    JumpTemp = offsettemp;
                    jumper = 2;
                    }
                    else{
                        PC = PC + 4;
                    }
                    break;
                    
                case 16: //bltzal
                    newPC = PC;
                    RegFile[31] = PC + 8;
                    if (RegFile[RegSource] < 0){
                    offsettemp = 0;
                    offsettemp = offset;
                    offsettemp = offsettemp<<2;
                    JumpTemp = offsettemp;
                    jumper = 2;
                    }
                    else{
                        PC = PC + 4;
                    }
                    break;
            }
            break;
        
        case 2: //j
            jumper = 1;
            immtemp = readWord(PC, true);
            immediateJump = immtemp<<6;
            immediateJump = immediateJump>>4;
            printf("Immidiate: %d\n", immediate);
            JumpTemp = immediateJump;
            break;
            
        case 3: //jal
            jumper = 1;
            immtemp = readWord(PC, true);
            immediateJump = immtemp<<6;
            immediateJump = immediateJump>>4;
            printf("Immidiate: %d\n", immediate);
            JumpTemp = immediateJump;
            RegFile[31] = PC + 8;
            break;
        
        case 4: //b or beq
            newPC = PC;
            if (RegFile[RegSource] == RegFile[RegT]){
                offsettemp = 0;
                offsettemp = offset;
                offsettemp = offsettemp<<2;
                JumpTemp = offsettemp;
                jumper = 2;
            }
            else{
                PC = PC + 4;
            }
            break;
            
        case 5: //bne
            newPC = PC;
            
            //RegTemp = PC + (offsettemp);
            if (RegFile[RegSource] != RegFile[RegT]){
                offsettemp = 0;
                offsettemp = offset;
                offsettemp = offsettemp<<2;
                JumpTemp = offsettemp;
                printf("AAAAAAAAAA\n");
                jumper = 2;
            }
            else{
                PC = PC + 4;
            }
            break;
            
        case 6: //blez
            newPC = PC;
            if (RegFile[RegSource] <= 0){
            offsettemp = 0;
            offsettemp = offset;
            offsettemp = offsettemp<<2;
            JumpTemp = offsettemp;
            }
            else{
            PC = PC + 4;
            }
            jumper = 2;
            break;
        
        case 7: //bgtz
            newPC = PC;
            if (RegFile[RegSource] > 0){
                offsettemp = 0;
                offsettemp = offset;
                offsettemp = offsettemp<<2;
                JumpTemp = offsettemp;
                jumper = 2;
            }
            else{
                PC = PC + 4;
            }
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
            Signimmediate = immtemp>>16;
            printf("Immidiate: %d\n", Signimmediate);
        
            if ((RegFile[RegSource] + Signimmediate)>2147483647){
                printf("Arith Overflow");
                RegFile[RegT] = 2147483647;
            }
			else if ((RegFile[RegSource] + Signimmediate)<-2147483648){
                printf("Arith Overflow");
                RegFile[RegT] = -2147483648;
			}
            else{
            RegFile[RegT] = RegFile[RegSource] + Signimmediate;
            }
            PC = PC + 4;
            break;
            
        case 10: //slti
            immtemp = readWord(PC + 2, true);
            immediate = immtemp>>16;
            printf("Immidiate: %d\n", immediate);
            if (RegFile[RegSource]<immediate){
                RegFile[RegT] = 1;
            }
            else{
                RegFile[RegT] = 0;
            }
            PC = PC + 4;
            break;
        
        case 11: //sltiu
            immtemp = readWord(PC + 2, true);
            immediate = immtemp>>16;
            printf("Immidiate: %d\n", immediate);
            if(RegFile[RegSource] < 0){
                RegTemp = 0;
            }
            else{
                RegTemp = RegFile[RegSource];
            }
            if(immediate < 0){
                RegTemp2 = 0;
            }
            else{
                RegTemp2 = RegFile[RegT];
            }
            if (RegTemp<RegTemp2){
                RegFile[RegT] = 1;
            }
            else{
                RegFile[RegT] = 0;
            }
            PC = PC + 4;
            break;
        
        case 12: //adni
            immtemp = readWord(PC + 2, true);
            immediate = immtemp>>16;
            printf("Immidiate: %d\n", immediate);
            RegFile[RegT] = ((RegFile[RegSource])&(immediate));
            PC = PC + 4;
            break;

        case 13: //ori,li
            immtemp = readWord(PC + 2, true);
            immediate = immtemp>>16;
            printf("Immidiate: %d\n", immediate);
            RegFile[RegT] = ((RegFile[RegSource])|(immediate));
            PC = PC + 4;
            break;
            
        case 14: //xori
            immtemp = readWord(PC + 2, true);
            immediate = immtemp>>16;
            printf("Immidiate: %d\n", immediate);
            RegFile[RegT] = (RegFile[RegSource] ^ immediate);
            PC = PC + 4;
            break;
            
        case 15: //lui
            
            immtemp = readWord(PC + 2, true);
            immediate = immtemp>>16;
            RegFile[RegT] = immediate;
            printf("Immidiate: %d\n", immediate);
			RegFile[RegT] = RegFile[RegT]<<16;
            PC = PC + 4;
            break;
            
        case 20: //beql 
            newPC = PC;
            if (RegFile[RegSource] == RegFile[RegT]){
                offsettemp = 0;
                offsettemp = offset;
                offsettemp = offsettemp<<2;
                RegTemp = PC + (offsettemp);
                jumper = 2;
            }
            else{
                PC = PC + 8;
            }
            break;
            
        case 21: //bnel
            newPC = PC;
            if (RegFile[RegSource] != RegFile[RegT]){
                offsettemp = 0;
                offsettemp = offset;
                offsettemp = offsettemp<<2;
                RegTemp = PC + (offsettemp);
                jumper = 2;
            }
            else{
                PC = PC + 8;
            }
            break;
        
        case 22: //blezl
            newPC = PC;
            if (RegFile[RegSource] <= RegFile[RegT]){
                offsettemp = 0;
                offsettemp = offset;
                offsettemp = offsettemp<<2;
                RegTemp = PC + (offsettemp);
                jumper = 2;
            }
            else{
                PC = PC + 8;
            }
            break;
            
        case 32: //lb
            RegFile[RegT] = readByte((RegFile[RegSource]+offset),true);
            PC = PC + 4;
            break; 
            
        case 33: //lh
            immtemp = readWord((RegFile[RegSource]+offset),true);
            immtemp = immtemp>>16;
            RegFile[RegT]=immtemp;
            PC = PC + 4;
            break;
            
        case 34: //lwl
            counter = 0;
            AddModifier = 0;
            for(j = byteNo; j<4;j += 1)
            {
            byteStore = readByte((RegFile[RegSource]+offset + j),true);
            byteStore = byteStore<<(16 - (4*counter));
            RegFile[RegT] = RegFile[RegT] | (~(AddModifier<<(16 - (4*counter)))) ;
            RegFile[RegT] = RegFile[RegT] | byteStore;
            }
            PC = PC + 4;
            break;
            
        case 35: //lw
			printf("Reading Line Begin\n");
            RegFile[RegT] = readWord((RegFile[RegSource]+offset),true);
			printf("Reading Line End\n");
            PC = PC + 4;
            break;
        
        case 36: //lbu
            if (RegFile[RegSource] + offset >= 65535){
                RegFile[RegT] = 65535;
            }
            else{
            RegFile[RegT] = readByte((RegFile[RegSource]+offset),true);
            }
            PC = PC + 4;
            break; 
            
        case 37: //lhu
            if (RegFile[RegSource] + offset >= 65535){
                RegFile[RegT] = 65535;
            }
            else{
            immtemp = readWord((RegFile[RegSource]+offset),true);
            immtemp = immtemp>>16;
            RegFile[RegT]=immtemp;
            RegFile[RegT]=immtemp;
            }
            PC = PC + 4;
            break; 
            
        case 38: //lwr
            counter = 0;

            AddModifier = 0;
            for(j = byteNo; j>=0;j -= 1)
            {
            byteStore = readByte((RegFile[RegSource]+offset - j),true);
            byteStore = byteStore<<(4*counter);
            RegFile[RegT] = RegFile[RegT] | (~(AddModifier<<(4*counter))) ;
            RegFile[RegT] = RegFile[RegT] | byteStore;
            counter = counter + 1;
            }
            PC = PC + 4;
            break;
            
        case 40: //sb
            writeByte((RegFile[RegSource] + offset),RegFile[RegT],true);
            PC = PC + 4;
            break;
        
        case 41: //sh
            immtemp = RegFile[RegT];
            immtemp = immtemp>>16;
            writeWord((RegFile[RegSource]+offset), immtemp, true);
            immtemp = RegFile[RegT];
            immtemp = immtemp<<16;
            immtemp = immtemp>>16;
            writeWord((RegFile[RegSource]+offset+16), immtemp, true);
            PC = PC + 4;
            break;
            
        case 42: //swl
            counter = 0;
            for(j = byteNo; j<4; j += 1)
            {
            byteStore = RegFile[RegT]<<(4*counter);
            byteStore = byteStore>>12;
            writeWord(RegFile[RegSource] + offset + j, byteStore,true);
            counter = counter + 1;
            }
            PC = PC + 4;
            break;
            
        case 43: //sw
            writeWord((RegFile[RegSource] + offset),RegFile[RegT],true);
            PC = PC + 4;
            break;
            
        case 46: //swr
            counter = 0;
            for(j = 0; j<=byteNo;j += 1)
            {
            byteStore = RegFile[RegT]<<(16-(4+(4*counter)));
            byteStore = byteStore>>12;
            writeWord(RegFile[RegSource] + offset - j, byteStore,true);
            counter = counter + 1;
            }
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
    //  break;
    //}
    RegFile[0] = 0;
    if(jumped != 0){
        
        switch(jumped){
			printf("Jumped: %d\n", jumped);
            case 1:  //j,jal
				PC = PC>>28;
				PC = PC<<28;
                RegTemp = JumpTemp;
                PC = PC | RegTemp;
                break;
            case 2: //b,bal
				printf("NewPc: %08x\n", newPC);
				printf("JumpTemp: %d\n", JumpTemp);
                RegTemp = JumpTemp;
				newPC = newPC + 4;
                PC = newPC + RegTemp;
                break;
			case 3: //jalr, jr
				RegTemp = JumpTemp;
				PC = RegTemp;
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
    InsCount = InsCount + 1;
    } //end fori
    
    
    //Close file pointers & free allocated Memory
    closeFDT();
    CleanUp();
    return 1;
}


