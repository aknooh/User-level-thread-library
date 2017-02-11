# Target programs
programs := test1 test2 test3 test4 test5 premptest1 premptest2

# User-level thread library
UTHREADLIB=libuthread
libuthread := $(UTHREADLIB)/$(UTHREADLIB).a

# Default rule
all: $(libuthread) $(programs)

# Avoid builtin rules and variables
MAKEFLAGS += -rR

# Don't print the commands unless explicitely requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Current directory
CUR_PWD := $(shell pwd)

# Define compilation toolchain
CC	= gcc

# General gcc options
CFLAGS	:= -Werror
#CFLAGS	+= -O2
CFLAGS	+= -O0
CFLAGS	+= -g
CFLAGS	+= -pipe

# Include path
INCLUDE := -I$(UTHREADLIB)

# Generate dependencies
DEPFLAGS = -MMD -MF $(@:.o=.d)

# Application objects to compile
objs := $(patsubst %,%.o,$(programs))

# Include dependencies
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

# Rule for libuthread.a
$(libuthread):
	@echo "MAKE	$@"
	$(Q)$(MAKE) V=$(V) -C $(UTHREADLIB)

# Generic rule for linking final applications
%: %.o $(libuthread)
	@echo "LD	$@"
	$(Q)$(CC) $(CFLAGS) -o $@ $< -L$(UTHREADLIB) -luthread

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $< $(DEPFLAGS)

# Cleaning rule
clean:
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)$(MAKE) V=$(V) -C $(UTHREADLIB) clean
	$(Q)rm -rf $(objs) $(deps) $(programs)

.PHONY: clean $(libuthread)

