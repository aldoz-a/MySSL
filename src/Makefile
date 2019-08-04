#
#	FILE
#      	Makefile - makefile for MySSL library
#  	PROJECT
#       MySSL library
#	AUTHOR
#      	Aldo Abate
#   OPERATING SYSTEM
#       Linux (kernel 2.6 and above - glibc 2.3.2 and above)
#   COMPILER
#       GNU GCC (ver. 4.4 and above)
#  	RELEASE
#       0.1.0 (August 2019)
#

# common variables
#

# sources, objects and deps
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# shared libraries names
LIBMYSSL = libmyssl.so

# compiler and options
#

# compiler
CC = gcc

# library path
LIBS_PATH = ../tests/lib

# includes path
INCLUDES_PATH = ../tests/include

# compiler options
CPPFLAGS = -fpic -Wall -Wshadow -pedantic

#linker options
LDFLAGS = -shared -fpic -lssl -lcrypto

# targets
#

# all targets
all: libmyssl install-libs install-includes

# library libmyssl.so creation
libmyssl: $(OBJS)
	$(CC) $^ -o $(LIBMYSSL) $(LDFLAGS)

# copy libraries into the shared libs directory
install-libs: libmyssl
	cp -dpf $(LIBMYSSL) $(LIBS_PATH)

# copy includes in the shared includes directory
install-includes:
	cp -dpf myssl.h $(INCLUDES_PATH)

# object files creation
#

%.o: %.c
	$(CC) -MMD -MP $(CPPFLAGS) -c $< -o $@

# other directives
#

# phony directives
.PHONY: clean

# clean objects - $(RM) is rm -f by default
clean:
	$(RM) $(OBJS) $(DEPS) $(LIBMYSSL)

# deps creation
-include $(DEPS)
