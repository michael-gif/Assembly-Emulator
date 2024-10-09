mov c 1
mov d 0
mov e 9
prnt 0
loop:
    add c b
    mov b a
    mov a c
    prnt c
    hlt 250
    inc d
    cmp e d
    je end
jmp loop
end: