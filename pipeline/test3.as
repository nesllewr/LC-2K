        lw      0       2       count	load reg2 with count
        lw      0       3       pos		load reg3 with pos
        lw      0       4       input1   store 25 in reg4
comp    beq     4       2       done
        add     2       3       2       count++
        beq     0       0       comp    back to loop
done    noop
        halt  
count   .fill   25
pos     .fill   1
input1	.fill	50
