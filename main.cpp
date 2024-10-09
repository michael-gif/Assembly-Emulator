#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <chrono>
#include <thread>


/// <summary>
/// Each label maps to an instruction index
/// </summary>
std::map<std::string, int> labels;

/// <summary>
/// Predefined registers with integer values
/// </summary>
std::map<std::string, int> registers = {
    {"a", 0},
    {"b", 0},
    {"c", 0},
    {"d", 0},
    {"e", 0}
};

int compareResult = 0;

struct ASMCommand {
    const char* name = "";
    std::string arg1, arg2;
    ASMCommand() {}
    ASMCommand(const char* _name) : name(_name) {}
    ASMCommand(const char* _name, std::string _arg1, std::string _arg2) : name(_name), arg1(_arg1), arg2(_arg2) {}
    virtual int execute() {
        return -1;
    };
    bool isRegister(std::string& name) {
        return registers.contains(name);
    }
};

struct MoveCommand : ASMCommand {
    MoveCommand(std::string& _arg1, std::string& _arg2) : ASMCommand("Move", _arg1, _arg2) {}

    /// <summary>
    /// Move arg2 into arg1 reg. Arg2 can be a register or value.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        if (isRegister(arg2)) {
            registers[arg1] = registers[arg2];
        }
        else {
            registers[arg1] = std::stoi(arg2);
        }
        return -1;
    }
};

struct IncrementCommand : ASMCommand {
    IncrementCommand(std::string& _arg1) : ASMCommand("Increment", _arg1, "") {}

    /// <summary>
    /// Increment register
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        registers[arg1]++;
        return -1;
    }
};

struct DecrementCommand : ASMCommand {
    DecrementCommand(std::string& _arg1) : ASMCommand("Decrement", _arg1, "") {}

    /// <summary>
    /// Decrement register
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        registers[arg1]--;
        return -1;
    }
};

struct AddCommand : ASMCommand {
    AddCommand(std::string& _arg1, std::string& _arg2) : ASMCommand("Add", _arg1, _arg2) {}

    /// <summary>
    /// Add second reg to first and store the result in the first.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        if (!isRegister(arg2)) {
            std::cout << "Error: " << arg2 << " is not a register" << std::endl;
            return -1;
        }
        registers[arg1] += registers[arg2];
        return -1;
    }
};

struct SubCommand : ASMCommand {
    SubCommand(std::string& _arg1, std::string& _arg2) : ASMCommand("Subtract", _arg1, _arg2) {}

    /// <summary>
    /// Subtract second reg from first and store the result in the first.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        if (!isRegister(arg2)) {
            std::cout << "Error: " << arg2 << " is not a register" << std::endl;
            return -1;
        }
        registers[arg1] -= registers[arg2];
        return -1;
    }
};

struct MulCommand : ASMCommand {
    MulCommand(std::string& _arg1, std::string& _arg2) : ASMCommand("Multiply", _arg1, _arg2) {}

    /// <summary>
    /// Multiply first and second reg, store result in first.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        if (!isRegister(arg2)) {
            std::cout << "Error: " << arg2 << " is not a register" << std::endl;
            return -1;
        }
        registers[arg1] *= registers[arg2];
        return -1;
    }
};

struct DivCommand : ASMCommand {
    DivCommand(std::string& _arg1, std::string& _arg2) : ASMCommand("Divide", _arg1, _arg2) {}

    /// <summary>
    /// Divide reg2 by reg1, store result in reg1.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        if (!isRegister(arg2)) {
            std::cout << "Error: " << arg2 << " is not a register" << std::endl;
            return -1;
        }
        registers[arg1] /= registers[arg2];
        return -1;
    }
};

struct PrintCommand : ASMCommand {
    PrintCommand(std::string& _arg1) : ASMCommand("Print", _arg1, "") {}

    /// <summary>
    /// Print arg1 reg to stdout. Arg1 can be a register or value.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (isRegister(arg1)) {
            std::cout << registers[arg1] << std::endl;
        }
        else {
            std::cout << arg1 << std::endl;
        }
        return -1;
    }
};

struct HaltCommand : ASMCommand {
    HaltCommand(std::string& _arg1) : ASMCommand("Halt", _arg1, "") {}

    /// <summary>
    /// Sleep for arg1 milliseconds. Arg1 can be a register or value.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (isRegister(arg1)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(registers[arg1]));   
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(std::stoi(arg1)));
        }
        return -1;
    }
};

struct CompareCommand : ASMCommand {
    CompareCommand(std::string& _arg1, std::string& _arg2) : ASMCommand("Compare", _arg1, _arg2) {}

    /// <summary>
    /// Compare both args for equality. Args must be registers.
    /// </summary>
    /// <returns></returns>
    int execute() override {
        if (!isRegister(arg1)) {
            std::cout << "Error: " << arg1 << " is not a register" << std::endl;
            return -1;
        }
        if (!isRegister(arg2)) {
            std::cout << "Error: " << arg2 << " is not a register" << std::endl;
            return -1;
        }
        compareResult = registers[arg1] == registers[arg2];
        return -1;
    }
};

struct JumpCommand : ASMCommand {
    std::string labelName;
    JumpCommand(std::string& _labelName) : ASMCommand("Jump") {
        labelName = _labelName;
    }

    /// <summary>
    /// Jumps to label
    /// </summary>
    /// <returns></returns>
    int execute() override {
        return labels[labelName];
    }
};

struct JumpEqualCommand : ASMCommand {
    JumpEqualCommand(std::string& _arg1) : ASMCommand("JumpEqual", _arg1, "") {}

    /// <summary>
    /// Jump to label if compareResult is 1
    /// </summary>
    /// <returns></returns>
    int execute() override {
        return compareResult ? labels[arg1] : -1;
    }
};

struct JumpNotEqualCommand : ASMCommand {
    JumpNotEqualCommand(std::string& _arg1) : ASMCommand("JumpNotEqual", _arg1, "") {}

    /// <summary>
    /// Jump to label if compareResult is 0
    /// </summary>
    /// <returns></returns>
    int execute() override {
        return compareResult ? -1 : labels[arg1];
    }
};

/// <summary>
/// Return an ASMCommand instance based on the command name
/// </summary>
/// <param name="commandChunks"></param>
/// <param name="commandIndex"></param>
/// <returns></returns>
ASMCommand* commandFactory(std::vector<std::string>& commandChunks, int commandIndex) {
    std::string key = commandChunks[0];
    if (key.back() == ':') {
        key.pop_back();
        labels[key] = commandIndex; // create new label
        return new ASMCommand(); // insert blank command to avoid throwing off the label indices
    }
    if (key == "mov") return new MoveCommand(commandChunks[1], commandChunks[2]);
    if (key == "inc") return new IncrementCommand(commandChunks[1]);
    if (key == "dec") return new DecrementCommand(commandChunks[1]);
    if (key == "add") return new AddCommand(commandChunks[1], commandChunks[2]);
    if (key == "sub") return new SubCommand(commandChunks[1], commandChunks[2]);
    if (key == "mul") return new MulCommand(commandChunks[1], commandChunks[2]);
    if (key == "div") return new DivCommand(commandChunks[1], commandChunks[2]);
    if (key == "prnt") return new PrintCommand(commandChunks[1]);
    if (key == "hlt") return new HaltCommand(commandChunks[1]);
    if (key == "cmp") return new CompareCommand(commandChunks[1], commandChunks[2]);
    if (key == "jmp") return new JumpCommand(commandChunks[1]);
    if (key == "jne") return new JumpNotEqualCommand(commandChunks[1]);
    if (key == "je") return new JumpEqualCommand(commandChunks[1]);
}

/// <summary>
/// Read an asm file and put each line into a vector of strings
/// </summary>
/// <param name="filename"></param>
/// <param name="asmLines"></param>
void readASMFile(std::string& filename, std::vector<std::string>& asmLines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue; // remove empty lines
        asmLines.push_back(line);
    }
    file.close();
}

/// <summary>
/// Parse a vector of string instructions, creating an ASMCommand instance for each instruction.
/// </summary>
/// <param name="asmLines"></param>
/// <param name="commands"></param>
void parseASM(std::vector<std::string>& asmLines, std::vector<ASMCommand*>& commands) {
    for (int i = 0; i < asmLines.size(); i++) {
        std::istringstream iss(asmLines[i]);
        std::string word;
        std::vector<std::string> chunks;
        while (iss >> word) chunks.emplace_back(word); // Store each word in the vector#
        ASMCommand* cmd = commandFactory(chunks, i);
        commands.emplace_back(cmd);
    }
}

/// <summary>
/// Loop through the vector of commands and execute each one.
/// A commands return value dictactes the next instruction to jump to.
/// </summary>
/// <param name="commands"></param>
void executeASM(std::vector<ASMCommand*>& commands) {
    for (int instructionIndex = 0; instructionIndex < commands.size(); instructionIndex++) {
        ASMCommand* cmd = commands[instructionIndex];
        int nextIndex = commands[instructionIndex]->execute();
        if (nextIndex != -1)
            instructionIndex = nextIndex - 1; // nextIndex - 1, because next iteration adds one, so cancel that out with the -1.
    }
}

/// <summary>
/// Deallocates ASM commands
/// </summary>
/// <param name="commands"></param>
void cleanupASM(std::vector<ASMCommand*>& commands) {
    for (int i = 0; i < commands.size(); i++) delete commands[i];
}


int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cout << "Please supply an asm file as an argument" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<std::string> asmLines;
    printf("Reading ASM file...\n");
    readASMFile(filename, asmLines);
    std::vector<ASMCommand*> commands;
    printf("Compiling...\n");
    parseASM(asmLines, commands);
    printf("Running...\n");
    executeASM(commands);
    printf("Program finished.\n");
    cleanupASM(commands);
    return 0;
}
