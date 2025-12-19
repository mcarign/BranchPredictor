CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
# You can select a C++ standard using the STD define below.  To do so, uncomment (remove leading #) and adjust the standard as needed.
#STD = -std=c++11
CFLAGS = $(OPT) $(WARN) $(STD) $(INC) $(LIB)

# List all your .cc/.cpp files here (source files, excluding header files)
SIM_SRC = sim_bp.cc predictor.cpp

# List corresponding compiled object files here (.o files)
SIM_OBJ = sim_bp.o predictor.o
 
#################################

# default rule

all: sim run_sims
	@echo "my work is done here..."
	
run_sims:	
	for m in {7..20}; do\
		./sim bimodal $$m traces/gcc_trace.txt > outputs/bim/out_bim_gcc_$$m.txt;\
		./sim bimodal $$m traces/perl_trace.txt > outputs/bim/out_bim_perl_$$m.txt;\
		./sim bimodal $$m traces/jpeg_trace.txt > outputs/bim/out_bim_jpeg_$$m.txt;\
		for n in $$(seq 0 $$m); do\
			./sim gshare $$m $$n traces/gcc_trace.txt > outputs/gsh/out_gsh_gcc_$${m}_$${n}.txt;\
		done;\
	done

# rule for making sim

sim: $(SIM_OBJ)
	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH sim-----------"


# generic rule for converting any .cpp file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc

.cpp.o:
	$(CC) $(CFLAGS)  -c $*.cpp


# type "make clean" to remove all .o files plus the sim binary

clean:
	rm -f *.o sim


# type "make clobber" to remove all .o files (leaves sim binary)

clobber:
	rm -f *.o


