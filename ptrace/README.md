# Function hooking via ptrace
Description: This directory contains some sample code for running ptrace
## Settings up the project
### Pre-reqs
1. Linux OS
2. gcc is installed
3. make installed
  using ubuntu you can install gcc using sudo apt-get install gcc or on fedora sudo yum install gcc
### Instructions
Once all the prereqs are aquired just run 
```bash
make all
```
This script will build `sample`, `sample2` and the executable `hw`

Trying running
`sample2 sample` or `sample hw`

It should print out the number of instructions ran and the value of RIP(x64) (or EIP(x86))

```
paul@paul-UX32VD:~/Git/cse409/ptrace$ ./sample2 hw
[14020] debugger started
[14021] target started. will run 'hw'
[14020] icounter = 1.  EIP = 0x00400078.  instr = 0x01c0c748
[14020] icounter = 2.  EIP = 0x0040007f.  instr = 0x01c7c748
[14020] icounter = 3.  EIP = 0x00400086.  instr = 0xa2c6c748
[14020] icounter = 4.  EIP = 0x0040008d.  instr = 0x0dc2c748
[14020] icounter = 5.  EIP = 0x00400094.  instr = 0xc748050f
Hello, World
[14020] icounter = 6.  EIP = 0x00400096.  instr = 0x3cc0c748
[14020] icounter = 7.  EIP = 0x0040009d.  instr = 0x0fff3148
[14020] icounter = 8.  EIP = 0x004000a0.  instr = 0x6548050f
[14020] the child executed 8 instructions
```

## Sources and References
1. [How debuggers work: Part 1 – Basics](http://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1/)