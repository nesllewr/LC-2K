        lw      0       1       input1	load input1 in reg1
        lw      0       2       input2	load input2 in reg2
        lw      0       3       0
        nor	1	2	4	nor reg1 & reg2 in reg4
        sw      0       5	4       store reg4 in reg5
        nor	1	1	1	completemnt of input1
        nor	2	2	2	completemnt of input2
        add 	1	2	3	add reg1 & reg2 in reg3
        sw	0	3	3
done    noop
        halt  
input1	.fill	40
input2	.fill	5
