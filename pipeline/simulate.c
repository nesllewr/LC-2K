#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of data words in memory */ 
#define NUMREGS 8 /* number of machine registers */ 
#define ADD 0
#define NOR 1 
#define LW 2 
#define SW 3 
#define BEQ 4 
#define JALR 5 /* JALR will not implemented for this project */ 
#define HALT 6 
#define NOOP 7 
#define NOOPINSTRUCTION 0x1c00000 
#define MAXLINELENGTH 1000

typedef struct IFIDStruct {
	int instr;
	int pcPlus1;
} IFIDType; 

typedef struct IDEXStruct {       
	int instr;      
	int pcPlus1;   
	int readRegA;    
	int readRegB;       
	int offset;
} IDEXType; 

typedef struct EXMEMStruct {        
	int instr;      
	int branchTarget;   
	int aluResult;      
	int readRegB;
} EXMEMType; 

typedef struct MEMWBStruct {      
	int instr;     
	int writeData; 
} MEMWBType; 

typedef struct WBENDStruct {      
	int instr;     
	int writeData; 
} WBENDType; 

typedef struct stateStruct {     
	int pc;      
	int instrMem[NUMMEMORY];      
	int dataMem[NUMMEMORY];      
	int reg[NUMREGS];     
	int numMemory;     
	IFIDType IFID;      
	IDEXType IDEX;       
	EXMEMType EXMEM;      
	MEMWBType MEMWB;      
	WBENDType WBEND;      
	int cycles; /* number of cycles run so far */ 
} stateType;

// enum opcode {
// 	add =0,
// 	nor, lw, sw, beq,jalr, halt, noop
// };


void run(stateType *, stateType *);
int convertNum(int);

void printState(stateType *);
int field0(int);
int field1(int);
int field2(int);
int opcode(int);
void printInstruction(int);

void IF(stateType *, stateType *);
void ID(stateType *, stateType *);
void EX(stateType *, stateType *);
void MEM(stateType *, stateType *);
void WB(stateType *, stateType *);
int isHazard(stateType *);

int main(int argc, char * argv[]){

	stateType *state, *newState;
	char line[MAXLINELENGTH];
	FILE *filePtr;
	state = malloc(sizeof(stateType));
	newState = malloc(sizeof(stateType));
	int i;

	if (argc != 2) {
		printf("error: usage: %s <machine-code file>\n", argv[0]);
		exit(1);
	}

	filePtr = fopen(argv[1], "r");
	if (filePtr == NULL) {
		printf("error: can't open file %s", argv[1]);
		perror("fopen");
		exit(1);
	}

	//initialize
	state->pc = 0;
	state->cycles = 0;
	memset(state, 0, sizeof(stateType));
	state->IFID.instr = NOOPINSTRUCTION;
	state->IDEX.instr = NOOPINSTRUCTION;
	state->EXMEM.instr = NOOPINSTRUCTION;
	state->MEMWB.instr = NOOPINSTRUCTION;
	state->WBEND.instr = NOOPINSTRUCTION;

	//load instruction
	for (state->numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;	state->numMemory++) {
		if (sscanf(line, "%d", state->instrMem+state->numMemory) != 1) {
			printf("error in reading address %d\n", state->numMemory);
			exit(1);
		}
		if(state->numMemory >= NUMMEMORY) {
			printf("error : memory exceeded. Max = %d\n", NUMMEMORY);
			exit(1);
		}
		state->dataMem[state->numMemory] = state->instrMem[state->numMemory];
		printf("memory[%d]=%d\n", state->numMemory, state->instrMem[state->numMemory]);
	}

	printf("%d memory words\n", state->numMemory);
	printf("\tinstruction memory:\n");

	for (i = 0; i < state->numMemory; i++) {
		printf("\t\tinstrMem[ %d ] ", i);
		printInstruction(state->instrMem[i]);
	}


	run(state, newState);

	return 0;
}


void run(stateType *state, stateType *newState){
	while (1) {      
		printState(state);   
		/* check for halt */     
		if (opcode(state->MEMWB.instr) == HALT) {      
			printf("machine halted\n"); 
			printf("total of %d cycles executed\n", state->cycles); 
			exit(0);
		} 
		newState = state;
		newState->cycles++;

		/* --------------------- IF stage --------------------- */ 
		/* --------------------- ID stage --------------------- */
		/* --------------------- EX stage --------------------- */ 
		/* --------------------- MEM stage --------------------- */ 
		/* --------------------- WB stage --------------------- */ 
		state = newState; 
		/* this is the last statement before end of the loop.   
		It marks the end of the cycle and updates the  
		current state with the values calculated in this   cycle */ 
	}
}

int convertNum(int num){
	if(num & (1<<15))
		num-=(1<<16);
	return num;
}

void printState(stateType *statePtr) {   
	int i;   
	printf("\n@@@\nstate before cycle %d starts\n", statePtr->cycles);  
	printf("\tpc %d\n", statePtr->pc);
    printf("\tdata memory:\n");

	for (i=0; i<statePtr->numMemory; i++) {   
		printf("\t\tdataMem[ %d ] %d\n", i, statePtr->dataMem[i]);
	}  
	
	printf("\tregisters:\n"); 
	
	for (i=0; i<NUMREGS; i++) {    
		printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}  

	printf("\tIFID:\n"); 
		printf("\t\tinstruction "); 
		printInstruction(statePtr->IFID.instr); 
		printf("\t\tpcPlus1 %d\n", statePtr->IFID.pcPlus1);
	printf("\tIDEX:\n"); 
		printf("\t\tinstruction "); 
		printInstruction(statePtr->IDEX.instr); 
		printf("\t\tpcPlus1 %d\n", statePtr->IDEX.pcPlus1); 
		printf("\t\treadRegA %d\n", statePtr->IDEX.readRegA); 
		printf("\t\treadRegB %d\n", statePtr->IDEX.readRegB);
		printf("\t\toffset %d\n", statePtr->IDEX.offset);   
	printf("\tEXMEM:\n"); printf("\t\tinstruction "); 
		printInstruction(statePtr->EXMEM.instr); 
		printf("\t\tbranchTarget %d\n", statePtr->EXMEM.branchTarget);
		printf("\t\taluResult %d\n", statePtr->EXMEM.aluResult);
		printf("\t\treadRegB %d\n", statePtr->EXMEM.readRegB);   
	printf("\tMEMWB:\n"); 
		printf("\t\tinstruction ");
		printInstruction(statePtr->MEMWB.instr); 
		printf("\t\twriteData %d\n", statePtr->MEMWB.writeData); 
	printf("\tWBEND:\n");
		printf("\t\tinstruction ");
		printInstruction(statePtr->WBEND.instr); 
		printf("\t\twriteData %d\n", statePtr->WBEND.writeData);
} 
int field0(int instruction) { 
	return( (instruction>>19) & 0x7); 
} 
int field1(int instruction) { 
	return( (instruction>>16) & 0x7); 
}
int field2(int instruction) {
	return(instruction & 0xFFFF); 
} 
int opcode(int instruction) { 
	return(instruction>>22); 
} 
void printInstruction(int instr) {
	char opcodeString[10];
	if (opcode(instr) == ADD) {  strcpy(opcodeString, "add"); } 
	else if (opcode(instr) == NOR) {  strcpy(opcodeString, "nor"); }
	else if (opcode(instr) == LW) {  strcpy(opcodeString, "lw"); } 
	else if (opcode(instr) == SW) {  strcpy(opcodeString, "sw"); } 
	else if (opcode(instr) == BEQ) {  strcpy(opcodeString, "beq"); } 
	else if (opcode(instr) == JALR) {  strcpy(opcodeString, "jalr"); }
	else if (opcode(instr) == HALT) {  strcpy(opcodeString, "halt"); } 
	else if (opcode(instr) == NOOP) {  strcpy(opcodeString, "noop"); } 
	else {  strcpy(opcodeString, "data");    }    
	printf("%s %d %d %d\n", opcodeString, field0(instr), field1(instr),  field2(instr)); 
}


void IF(stateType *state, stateType *newState){

	newState->IFID.instr = state->instrMem[state->pc];
	newState->IFID.pcPlus1 = newState->pc = state->pc+1;	

}
void ID(stateType *state, stateType *newState){
	if (isHazard(state)) {
		newState->pc = state->pc;
		newState->IFID = state->IFID;
		newState->IDEX.instr = NOOPINSTRUCTION;
		// newState->IFID.instr = state->IFID.instr;
		// newState->IFID.pcPlus1 = state->pc;
		// newState->IDEX.pcPlus1 = 0;
		// newState->IDEX.readRegA = 0;
		// newState->IDEX.readRegB = 0;
		// newState->IDEX.offset = 0;
	}
	else {
		newState->IDEX.instr = state->IFID.instr;
		newState->IDEX.pcPlus1 = state->IFID.pcPlus1;
		newState->IDEX.readRegA = state->reg[field0(state->IFID.instr)];
		newState->IDEX.readRegB = state->reg[field1(state->IFID.instr)];
		newState->IDEX.offset = convertNum(field2(state->IFID.instr));
	}

}
void EX(stateType *state, stateType *newState){

}
void MEM(stateType *state, stateType *newState){

}
void WB(stateType *state, stateType *newState){

}

int isHazard(stateType *state){

	int curIS = opcode(state->IFID.instr);
	if(curIS == LW){
		int destReg = field1(state->IDEX.instr);
		int regA = field0(state->IDEX.instr);
		int regB = field1(state->IDEX.instr);
		if (curIS == ADD || curIS == NOR || curIS == BEQ) {
			return (destReg == regA || destReg == regB);
		}
		else if (curIS == LW || curIS == SW) {
			return (destReg == regA);
		}
		
	}
	return 0;

}