BIN_DIR = ./bin/
EXE = $(BIN_DIR)gen
SRC_DIR = ./src/
EXE_SRC = main.cc
GEN_DIR = ./generate/
LIB_DIR = ./lib/

C_COMPILER = gcc
CC_COMPILER = g++

CFLAGS = -g -std=c++11 
OPTS = -fPIC -shared

DYNAMIC = make_so
DYNAMIC_LIB = $(GEN_DIR)libroute.so
STATIC = make_a
STATIC_LIB = $(GEN_DIR)libroute.a

INCLUDE_PATH = -I$(SRC_DIR) 

LD_LIBRARY_PATH = -L$(LIB_DIR)

LD_FLAGS = -lglog -lm -lpthread

OBJ = route_select.o
OBJS_DIR = ./obj/
OBJS = $(addprefix $(OBJS_DIR), $(OBJ))

default: make_dir $(EXE)

all: make_dir $(DYNAMIC) $(STATIC) $(EXE)
	@echo '---------------- DONE FOR ALL ---------------'

$(EXE): $(OBJS)
	$(CC_COMPILER) $(EXE_SRC) -o $(EXE) $(INCLUDE_PATH) $(LD_LIBRARY_PATH) $(CFLAGS) $(OBJS) $(LD_FLAGS)

$(OBJS_DIR)%.o: $(SRC_DIR)%.cc
	$(CC_COMPILER) $(INCLUDE_PATH) $(LD_LIBRARY_PATH) $(CFLAGS) $(OPTS) -c $< -o $@

$(DYNAMIC): $(OBJS)
	$(CC_COMPILER) $(INCLUDE_PATH) $(LD_LIBRARY_PATH) $(CFLAGS) $(OPTS) $(OBJS) -o $(DYNAMIC_LIB)

$(STATIC):	$(OBJS)
	$(AR) rsv $(STATIC_LIB) $(OBJS)

run:
	bash ./run.sh

make_dir:
	mkdir -p $(BIN_DIR) $(OBJS_DIR) $(GEN_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJS_DIR) $(GEN_DIR)
