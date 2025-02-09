    .section .text
    .global memset_optimized
memset_optimized:
    addi t1, a0, 0           # t1 = a0 (working pointer)
    beq  a2, x0, ret_label    # if (a2 == 0) return

    # Check pointer alignment: compute t0 = t1 mod 4
    andi t0, t1, 3           # t0 = a0 & 3
    beq  t0, x0, aligned_start  # if (t0 == 0) pointer is aligned

    # Compute how many bytes to reach the next 4-byte boundary:
    addi t3, x0, 4           # t3 = 4
    sub  t3, t3, t0          # t3 = 4 - (t1 mod 4)
    blt  a2, t3, byte_loop_all  # if (a2 < t3) fallback to byte-by-byte

    # Process misaligned prefix until t1 becomes 4-byte aligned:
misaligned_loop:
    sb   a1, 0(t1)           # store one byte
    addi t1, t1, 1           # increment pointer
    addi a2, a2, -1          # decrement total count
    addi t3, t3, -1          # decrement alignment counter
    bne  t3, x0, misaligned_loop

aligned_start:
    # Now t1 is 4-byte aligned.
    addi t4, x0, 4           # t4 = 4 (for comparison)
    blt  a2, t4, tail_loop   # if remaining count < 4, go to tail loop

    # Build a 32-bit word in t2 with all four bytes equal to a1.
    # Instead of slli, load the shift immediate into a register and use sll.
    addi t4, x0, 24          # t4 = 24
    sll  t2, a1, t4          # t2 = a1 << 24

    addi t5, x0, 16          # t5 = 16
    sll  t3, a1, t5          # t3 = a1 << 16
    or   t2, t2, t3          # t2 |= (a1 << 16)

    addi t4, x0, 8           # t4 = 8 (reuse t4)
    sll  t3, a1, t4          # t3 = a1 << 8
    or   t2, t2, t3          # t2 |= (a1 << 8)

    or   t2, t2, a1          # t2 |= a1

    # Compute the number of 32-bit words to store: t3 = a2 >> 2.
    addi t4, x0, 2           # t4 = 2
    srl  t3, a2, t4          # t3 = a2 >> 2

word_loop:
    sw   t2, 0(t1)           # store word (4 bytes)
    addi t1, t1, 4           # increment pointer by 4
    addi t3, t3, -1          # decrement word count
    bne  t3, x0, word_loop   # loop if word count != 0

    # Process any remaining trailing bytes.
    andi a2, a2, 3           # a2 = a2 mod 4

tail_loop:
    beq  a2, x0, ret_label   # if no trailing bytes, return
byte_tail_loop:
    sb   a1, 0(t1)           # store one trailing byte
    addi t1, t1, 1           # increment pointer
    addi a2, a2, -1          # decrement count
    bne  a2, x0, byte_tail_loop
    jal  x0, ret_label       # jump to return

byte_loop_all:
    beq  a2, x0, ret_label   # if count == 0, return
byte_loop_all_loop:
    sb   a1, 0(t1)
    addi t1, t1, 1
    addi a2, a2, -1
    bne  a2, x0, byte_loop_all_loop

ret_label:
    jalr x0, ra, 0           # return


