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
void run();
void printState(stateType *);
int field0(int);
int field1(int);
int field2(int);

int main(int argc, char * argv[]){


}
void run(){
	while (1) {      
		printState(&state);   
		/* check for halt */     
		if (opcode(state.MEMWB.instr) == HALT) {      
			printf("machine halted\n"); 
			printf("total of %d cycles executed\n", state.cycles); 
			exit(0);
		} 
		newState = state; newState.cycles++;

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

void printintState(stateType *statePtr) {   
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