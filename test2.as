        lw		0       1       five    load reg1 with 5 (symbolic address)        
        lw      1       2       3       load reg2 with -1 (numeric address)
        lw		0		4		stAddr 
start   add     1       2       1       decrement reg1        
		beq     0       1       done       goto end of program when reg1==0
        jalr    4       5				go back to the beginning of the loop  use jalr     
done    halt                            end of program 
five    .fill   5 
neg1    .fill   -1 
stAddr  .fill   start                   will contain the address of start (2)