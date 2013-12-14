Getting Pin
=========

- Download pin from [here](http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool#GettingStarted)

Building Pin Tools
===============

1. Navagate to cd source/tools/ManualExamples and make all

```
$ cd source/tools/ManualExamples
```

2. Build the tools 

   -  make all of the tools
```         
$ make all
```

- or make just one tool

```
$ make malloctrace.test
```

Using Pin
========
```
$ ../../../pin -t  obj-intel64/<pin-tool>.so  -- <prgm-to-test>
```
e.g.
 ```
$ ../../../pin -t obj-intel64/malloctrace.so -- ~/Desktop/test
```

   		