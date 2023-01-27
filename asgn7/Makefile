CFLAGS = -Wall -Wextra -Werror -Wpedantic
CC = clang $(CFLAGS)

target1 = encode
target2 = decode

src  = $(wildcard *.c)
deps = $(wildcard *.h)
obj  = $(patsubst %.c, %.o, $(src))

all: $(target1) $(target2)

$(target1): encode.o trie.o io.o
	$(CC) $^ -o $@ -Wall -lm

$(target2): decode.o word.o io.o 
	$(CC) $^ -o $@ -Wall -lm

%.o: %.c $(deps)
	$(CC) -c $< -o $@ -Wall

.PHONY: clean
clean:
	-rm -rf $(target1) $(target2) $(obj)
