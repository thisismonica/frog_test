# frog_test
State-of-the-art automatic testing framework

# Frog Testing Script/ Tools/ Original Source Code files
- frog_pointer_support.py: main python for integration of KLEE and Tarantula, creating automatic, unassisted testing framework.

- support_frog.py: support functions of Frog, mainly Tarantula implementations

- tools/ : Folder of KLEE Symbolic Execution Enginee

- *.c: Original testing file
- *.test.c: Instrumented testing file with symbolization command
- *.replay.c: Modified testing file for replaying of test cases