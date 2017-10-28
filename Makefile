IDIR = .
SDIR = .
ODIR = obj
LDIR = lib

CC=clang++-4.0
#CC=g++
#CC=g++-7

V = 0
ACTUAL_CC := $(CC)
CC_0 = @echo "Compiling $^ -> $@"; $(ACTUAL_CC)
CC_1 = $(ACTUAL_CC)
CC = $(CC_$(V))


CFLAGS= -g -std=c++14 -DBOOST_LOG_DYN_LINK -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wundef -fdiagnostics-show-option -Wno-unused-private-field -I$(IDIR)

LIBS=-lm -lpthread -lboost_log -lboost_log_setup  -lboost_system -lboost_thread 

#_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = hellomake.o hellofunc.o
CPP_FILES1 := $(wildcard $(SDIR)/*.cpp)

OBJ_FILES1 := $(addprefix $(ODIR)/,$(notdir $(CPP_FILES1:.cpp=.o)))

MKDIR_P = mkdir -p

.PHONY: directories

all: directories test

directories: ${ODIR}

${ODIR}:
	${MKDIR_P} ${ODIR}

test: $(OBJ_FILES1)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
