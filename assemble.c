#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#define MAXLINELENGTH 1000 

typedef struct {
	char label[MAXLINELENGTH],  opcode[MAXLINELENGTH],  arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	int machine;
}Instruction;

char *arrLabel[MAXLINELENGTH];

Instruction is[MAXLINELENGTH];

int lineNum = 0, labelNum=0;
int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int isInstruction(char *);
int rType(int);
int iType(int);
int jType(int);
int oType(int);
int checkLabel(char *);

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
		switch(isInstruction(is[i].opcode)){
			
			case 0:
				rType(i);
			case 1:
				rType(i);
			default :
				rType(i);
			// case 2:
			// case 3:
			// case 4:
			// case 5:
			// case 6:
			// case 7:

			
		}
		fprintf(outFilePtr, "%d\n", is[i].machine);

	}

	// for(i=0;i<lineNum;i++){
	// 	printf("label %s\n", is[i].label );
	// }
   
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


int isInstruction(char *opcode) {
	
	if(strstr(opcode,"add")!=NULL)  return 0;
	else if(strstr(opcode,"nor")!=NULL) return 1;
	else if(strstr(opcode,"lw")!=NULL) return 2;
	else if(strstr(opcode,"sw")!=NULL) return 3;
	else if(strstr(opcode,"beq")!=NULL) return 4;
	else if(strstr(opcode,"jalr")!=NULL) return 5;
	else if(strstr(opcode,"halt")!=NULL) return 6;
	else if(strstr(opcode,"noop")!=NULL) return 7;
	else return -1;
}

int rType(int idx) {
	int result;
	int regA, regB, destReg;
	if((regA = isNumber(is[idx].arg0))!=1){
		regA = checkLabel(is[idx].arg0);
		regA = isNumber(is[regA].arg0);
	}
	if((regB = isNumber(is[idx].arg1))!=1){
		regB = checkLabel(is[idx].arg1);
		regB = isNumber(is[regB].arg1);
	}
	if((destReg = isNumber(is[idx].arg2))!=1){
		destReg = checkLabel(is[idx].arg2);
		destReg = isNumber(is[destReg].arg2);
	}
	// regB = isNumber(is[idx].arg1);
	// destReg = isNumber(is[idx].arg2);
	// if(type==0){
		result = regA << 19 ;
		result += regB << 16;
		result += destReg;
		is[idx].machine = result;
		return 0;
	// }
	// else if(type==1){}
}

int checkLabel(char *string){
  int i;
  for(i=0;i<labelNum;i++){
  	if(strcmp(string, arrLabel[i]) == 0) return i;
  }
  return 0;
}