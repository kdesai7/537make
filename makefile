CC=gcc
WARNING_FLAGS=-Wall -Wextra
EXE=537make
# LIBS is placed at the end of gcc's linking stage (after all .o files) so it links the necessary library functions (like pthread) to your code
LIBS=-lpthread # if needed, add more libraries here

FILES = build_spec_graph.c \
		build_spec_graph.h \
		build_spec_repr.c \
		build_spec_repr.h \
		main.c \
		main.h \
		proc_creation_prog_exe.c \
		proc_creation_prog_exe.h \
		text_parsing.c \
		text_parsing.h
HEADER_INFO = student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
MARK_INFO = 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
JENNY_INFO = 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

# Text appearance (color)
# https://misc.flogisoft.com/bash/tip_colors_and_formatting
NORMAL = \e[0m
LIGHT_GREEN = \e[92m

# the -g flag at all gcc compilation stages ensures that you can use gdb to debug your code
$(EXE): main.o build_spec_graph.o text_parsing.o build_spec_repr.o proc_creation_prog_exe.o
	$(CC) -g -o $(EXE) main.o build_spec_graph.o text_parsing.o build_spec_repr.o proc_creation_prog_exe.o $(LIBS)

main.o: main.c build_spec_graph.h text_parsing.h build_spec_repr.h proc_creation_prog_exe.h
	$(CC) -g $(WARNING_FLAGS) -c main.c

build_spec_graph.o: build_spec_graph.c build_spec_graph.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_graph.c

text_parsing.o: text_parsing.c text_parsing.h build_spec_repr.h main.h
	$(CC) -g $(WARNING_FLAGS) -c text_parsing.c

build_spec_repr.o: build_spec_repr.c build_spec_repr.h main.h
	$(CC) -g $(WARNING_FLAGS) -c build_spec_repr.c

proc_creation_prog_exe.o: proc_creation_prog_exe.c proc_creation_prog_exe.h
	$(CC) -g $(WARNING_FLAGS) -c proc_creation_prog_exe.c

# the -f flag for rm ensures that clean doesn't fail if file to be deleted doesn't exist
clean:
	rm -f $(EXE) *.o

# recompile runs clean and then makes everything again to generate executable
# this is equivalent to running "make clean" followed by "make"
recompile: clean $(EXE)

# run tests
test: $(EXE)
	$(EXE)
	echo -e "$(LIGHT_GREEN)SUCCESS$(NORMAL)"

# add info to each file
info:
	for file in $(FILES) ; do \
		vim -c "1 s/^.*$$/\/\/ $(HEADER_INFO)" -c "wq" $$file ; \
		vim -c "2 s/^.*$$/\/\/ $(MARK_INFO)" -c "wq" $$file ; \
		vim -c "3 s/^.*$$/\/\/ $(JENNY_INFO)" -c "wq" $$file ; \
	done