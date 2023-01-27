CFLAGS=-Wall -Wextra  -Werror  -Wpedantic  
CC=clang $(CFLAGS)
all        :         sequence.o sieve.o bv.o
	$(CC) -lm -o sequence sequence.o sieve.o bv.o
all.o      :         sequence.c sieve.c bv.c
	$(CC) -c sequence.c sieve.c bv.c
clean    :
	rm -f sequence sequence.o sieve.o bv.o
infer    :
	make  clean; infer -capture  -- make; infer -analyze  -- make

