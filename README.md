# 537make

A reproduction of the basic functionality of make, including makefile parsing, dependency checking, and command execution.

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

All changes are detailed in the "Start to fix Valgrind errors" commit, feel free to `git diff`, the repo is included in the handin directory.

`build_spec_repr.c:81`: Didn't call newNode, instead malloc'd which didn't set fields
`build_spec_repr.c:87`: Didn't call newNode, instead malloc'd which didn't set fields
`build_spec_repr.c:100`: Didn't call newNode, instead malloc'd which didn't set fields

All issues were solved with calling newNode(NULL) instead. In some cases, we realized we could remove error logging as that was handled in newNode. In one case (line 100), we realized we didn't check a malloc, so we fixed that real quick :).

