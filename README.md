#MAI

Team members:
Kevin Fisher
Nathaniel Warren III
Edward Prins III
Miguel Betances Lee
Dereje Arega

Research Problem


For our research project, we have decided to write a MIPS interpreter in C.

Similar to the Python interpreter, it will allow a user to run it and enter MIPS assembly instructions line by line, displaying which registers/values change as a result of each line. We believe this would be very useful for students learning about assembly language, because it would allow them to test fragments of programs easily and quickly, rather than having to create a full MIPS program in an IDE/simulator like MARS.

In addition to the live interpreter mode, users will be able to load and run a MIPS file, just like in MARS. A small goal that we have is running MIPS programs faster than MARS can. We believe this will be easy given that MARS has a huge overhead (the Java runtime). Our program will be a faster C program with no such limitation.

We plan to implement pseudo-opcodes like in MIPS (such as mov and li).

Technical Approach


MAI will be implemented in C. We will use only the standard C library, meaning it will run on all platforms without modification.

MAI will have two modes of operation -- live interpreter and input file mode. In live interpreter mode, users enter assembly instructions on a line by line basis. In input file mode, MAI will read a file and run it, similar to MARS.

MAI differs from MARS in that it is not an assembler. While MARS translates the assembly code into machine code and simulates that, MAI will instead parse the text and execute C functions based on that.

We will create a small C function for every emulated MIPS instruction. Each of these functions will take two parameters, a struct containing the current MIPS state, and a string for parsing the parameters of the MIPS instruction.

It will contain a dictionary structure that resolves a string representation of an instruction (such as add) into a corresponding C function. Since MIPS is a reduced instruction set architecture, it will not be hard to implement all instructions, as opposed to something like x86.

The basic loop for the interpreter mode will be as follows:
Read a line from console
Resolve the opcode name (add, sub, etc…) into a corresponding C function
Call the function with the string containing the instruction operands
The function will parse the substring, using another standard function to translate register names into int pointers, and act upon those.
Any user input error will simply write a description of the problem to console and have no other effect
Display any changed registers’ values
Repeat

The file input mode will be very similar, except that it reads each line from the file in and substitutes that for user input.

For testing opcode emulation correctness, we will run small portions of MIPS assembly in our interpreter and verify the resulting MIPS register state is as we expect. We can also compare results against running the same programs in MARS.

For testing speed, we will time programs that run in MARS and in MAI.

We will use github to collaborate at: https://github.com/megabytefisher/MAI

Milestones


A basic system for reading user input and parsing strings from MIPS opcodes into our own C emulation functions
Implementation of a majority of the standard MIPS instruction set.
Ability to input .data sections (like .data text : word 5)
Implementation of MARS pseudo-instructions
Ability to read MIPS programs from file and run them.
Optimization/Speed ups

References


Since we are only using the standard C library, our main reference will be websites that describe the C language and stdlib. Such as cplusplus.com

Another reference we will need is descriptions of the MIPS instruction set and syscalls, such as:
http://courses.missouristate.edu/kenvollmar/mars/help/syscallhelp.html
http://www.mrc.uidaho.edu/mrc/people/jff/digital/MIPSir.html

Projected Example



Welcome to the MIPS Live Interpreter
Enter an opcode to execute:
>> addi $a0, $zero, 5
Modified registers:
$a0 : 5
>> add $a0, $a0, $a0
Modified registers:
$a0 : 10
>> li $v0, 1
Modified registers:
$v0 : 1
>> syscall
10
>> .data
testValue: .word 15
.text
>> li $v0, 10
Modified registers:
$v0 : 10
>> syscall
(exits program)




