CFLAGS=-Wall -Wextra  -Werror  -Wpedantic  
CC=clang $(CFLAGS)
all        :         tower.o stack.o
	$(CC) -lm -o tower tower.o stack.o  
all.o      :         tower.c stack.c
	$(CC) -c tower.c stack.c
clean    :
	rm -f tower tower.o stack.o
infer    :
	make  clean; infer -capture  -- make; infer -analyze  -- make

