UNAME := $(shell uname)

DEBUG=0

SLIB=lib739kv.so

VPATH=./src/:./testing
OBJDIR=./obj/

CC=gcc 

CPP=g++ -std=c++11 

OPTS=-Ofast 

LDFLAGS= -lm -pthread -ldl 

COMMON= -Iinclude/ -Isrc/

CFLAGS=-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC

ifeq ($(DEBUG), 1) 
	OPTS=-O0 -g
endif

#for MacOS clang we need to add libc++
ifeq ($(UNAME),Darwin)
	CPP += -stdlib=libc++ 
endif

CFLAGS+=$(OPTS)

#list of objects to build
OBJ=sqlite3.o \
    debug.o \
	sqlstatement.o \
	datastore.o \
	message.o \
	server.o \
	client.o \
	lib739kv.o \

#list of executable to build  
EXEC = test test_server test_client kvdaemon kvservice

EXECOBJA = $(addprefix $(OBJDIR), $(EXEC))

EXECOBJ = $(addsuffix .o, $(EXECOBJA))

OBJS = $(addprefix $(OBJDIR), $(OBJ))

DEPS = $(wildcard src/*.h) Makefile include/lib739kv.h

TESTFILES = test.db

all: obj $(SLIB) $(EXECOBJ) $(EXEC)

$(SLIB): $(OBJS)
	$(CPP) $(CFLAGS) -shared $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CPP) $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@
	
$(EXEC): $(EXECOBJ) $(SLIB)
	$(CPP) $(COMMON) $(CFLAGS) $(OBJDIR)$@.o -o $@ $(LDFLAGS) $(SLIB)

obj:
	mkdir -p obj

.PHONY: clean

clean:
	rm -rf $(OBJS) $(SLIB) $(EXEC) $(EXECOBJ) $(OBJDIR) $(EXEC) $(TESTFILES)
