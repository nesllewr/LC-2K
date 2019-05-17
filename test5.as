        lw      0       1       pos1    load reg1 with 1
        lw      0       2       cntAdr  load reg2 with address of cnt function
        jalr    2       6
count   lw      0       3       addrress   load start address of A array
        add     0       0       2
loop    lw      3       4       0       load reg4 with A[reg3]
        beq     0       4       start
        add     1       2       2
        add     1       3       3
        beq     0       0       loop      count elements in A array
start   add     0       2       1       store counted elements into reg1
        jalr    6       2
done    noop
        halt                            end of program
pos1    .fill   1
cntAdr  .fill   count
addrA   .fill   A
A       .fill   4