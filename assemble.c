#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#define MAXLINELENGTH 1000 

typedef struct {
	char label[MAXLINELENGTH],  opcode[MAXLINELENGTH],  arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	
}Instruction;

char *arrLabel[MAXLINELENGTH];
int machine[MAXLINELENGTH];

Instruction is[MAXLINELENGTH];

int lineNum = 0, labelNum=0;
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int isInstruction(char *, int idx);
int rType(int);
int iType(int);
int jType(int);
int oType(int);
int fill(int);
int checkLabel(char *);
int findLabel(char*);

int main(int argc, char *argv[]) { 

	char *inFileopcode, *outFileopcode; 
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],     
		arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) { 
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]); 
		exit(1);    
	}

	inFileopcode = argv[1];  
	outFileopcode = argv[2];   
	
	inFilePtr = fopen(inFileopcode, "r"); 
	if (inFilePtr == NULL) {  
		printf("error in opening %s\n", inFileopcode);  
		exit(1);
    }

	outFilePtr = fopen(outFileopcode, "w");
	if (outFilePtr == NULL) {  
		printf("error in opening %s\n", outFileopcode); 
		exit(1);
	}  

	if (! (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) )) {
    }    


	int i;

	for( i = 0; i < lineNum; i++){
		switch(isInstruction(is[i].opcode, i)){
			case 0:
				rType(i);
				break;
			case 1:
				rType(i);
				break;
			case 2:
				iType(i);
				break;
			case 3:
				iType(i);
				break;
			case 4:
				iType(i);
				break;
			case 5:
				jType(i);
				break;
			case 6:
				oType(i);
				break;
			case 7:
				oType(i);
				break;
			case 8:
				fill(i);
				break;
				
			default :
				printf("error : nonexist opcode\n");
				exit(1);
			
		}
		fprintf(outFilePtr, "is[%d]: %d\n", i,machine[i]);

	}   
	fclose(outFilePtr);
    return(0); 
}

int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {    
	
	char line[MAXLINELENGTH];   
	char *ptr = line;    /* delete prior values */   
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';    /* read the line from the assembly-language file */  
	
	while (fgets(line, MAXLINELENGTH, inFilePtr) != NULL) {        /* reached end of file */     
		
		if (strchr(line, '\n') == NULL) {        /* line too long */      
			printf("error: line too long\n");     
			exit(1);
	    }   
		ptr = line;   

		memset(is[lineNum].label, '\0', MAXLINELENGTH);

		if (sscanf(ptr, "%[^\t\n\r ]", is[lineNum].label)) {  
			ptr += strlen(is[lineNum].label);    
		}    
		
		sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", is[lineNum].opcode, is[lineNum].arg0, is[lineNum].arg1, is[lineNum].arg2);   
		
		if(is[lineNum].label[0] != '\0'){
			if(checkLabel(is[lineNum].label)!=0){
				printf("error : same label\n");
				exit(1);
			}
			arrLabel[labelNum++] = is[lineNum].label;
		}	
		//is[lineNum].index = lineNum;
		lineNum++;

    }   
	return 1;
}

int isNumber(char *string) {    /* return 1 if opcode is a number */  
	int i;  
	return( (sscanf(string, "%d", &i)) == 1); 
}

int isInstruction(char *opcode, int i) {
	
	if(strstr(opcode,"add")!=NULL) {
		machine[i] = 0;
		return 0;
	} 
	else if(strstr(opcode,"nor")!=NULL){
		machine[i] = 1 << 22;
		return 1;
	} 
	else if(strstr(opcode,"lw")!=NULL) {
		machine[i] = 2 << 22;
		return 2;
	}
	else if(strstr(opcode,"sw")!=NULL) {
		machine[i] = 3 << 22;
		return 3;
	}
	else if(strstr(opcode,"beq")!=NULL) {
		machine[i] = 4 << 22;
		return 4;
	}
	else if(strstr(opcode,"jalr")!=NULL){
		machine[i] = 5 << 22;
		return 5;
	} 
	else if(strstr(opcode,"halt")!=NULL) {
		machine[i] = 6 << 22;
		return 6;
	}
	else if(strstr(opcode,"noop")!=NULL){
		machine[i] = 7 << 22;
		return 7;
	} 
	else if(strstr(opcode, ".fill")!=NULL){
		return 8;
	}
	else return -1;
}

int rType(int idx) {
	int result;
	int regA, regB, destReg;

	if((regA = isNumber(is[idx].arg0))!=1){
		regA = findLabel(is[idx].arg0);
	}
	else sscanf(is[idx].arg0, "%d", &regA);

	if((regB = isNumber(is[idx].arg1))!=1){
		regB = findLabel(is[idx].arg1);
	}
	else sscanf(is[idx].arg1, "%d", &regB);

	if((destReg = isNumber(is[idx].arg2))!=1){
		destReg = findLabel(is[idx].arg2);
	}
	else sscanf(is[idx].arg2, "%d", &destReg);

	result = regA << 19 ;
	result += regB << 16;
	result += destReg;
	machine[idx] += result;

	return 0;
}
int iType(int idx){
	int result;
	int regA, regB, offset;

	if((regA = isNumber(is[idx].arg0))!=1){
		regA = findLabel(is[idx].arg0);
	}
	else sscanf(is[idx].arg0, "%d", &regA);

	if((regB = isNumber(is[idx].arg1))!=1){
		regB = findLabel(is[idx].arg1);
	}
	else sscanf(is[idx].arg1, "%d", &regB);

	if((offset = isNumber(is[idx].arg2))!=1){
		offset = findLabel(is[idx].arg2);
	}
	else {
		sscanf(is[idx].arg2, "%d", &offset);
		if(offset > 32767 && offset < -32768){
			printf("error : wrong range offest\n");
			exit(1);
		}
	}
	result = regA << 19 ;
	result += regB << 16;
	result += offset;
	machine[idx] += result;

	return 0;
}
int jType(int idx){
	machine[idx] =0;

	return 0;
}
int oType(int idx){
	return 0;
}
int fill(int idx){
	int i;
	if(isNumber(is[idx].arg0)!=1){
		machine[idx] = findLabel(is[idx].arg0);
	}
	else {
		sscanf(is[idx].arg0, "%d", &i);
		machine[idx] = i;
	} 
	return 0;
}

int checkLabel(char *string){
  int i;
  for(i=0;i<labelNum;i++){
  	if(strcmp(string, arrLabel[i]) == 0) return i;
  }
  return 0;
}

int findLabel(char *string){
	int i;
	for(i=0;i<lineNum;i++){
		if(strcmp(is[i].label, string) == 0) return i;
	}
}