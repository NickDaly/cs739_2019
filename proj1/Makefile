UNAME := $(shell uname)

DEBUG=0

SLIB=lib739kv.so
EXEC=test

VPATH=./src/:./tests
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
	server.o \
	lib739kv.o 

#the executable obj
EXECOBJA=test.o 

EXECOBJ = $(addprefix $(OBJDIR), $(EXECOBJA))

OBJS = $(addprefix $(OBJDIR), $(OBJ))

DEPS = $(wildcard src/*.h) Makefile include/lib739kv.h

all: obj  $(SLIB) $(EXEC)

$(EXEC): $(EXECOBJ) $(SLIB)
	$(CPP) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(SLIB)

$(SLIB): $(OBJS)
	$(CPP) $(CFLAGS) -shared $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: %.cpp $(DEPS)
	$(CPP) $(COMMON) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

.PHONY: clean

clean:
	rm -rf $(OBJS) $(SLIB) $(EXEC) $(EXECOBJ) $(OBJDIR) test.db