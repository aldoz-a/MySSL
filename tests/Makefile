#
#	FILE
#      	Makefile - makefile for MySSL library test
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

SRV = server
CLI = client

# sources, objects and deps
SRCS_SRV = $(wildcard $(SRV)/*.c)
SRCS_CLI = $(wildcard $(CLI)/*.c)
OBJS_SRV = $(SRCS_SRV:.c=.o)
OBJS_CLI = $(SRCS_CLI:.c=.o)
DEPS_SRV = $(SRCS_SRV:.c=.d)
DEPS_CLI = $(SRCS_CLI:.c=.d)

# compiler and options
#

# compiler
CC = gcc

# library path
LIBS_PATH = lib

# includes path
INCLUDES_PATH = include

# compiler options
CPPFLAGS = -I$(INCLUDES_PATH) -Wall -Wshadow -pedantic

#linker options
LDFLAGS = -L$(LIBS_PATH) -lssl -lcrypto -lmyssl

# targets
#

# all targets
all: server client

# target executable file creation
server: $(OBJS_SRV)
	$(CC) $^ -o $(SRV)/$@ $(LDFLAGS)

# target executable file creation
client: $(OBJS_CLI)
	$(CC) $^ -o $(CLI)/$@ $(LDFLAGS)

# object files creation
#

# motor and lib objects
%.o: %.c
	$(CC) -MMD -MP $(CPPFLAGS) -c $< -o $@

# other directives
#

# phony directives
.PHONY: clean

# clean objects - $(RM) is rm -f by default
clean:
	$(RM) $(OBJS_SRV) $(OBJS_CLI) $(DEPS_SRV) $(DEPS_CLI)

# deps creation
-include $(DEPS_SRV) $(DEPS_CLI)
