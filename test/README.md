# OS Project 2: Synchronization

An exercise in implementation of monitors in C.

Synchronization takes in lines of input from stdin in one thread (reader), and transforms them in two others. The first transformation (munch1) replaces all spaces with asterisks (*), and the second transformation (munch2) replaces all lowercase characters with their uppercase counterparts. A fourth thread (writer) prints out the transformed strings to stdout.

When reader encounters EOF, it signals for the other threads that no more input is incoming. The threads then gracefully terminate, and writer prints out the number of strings that were processed. The main thread prints out stats about the queues that were used for the transformation (number of times enqueued, dequeued, blocked).

If a line is too long for the constant-sized buffer, an error message is printed to stderr, the line is thrown out, and input continues.
