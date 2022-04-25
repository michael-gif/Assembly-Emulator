mov 1 b
mov b c
mov b d
loop:
    add c d
    mov b c
    mov b a
    mov d b
    prnt d
    hlt .25
jmp loop