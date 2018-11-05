# Mark Wiemer mww 9074356420
# Jenny Ye haengjung 9075878315

all: main.o munch.o queue.o reader.o writer.o
	gcc -o prodcomm main.o munch.o queue.o reader.o writer.o -lpthread

main.o: main.c main.h munch.h queue.h writer.h
	gcc -Wall -Wextra main.c

munch.o: munch.c main.h munch.h queue.h
	gcc -Wall -Wextra -c munch.c

queue.o: queue.c main.h queue.h
	gcc -Wall -Wextra -c queue.c

reader.o: reader.c main.h
	gcc -Wall -Wextra -c reader.c

writer.o: writer.c main.h queue.h
	gcc -Wall -Wextra -c writer.c

clean:
	rm -f procdomm out.txt *.o
	rm -rf ./scan-build

scan-build: clean
	scan-build -o ./scan-build make
