# Python Assembly Language Interpreter
An interpreter that will read and execute an asm file from the terminal

# Usage
- write your assembly in a file ending with .asm
- run main.py in a terminal, giving the asm file you wrote as an argument

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
- Running the command "python main.py -f fibonacci.asm":
![Fibonacci output](https://github.com/michael-gif/assembly-interpreter/blob/main/output.png)
