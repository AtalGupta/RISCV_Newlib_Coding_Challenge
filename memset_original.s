    .section .text
    .global memset_original
memset_original:
    addi t1, a0, 0           # t1 = a0 (copy destination pointer)
    beq  a2, x0, ret_label    # if (a2 == 0) return

1:
    sb   a1, 0(t1)           # store byte (a1) at address in t1
    addi a2, a2, -1          # decrement count in a2
    addi t1, t1, 1           # increment pointer
    bne  a2, x0, 1b          # loop if a2 != 0

ret_label:
    jalr x0, ra, 0           # return (jr ra)

