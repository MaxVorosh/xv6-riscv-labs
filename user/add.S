.data                                      # Data section

msg_not_numb: .asciz "Not a number\n"
msg_empty_str: .asciz "Empty input\n"
msg_one_numb: .asciz "Only one number given\n"
msg_buf_over: .asciz "Buffer overflow\n"

msg_fmt: .asciz "%d\n"

input: .space BUF_SIZE

#define ONE_NUMB_LEN 22
#define NOT_NUMB_LEN 13
#define EMPTY_STR_LEN 12
#define BUF_OVER_LEN 16
#define FMT_LEN 3
#define BUF_SIZE 23

.text                                      # Text (code) section
.global main                               # Exportable object (label)

main:
    la a0, input
    li a1, BUF_SIZE
    call gets

    la t0, input
    li t1, BUF_SIZE
    lbu t2, '\n'
    beq 0(input), t2, empty_fail
    lbu t2, ' '
    beq 0(input), ' ', empty_fail
    lbu t2, '\0'
    beq 0(input), '\0', empty_fail

find_space:
    lbu t3, 0(t0)
    lbu t2, '0'
    blt t3, t2, not_numb_fail
    lbu t2, '9'
    blt t3, t2, not_numb_fail
    lbu t2, '\0'
    beq t3, t2, one_numb_fail
    lbu t2, '\n'
    beq t3, t2, one_numb_fail
    lbu t2, ' '
    beq t3, t2, process_second
    addi t0, 1
    addi t1, -1
    beq zero, t1, buf_over_fail
    j find_space

process_second:
    addi t0, 1
    addi t1, -1
    mv t4, t0

check_second:
    beq zero, t1, buf_over_fail
    lbu t3, 0(t4)
    lbu t2, '0'
    blt t3, t2, not_numb_fail
    lbu t2, '9'
    blt t3, t2, not_numb_fail
    lbu t2, '\0'
    beq t3, t2, do_sum
    lbu t2, '\n'
    beq t3, t2, do_sum
    lbu t2, ' '
    beq t3, t2, do_sum
    addi t4, 1
    addi t1, -1
    j check_second

do_sum:
    la a0, input
    call atoi
    mv t1, a0
    
    mv a0, t0
    call atoi
    
    add t0, t1, a0
    la a0, msg_fmt
    mv a1, t0
    call printf

    li a7, 2
    li a0, 0
    ecall

empty_fail:
    li a7, 16
    li a0, 2
    la a1, msg_empty_str
    li a2, EMPTY_STR_LEN
    ecall

    li a7, 2
    li a0, 5
    ecall

one_numb_fail:
    li a7, 16
    li a0, 2
    la a1, msg_one_numb
    li a2, ONE_NUMB_LEN
    ecall

    li a7, 2
    li a0, 1
    ecall

not_numb_fail:
    li a7, 16
    li a0, 2
    la a1, msg_not_numb
    li a2, NOT_NUMB_LEN
    ecall

    li a7, 2
    li a0, 3
    ecall

buf_over_fail:
    li a7, 16
    li a0, 2
    la a1, msg_buf_over
    li a2, BUF_OVER_LEN
    ecall

    li a7, 2
    li a0, 2
    ecall
