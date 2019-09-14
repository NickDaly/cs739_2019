SDIR = .
CC=g++

CFLAGS = -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC -std=c++11  #-DNDEBUG -O3
LDFLAGS = -Wl,--enable-new-dtags -Wl,-rpath,-lpthread 
OBJS = 

all: lib739kv

lib739kv:	$(OBJS)
	$(CC) -shared -Wl,-soname,lib739kv.so -o lib739kv.so $(OBJS) $(LDFLAGS)

lib739kv.o:
	$(CC) $(CFLAGS) -c lib739kv.cpp

clean:
	rm -f $(OBJS) *.so *.o
