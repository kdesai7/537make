CC=gcc
WARNING_FLAGS=-Wall -Wextra
EXE=537make
# LIBS is placed at the end of gcc's linking stage (after all .o files) so it links the necessary library functions (like pthread) to your code
LIBS=-lpthread # if needed, add more libraries here

# the -g flag at all gcc compilation stages ensures that you can use gdb to debug your code
$(EXE): main.o build_spec_graph.o text_parsing.o build_spec_repr.o proc_creation_prog_exe.o
	$(CC) -g -o $(EXE) main.o build_spec_graph.o text_parsing.o build_spec_repr.o proc_creation_prog_exe.o $(LIBS)

main.o: main.c build_spec_graph.h text_parsing.h build_spec_repr.h proc_creation_prog_exe.h
	$(CC) -g $(WARNING_FLAGS) -c main.c

build_spec_graph.o: build_spec_graph.c build_spec_graph.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_graph.c

text_parsing.o: text_parsing.c text_parsing.h
	$(CC) -g $(WARNING_FLAGS) -c text_parsing.c

build_spec_repr.o: build_spec_repr.c build_spec_repr.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_repr.c

proc_creation_prog_exe.o: proc_creation_prog_exe.c proc_creation_prog_exe.h
	$(CC) -g $(WARNING_FLAGS) -c proc_creation_prog_exe.c

# the -f flag for rm ensures that clean doesn't fail if file to be deleted doesn't exist
clean:
	rm -f $(EXE) *.o

# recompile runs clean and then makes everything again to generate executable
# this is equivalent to running "make clean" followed by "make"
recompile: clean $(EXE)
