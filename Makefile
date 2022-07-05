CC = gcc
CFLAGS = -g 
CFLAGS +=  
CPPFLAGS += 

LDLIBS += -lm -lcrypto #-lm for pow inside math.h , -lcrypto for openssl

BINS = a.out

all: ./Integration/server.c ./Verifier/verifier.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDLIBS)  

clean:
	rm *.o $(BINS)