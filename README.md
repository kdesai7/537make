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
