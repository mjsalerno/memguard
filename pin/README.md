Getting Pin
=========

- Download pin from [here](http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool#GettingStarted)

Building Pin Tools
===============

1. Navagate to cd source/tools/ManualExamples and make all

```bash
$ cd source/tools/ManualExamples
```

2. Build the tools 

   -  make all of the tools
```bash       
$ make all
```

- or make just one tool

```bash
$ make malloctrace.test
```

Using Pin
========
```bash
$ ../../../pin -t  obj-intel64/<pin-tool>.so  -- <prgm-to-test>
```
example

```bash
$ ../../../pin -t obj-intel64/malloctrace.so -- ~/Desktop/test
```

Using MyPinTool
===============

1. Make and test the tool.

```bash
$ ./make.sh
```

2. The results will be in the obj-intel64 folder.

Or run it manually

1. make

```bash
$ make PIN_ROOT=pin
```

2. test

```bash
$ make PIN_ROOT=pin test
```

3. clean

```bash
$ make PIN_ROOT=pin clean
```