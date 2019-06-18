        lw      0       2       mcand
        lw      0       3       mplier
        lw      0       4       bit     bitmask
        lw      0       5       count   left shift until count reach mplier
        nor     3       3       5       store complement of mplier in reg5
loop    beq     7       3       done    compare count and mplier
        nor     4       4       4       complement of bitmask
        nor     5       4       6       put mplier&bitmask in reg6
        add     6       7       7       count++ abouts it multiply
        beq     6       0       lshift  go to lshift if bitmask is 0
        add     1       2       1       
lshift  nor     4       4       4       restore bitmask
        lw      0       6       maxbit
        beq     4       6       done    halt when larger than 16bit
        nor     4       4       6       
        add     4       4       4       shift bitmask
        add     2       2       2       shift mscand
        beq     0       0       loop    back to loop
done    sw      0       1       1
        halt
mcand   .fill   32766
mplier  .fill   10383
bit     .fill   1
count   .fill   0
maxbit  .fill   65536
