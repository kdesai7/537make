# 537make

A reproduction of the basic functionality of make, including makefile parsing, dependency checking, and command execution. Done as an assignment for Introduction to Operating Systems.

## Files

* `in.txt` contains a sample valid makefile

* `blank.txt` contains several empty lines (no spaces)
* `spaces.txt` contains several lines that contain zero or more spaces

## Testing

Running `make test` automatically compiles and tests the code. The tests should output "SUCCESS" in green at the end. Manually inspect the output to make sure:
* Every file in the "FAILURES" section fails.


`basic_testcase`: A base test case which includes basic commands in the rule

`simple_testcase`: A test case with a valid, but exotic makefile.

`basic_cycle_testcase`: A testcase with a direct cycle in the graph.

`advanced_cycle_testcase`: A testcase with a more indirect cycle.

`linesize_testcase`: A invalid case of target line buffer overflow

`whitespace_testcase`: A special case of whitespaces in multilple lines

## Valgrind Errors

### Solved Errors

All changes are detailed in the "Start to fix Valgrind errors" commit, feel free to `git diff`, the repo is included in the handin directory.

`build_spec_repr.c:81`: Didn't call newNode, instead malloc'd which didn't set fields
`build_spec_repr.c:87`: Didn't call newNode, instead malloc'd which didn't set fields
`build_spec_repr.c:100`: Didn't call newNode, instead malloc'd which didn't set fields

All issues were solved with calling newNode(NULL) instead. In some cases, we realized we could remove error logging as that was handled in newNode. In one case (line 100), we realized we didn't check a malloc, so we fixed that real quick :).

### Ignored Errors

`valgrind.txt` contains remaining errors, as documented from 3 runs of valgrind:
* pid:4323, test directory: `whitespace_testcase`
* pid:4658, test directory: `basic_testcase`
* pid:5719, test directory: `simple_testcase`

The only errors remaining are "HEAP SUMMARY" errors, that is, memory leaks. But all of these are one-time allocations that can be freed once the program is complete, but are not explicitly freed because they will be implicitly freed on program exit.

They are described in detail below.

One-time allocation (names of targets in graph building)
```
==4323== 64 (16 direct, 48 indirect) bytes in 1 blocks are definitely lost in loss record 14 of 22
==4323==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4323==    by 0x10B086: newNode (node.c:40)
==4323==    by 0x109901: buildSpecGraph (build_spec_graph.c:154) # Line of interest
==4323==    by 0x109293: main (main.c:163)
```

One-time allocation (nodes for TargetInfo)
```
==4323== 64 (16 direct, 48 indirect) bytes in 1 blocks are definitely lost in loss record 15 of 22
==4323==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4323==    by 0x10B086: newNode (node.c:40)
==4323==    by 0x109917: buildSpecGraph (build_spec_graph.c:163) # Line of interest
==4323==    by 0x109293: main (main.c:163)
```

One-time allocation (tokenized command) once for each command
```
==4323== 64 bytes in 3 blocks are definitely lost in loss record 16 of 22
==4323==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4323==    by 0x10A2FB: tokenize (text_parsing.c:129) # Line of interest, the "tokenized line"
==4323==    by 0x10A9A1: parse (text_parsing.c:335)
==4323==    by 0x109199: main (main.c:134)
```

One-time allocation (graph representation of makefile)
```
==4323== 108 (24 direct, 84 indirect) bytes in 1 blocks are definitely lost in loss record 18 of 22
==4323==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4323==    by 0x109461: newGraph (build_spec_graph.c:40)
==4323==    by 0x109A30: buildSpecGraph (build_spec_graph.c:192)
==4323==    by 0x109293: main (main.c:163)
```

One-time allocation (struct that tracks partial makefile, at the end it tracks entire makefile)
```
==4323== 569 (16 direct, 553 indirect) bytes in 1 blocks are definitely lost in loss record 21 of 22
==4323==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4323==    by 0x10ACA6: newTargetInfoBuilder (build_spec_repr.c:96)
==4323==    by 0x10A69C: parse (text_parsing.c:243)
==4323==    by 0x109199: main (main.c:134)
```

One-time allocation (name of a given target) per target
```
==4323== 3,072 bytes in 3 blocks are definitely lost in loss record 22 of 22
==4323==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4323==    by 0x10ABDB: newTargetInfo (build_spec_repr.c:75)
==4323==    by 0x10AD0E: addNewTarget (build_spec_repr.c:119)
==4323==    by 0x10A192: processLine (text_parsing.c:64)
==4323==    by 0x10A9C7: parse (text_parsing.c:337)
==4323==    by 0x109199: main (main.c:134)
```

