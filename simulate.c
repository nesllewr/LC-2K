#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#define NUMMEMORY 65536 /* maximum number of words in memory */ 
#define NUMREGS 8 /* number of machine registers */ 
#define MAXLINELENGTH 1000
#define BITS 0b111

typedef struct stateStruct {  
	int pc;   
	int mem[NUMMEMORY];  
	int reg[NUMREGS];   
	int numMemory; 
} stateType;

enum opcode {
	add =0,
	nor, lw, sw, beq,jalr, halt, noop
};
typedef struct{
	int opcode, regA, regB;
}Instruction;

Instruction decode(int);
void rType(Instruction,int);
void iType(Instruction, int);
void jType(Instruction,int);
void oType(Instruction,int);
int convertNum(int);
void printState(stateType *); 

	stateType state;   
int main(int argc, char *argv[]) { 

	char line[MAXLINELENGTH];  
	FILE *filePtr;   
	int isNum=0;

	if (argc != 2) {   
		printf("error: usage: %s <machine-code file>\n", argv[0]); 
		exit(1); 
	}   

	filePtr = fopen(argv[1], "r"); 

	if (filePtr == NULL) {    
		printf("error: can't open file %s", argv[1]);    
		perror("fopen");        
		exit(1);  
	}    /* read in the entire machine-code file into memory */   

	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;state.numMemory++) {  
		if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {      
			printf("error in reading address %d\n", state.numMemory);    
			exit(1);    
		}      
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);  
	}

	memset(state.reg, 0 , sizeof(int)*NUMREGS);
	printState(&state);

	while(1){

		if(state.pc < 0 || state.pc > NUMMEMORY-1){
			printf("error : out of range\n");
			exit(1);
		}

		int cur = state.mem[state.pc++];
		Instruction is = decode(cur);
		isNum++;
		
		switch(is.opcode){
			case add:
				rType(is,cur);
				break;
			case nor:
				rType(is,cur);
				break;
			case lw:
				iType(is,cur);
				break;
			case sw:
				iType(is,cur);
				break;
			case beq:
				iType(is,cur);
				break;
			case jalr:
				jType(is,cur);
				break;		
			case halt:
				oType(is,cur);
				printf("total of %d instructions executed\n", isNum);
				printf("final state of machine:\n");
				break;
			case noop:
				oType(is,cur);
				break;
			default :
				printf("error : unrecognized opcode\n");
				//printf("%s\n", opcode);
				exit(1);			
		}
		printState(&state);
		if(is.opcode==halt) break;
	}


	return(0);
} 
Instruction decode(int context){
	Instruction is;
	is.opcode = (context>>22) & BITS;
	is.regA = (context>>19) & BITS;
	is.regB = (context>>16) & BITS;
	if(is.opcode < 0 || is.opcode > 7){
		printf("error : out of opcode range\n");
		exit(1);
	}
	if(is.regA < 0 || is.regB < 0 || is.regA >= NUMREGS || is.regB >=NUMREGS){
		printf("error : out of register range\n");
		exit(1);
	}
	return is;
}
void rType(Instruction is, int context){
	int destReg = context & BITS;
	if(is.opcode==add){
		state.reg[destReg] = state.reg[is.regA] + state.reg[is.regB];
	}
	else if(is.opcode==nor){
		state.reg[destReg] = ~(state.reg[is.regA] | state.reg[is.regB]);
	}
}
void iType(Instruction is, int context){
	int offset = convertNum(context & 0b1111111111111111);

	if(offset > 32767||offset<-32768){
		printf("error : out of offset range\n");
		exit(1);
	}

	if(is.opcode==lw){
		state.reg[is.regB] = state.mem[offset + state.reg[is.regA]];
	}
	else if(is.opcode==sw){
		state.reg[is.regA] = state.mem[offset + state.reg[is.regB]];
	}
	else if(is.opcode==beq){
		if(state.reg[is.regA]==state.reg[is.regB]){
			state.pc += offset;
		}

	}
}
void jType(Instruction is, int context){

	state.reg[is.regB] = state.pc +1;
	state.pc = state.reg[is.regA];

}
void oType(Instruction is, int context){
	if(is.opcode == halt){
		printf("machine halted\n");
	}
}

int convertNum(int num){
	if(num & (1<<15))
		num-=(1<<16);
	return num;
}

void printState(stateType *statePtr) {  

	int i;  

	printf("\n@@@\nstate:\n");   
	printf("\tpc %d\n", statePtr->pc);  
	printf("\tmemory:\n"); 

	for (i=0; i<statePtr->numMemory; i++) {    
		printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);  
	}  
	
	printf("\tregisters:\n");   
	
	for (i=0; i<NUMREGS; i++) {  
		printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]); 
	}   

	printf("end state\n"); 
}
