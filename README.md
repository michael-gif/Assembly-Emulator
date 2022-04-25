# Python Assembly Language Interpreter
An interpreter that will read and execute an asm file from the terminal

# Usage
- write your assembly in a file ending with .asm
- run `python main.py -f <filename>.asm` in a terminal, giving the asm file you wrote as `<filename>`

# Instruction set
| Instruction         | Description |
| ------------------- | ----------- |
| `mov <reg1> <reg2>` | moves contents of reg1 to reg2 |
| `add <reg1> <reg2>` | adds reg1 to reg2 storing the result in reg2 |
| `sub <reg1> <reg2>` | subtracts reg1 from reg2 storing the result in reg2 |
| `inc <reg>`         | increments reg by 1 |
| `dec <reg>`         | decrements reg by 1 |
| `imul <reg1> <reg2>`| multiplies reg1 by reg2 storing the result in reg2 |
| `idiv <reg1> <reg2>`| divides reg1 by reg2 storing the result in reg2 |
| `prnt <reg>`        | prints to stdout |
| `prnt <int>`        | prints to stdout |
| `hlt <int>`         | waits the specified time (can be a float) |
| `hlt <reg>`         | waits the specified time |

# Example
The asm file:
```
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
```
- Saved as "fibonacci.asm"
- Running the command `python main.py -f fibonacci.asm`:  
![Fibonacci output](https://github.com/michael-gif/assembly-interpreter/blob/main/stuff/output.png)
