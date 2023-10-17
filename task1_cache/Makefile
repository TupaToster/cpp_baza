OS=$(shell uname)

CC=g++

OBJDIR=_objs/
DEPSDIR=_deps/

GPP_ERRORS_FILE=gpp_error.log

# Put names of all output files in here (without extension)
CACHE_OUT=cache

# Write sources variables here
SRC=cache.cpp cache_lib.cpp

# Put all the uncathegorized stuff here


# Till here
ifeq ($(OS), Windows_NT)
	OS=.exe
else
	OS=.out
endif

all: $(DEPSDIR) $(OBJDIR) $(addsuffix $(OS), $(CACHE_OUT))

-include $(addprefix $(DEPSDIR),*.d)

$(OBJDIR)%.o: %.cpp
	$(CC) -M $(CFLAGS) $< -o $(DEPSDIR)$(@F:.o=.d)
	echo "$(OBJDIR)" > line.tmp
	mv $(DEPSDIR)$(@F:.o=.d) input.tmp
	head -c -1 -q line.tmp input.tmp > $(DEPSDIR)$(@F:.o=.d)
	rm input.tmp line.tmp
	$(CC) -c $(CFLAGS) $< -o $@ 2> $(GPP_ERRORS_FILE)
# Write stuff here

$(addsuffix $(OS), $(CACHE_OUT)): $(addprefix $(OBJDIR), $(SRC:.cpp=.o))
	$(CC) $< -o $@ 2> $(GPP_ERRORS_FILE)

# Till here
$(OBJDIR):
	mkdir $(OBJDIR)

$(DEPSDIR):
	mkdir $(DEPSDIR)

clean:
	rm -r -f $(OBJDIR) $(DEPSDIR)
	rm -r -f *.png *.dot *.o *.out *.lol* *.exe* *.log
	clear

.PHONY: clean