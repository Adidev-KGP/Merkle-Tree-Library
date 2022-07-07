CC = gcc
CFLAGS = -g 
CFLAGS +=  
CPPFLAGS += 

LDLIBS += -lm -lcrypto #-lm for pow inside math.h , -lcrypto for openssl

BINS = a.out

all: ./integration/server.c ./verifier/verifier.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDLIBS)  

clean:
	rm *.o $(BINS)