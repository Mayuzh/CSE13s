CFLAGS=-Wall -Wextra  -Werror  -pedantic  
CC=clang $(CFLAGS)
lrc      :         math.o
	$(CC) -o math math.o -lm
lrc.o    :         math.c
	$(CC) -c math.c
clean    :
	rm -f math math.o
infer    :
	make  clean; infer -capture  -- make; infer -analyze  -- make

