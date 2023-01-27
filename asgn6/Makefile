CFLAGS=-Wall -Wextra  -Werror  -pedantic  
CC=clang $(CFLAGS)
OBJECTS = hatterspeak.o bf.o bv.o hash.o ll.o parser.o speck.o
EXECBIN = hatterspeak
.PHONY: all
all        :         $(EXECBIN)
$(EXECBIN) :         $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECBIN)
hatterspeak.o  :     hatterspeak.c
	$(CC) -c hatterspeak.c
bf.o           :     bf.c
	$(CC) -c bf.c
bv.o           :     bv.c
	$(CC) -c bv.c
hash.o         :     hash.c
	$(CC) -c hash.c
ll.o           :     ll.c
	$(CC) -c ll.c
parser.o       :     parser.c
	$(CC) -c parser.c
speck.o        :     speck.c
	$(CC) -c speck.c
.PHONY: clean
clean    :
	rm $(OBJECTS) $(EXECBIN)
infer    :
	make  clean; infer -capture  -- make; infer -analyze  -- make

