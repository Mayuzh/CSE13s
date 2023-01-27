CFLAGS=-Wall -Wextra  -Werror  -pedantic  
CC=clang $(CFLAGS)
OBJECTS = sorting.o binary.o bubble.o quick.o shell.o
EXECBIN = sorting
.PHONY: all
all        :         $(EXECBIN)
$(EXECBIN) :         $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECBIN)
sorting.o  :         sorting.c
	$(CC) -c sorting.c
binary.o   :         binary.c
	$(CC) -c binary.c
bubble.o   :         bubble.c
	$(CC) -c bubble.c
quick.o    :         quick.c
	$(CC) -c quick.c
shell.o    :         shell.c
	$(CC) -c shell.c
.PHONY: clean
clean    :
	rm $(OBJECTS) $(EXECBIN)
infer    :
	make  clean; infer -capture  -- make; infer -analyze  -- make

