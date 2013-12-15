#Getting Pin


- Download pin from [here](http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool#GettingStarted)

#Building Pin Tools


- Navagate to cd source/tools/ManualExamples and make all

```bash
$ cd source/tools/ManualExamples
```

- Build the tools 

   -  make all of the tools
```bash       
$ make all
```

- or make just one tool

```bash
$ make malloctrace.test
```

#Using Pin

```bash
$ ../../../pin -t  obj-intel64/<pin-tool>.so  -- <prgm-to-test>
```
example

```bash
$ ../../../pin -t obj-intel64/malloctrace.so -- ~/Desktop/test
```
---
#Using MyPinTool


- Make and test the tool.

```bash
$ ./make.sh
```

- The results will be in the obj-intel64 folder.

Or run it manually

- make

```bash
$ make PIN_ROOT=pin
```

- test

```bash
$ make PIN_ROOT=pin test
```

- clean

```bash
$ make PIN_ROOT=pin clean
```

# Errors
An error may occur when you run pin:
```bash
Killed
paul@paul-UX32VD:~/Git/cse409/pin/MyPinTool$ ./pin/pin -t obj-intel64/mallocmem.so -- test
Killed
E:Attach to pid 7725 failed. 
E:  The Operating System configuration prevents Pin from using the default (parent) injection mode.
E:  To resolve this, either execute the following (as root):
E:  $ echo 0 > /proc/sys/kernel/yama/ptrace_scope
E:  Or use the "-injection child" option.
E:  For more information, regarding child injection, see Injection section in the Pin User Manual.
E:
```

If this happens do:

1. sudo su
2. echo 0 > /proc/sys/kernel/yama/ptrace_scope
3. exit

Run the command again

# Sources
1. [Detecting uninitialized memory read access bugs using pin](http://jbremer.org/detecting-uninitialized-memory-read-access-bugs-using-pin-a-la-valgrind/)
2. [Pintool tutorials](http://shell-storm.org/repo/Notepad/more-Pin-stuff-references.txt)
3. [Pinheads Discussion Group](http://groups.yahoo.com/neo/groups/pinheads/info)