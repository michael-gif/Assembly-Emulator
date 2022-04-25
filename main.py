from argparse import ArgumentParser
from time import sleep

parser = ArgumentParser()
parser.add_argument("-f", "--file", dest="filename",
                    help="specifies the asm file to interpret", metavar="FILE")
args = vars(parser.parse_args())
filename = args["filename"]
if filename is None:
    quit()

registers = {
    "a": 0,
    "b": 0,
    "c": 0,
    "d": 0,
    "e": 0,
    "f": 0,
    "g": 0,
    "h": 0
}


class InvalidRegister(Exception):
    '''
    Custom exception class for when the program can't find the register specified in the assembly
    '''
    def __init__(self, register):
        super().__init__("")
        self.register = register


def ThrowsException(func):
    '''
    Decorator that applies a wrapper which will hand InvalidRegister and ValueError exceptions
    :param func:
    :return:
    '''
    def wrapper(args):
        try:
            func(args)
        except Exception as e:
            if type(e) == InvalidRegister:
                print(f"cannot find register {e.register}")
            if type(e) == ValueError:
                value = str(e).strip("'")
                print(f'cannot convert {value} to int')

    return wrapper


def is_int(s):
    '''
    Checks if a number is an integer without using a try/except block
    :param s:
    :return:
    '''
    if s[0] in ('-', '+'):
        return s[1:].isdigit()
    return s.isdigit()


def is_float(num):
    '''
    Checks if a number is a float without using a try/except block
    Used only for the halt command so you can easily specify a delay in an asm program
    :param num:
    :return:
    '''
    try:
        float(num)
        return True
    except ValueError:
        return False


@ThrowsException
def mov(args):
    if args[1] not in registers:
        raise InvalidRegister(args[1])
    if is_int(args[0]):
        registers[args[1]] = int(args[0])
    else:
        if args[0] not in registers:
            raise InvalidRegister(args[0])
        registers[args[1]] = registers[args[0]]


@ThrowsException
def add(args):
    if args[0] not in registers:
        raise InvalidRegister(args[0])
    if args[1] not in registers:
        raise InvalidRegister(args[1])
    registers[args[1]] += registers[args[0]]


@ThrowsException
def sub(args):
    if args[0] not in registers:
        raise InvalidRegister(args[0])
    if args[1] not in registers:
        raise InvalidRegister(args[1])
    registers[args[1]] -= registers[args[0]]


@ThrowsException
def inc(args):
    if args[0] not in registers:
        raise InvalidRegister(args[0])
    registers[args[0]] += 1


@ThrowsException
def dec(args):
    if args[0] not in registers:
        raise InvalidRegister(args[0])
    registers[args[0]] -= 1


@ThrowsException
def imul(args):
    if args[0] not in registers:
        raise InvalidRegister(args[0])
    if args[1] not in registers:
        raise InvalidRegister(args[1])
    registers[args[1]] *= registers[args[0]]


@ThrowsException
def idiv(args):
    if args[0] not in registers:
        raise InvalidRegister(args[0])
    if args[1] not in registers:
        raise InvalidRegister(args[1])
    registers[args[1]] /= registers[args[0]]


@ThrowsException
def jmp(args):
    global instruction_counter
    try:
        instruction_counter = loops[args[0]]
    except KeyError:
        print(f"cannot find label {args[0]}")


@ThrowsException
def hlt(args):
    global halt_value
    if is_float(args[0]):
        halt_value = float(args[0])
    else:
        if args[0] not in registers:
            raise InvalidRegister(args[0])
        halt_value = registers[args[0]]


@ThrowsException
def prnt(args):
    if is_int(args[0]):
        print(int(args[0]))
    else:
        if args[0] not in registers:
            raise InvalidRegister(args[0])
        print(registers[args[0]])


commands = {
    "mov": mov,
    "add": add,
    "sub": sub,
    "inc": inc,
    "dec": dec,
    "imul": imul,
    "idiv": idiv,
    "jmp": jmp,
    "hlt": hlt,
    "prnt": prnt
}

with open(filename) as f:
    lines = [line.strip('\n').strip(" ") for line in f.readlines()]

loops = {}
for i in range(len(lines)):
    keyword = lines[i].split(" ")[0]
    if keyword.endswith(":"):
        loops[keyword[0:-1]] = i

halt_value = 0
instruction_counter = 0
while instruction_counter < len(lines):
    parts = lines[instruction_counter].split(" ")
    keyword, arguments = parts[0], parts[1:]
    if keyword.endswith(":"):
        instruction_counter += 1
        continue
    if keyword in commands:
        commands[keyword](arguments)
    if halt_value:
        sleep(halt_value)
        halt_value = 0
    instruction_counter += 1
