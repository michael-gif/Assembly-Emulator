# C++ Assembly Emulator
An emulator that will read and execute an asm file from the terminal

# Usage
- Run `asmc.exe <filename.asm>` in a terminal. You must include the `.asm` extension

# Suported instruction set
| Instruction         | Description |
| ------------------- | ----------- |
| `mov <reg1> <reg2>` | moves contents of reg2 to reg1 |
| `inc <reg>`         | increments reg by 1 |
| `dec <reg>`         | decrements reg by 1 |
| `add <reg1> <reg2>` | adds reg2 to reg1 storing the result in reg1 |
| `sub <reg1> <reg2>` | subtracts reg2 from reg1 storing the result in reg1 |
| `mul <reg1> <reg2>` | multiplies reg2 by reg1 storing the result in reg1 |
| `div <reg1> <reg2>` | divides reg1 by reg2 storing the result in reg1 |
| `prnt <reg or int>` | prints to stdout |
| `hlt <reg or int>`  | waits the specified time in ms |
| `cmp <reg1> <reg2>` | compares both registers for equality, stores result in read only comparison result register |
| `jmp <label>`       | jump to label |
| `je <label>`        | jump to label if the comparison result is 1 |
| `jne <label>`       | jump to label if the comparison result is 0 |

# Registers
There are 6 registers available: `a, b, c, d, e, f`. Each register holds an `int`.

# Examples
### First 10 numbers of fibonacci [`fibonacci.asm`](https://github.com/michael-gif/assembly-interpreter/blob/main/resources/fibonacci.asm):
```
mov c 1
mov d 0
mov e 10
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
```
Output  
![Output](https://github.com/michael-gif/assembly-interpreter/blob/main/resources/fibonacci_output.png)

### Comparison and jump not equal example [`comparison.asm`](https://github.com/michael-gif/assembly-interpreter/blob/main/resources/comparison.asm):
```
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
```
Output  
![Output](https://github.com/michael-gif/assembly-interpreter/blob/main/resources/jne_output.png)