TARGET=test
MODULE=

OBJS=cfg_parser.o main.o 
FLAGS=-Wall -g -O2
INCDIR=
LIBS=

CC=g++
SUBDIRS=

all: $(TARGET) $(MODULE) FORCE
.PHONY: all

$(SUBDIRS): FORCE
	@$(MAKE) -C $@

SUBDIRS_CLEAN = $(SUBDIRS:%=%/clean)
$(SUBDIRS_CLEAN): FORCE
	$(MAKE) `basename $@` -C `dirname $@`
.PHONY: $(SUBDIRS_CLEAN)

$(TARGET): $(SUBDIRS) $(OBJS)
	$(CC) $(OBJS) $(FLAGS) $(INCDIR) $(LIBS) -o $@

$(MODULE): $(SUBDIRS) $(OBJS)
	ld -r $(OBJS) $(LIBS) -o $(MODULE)

#	$(CC) $(OBJS) $(FLAGS) $(INCDIR) $(LIBS) -o $(MODULE)

%.o: %.cpp
	$(CC) $(FLAGS) $(INCDIR) -c $< -o $@

%.o: %.c
	$(CC) $(FLAGS) $(INCDIR) -c $< -o $@

clean: $(SUBDIRS_CLEAN) FORCE
	$(RM) $(TARGET) $(MODULE) $(OBJS)
.PHONY: clean

install: 

FORCE:
.PHONY: FORCE

