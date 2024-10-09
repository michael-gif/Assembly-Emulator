mov a 9
mov b 10

cmp b a
je l1
jne l2

l1:
prnt equal
jmp end

l2:
prnt not_equal
jmp end

end: